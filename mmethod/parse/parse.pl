#!/usr/bin/perl
use strict;
use warnings;

# decl, name, nspace, arity, fullarity, tags
sub output_preabule;

# decl, hashes
sub output_overload;

# output
sub do_compile;

{
  use Getopt::Long;
  
  my $help = 0;
  my $verbose = 0;
  my $outname = "a.mmo";
  GetOptions( "h|help"     => \$help,
              "v|verbose"  => \$verbose,
              "o|output=s" => \$outname
            )
  or $help = 2;

  if($help) {
    print STDERR <<USAGE ;
Usage: parse [options] [input]
  -h, --help            display this help and exit
  -v, --verbose         enable verbose output
  -o, --output          output file name [a.mmo]

If [input] is not set, the program will read from standard input.
USAGE
    exit($help == 2);
  }

  open(my $outfile, ">$outname") or die("Cannot open file $outname");
  
  do_compile $outfile;
  
  close($outfile);
}

sub do_compile {
  my $out = shift;

  my %registry;

  # read compiler output
  while(my $line = <>)
  {
    # parse compiler line
    if($line =~ /.*\[with T = rtti::mmethod::mmethod_(\w+)<((\w+::)*)(\w+),\s*(\d+)ul\s*,\s*(\d+)ul\s*,\s*(.*)>\].*/) {
      my $predicate = $1;
      my( $nspace, $name, $vsize, $fullarity ) = ($2, $4, $5, $6);
      my $tail = $7;
      
      my $key = "::" . $nspace . $name;
      
      # add decl to registry
      $registry{$key} ||= {};
      my $decl = $registry{$key};
      
      # fill declaration
      if( $predicate eq 'declare' ) {
        $decl->{name} = $name;
        $decl->{nspace} = $nspace;
        $decl->{vsize} = $vsize;
        $decl->{fullarity} = $fullarity;
        
        $tail =~ s/(\d)ul/$1/;
        $decl->{argpos} = $tail;
      }
      # fill overloads
      elsif( $predicate eq 'implement' ) {
        output_overload( $decl, $tail );
      }
    }
  }
  
  # output
  {
    # print size, useful for reading
    print{ $out } (scalar keys %registry), "\n";
    
    while(my($key, $decl) = each (%registry)) {
      print{ $out } $key, "\n";
      print{ $out } '  ', $decl->{name}, "\n";
      print{ $out } '  ', $decl->{nspace}, "\n";
      print{ $out } '  ', $decl->{vsize}, "\n";
      print{ $out } '  ', $decl->{fullarity}, "\n";
      print{ $out } '  ', $decl->{argpos}, "\n";
      
      # print overloads
      my @overloads = @{ $decl->{overloads} };
      print{ $out } '  ', scalar @overloads, "\n";
      foreach my $overload ( @overloads ) {
        # size shall be vsize
        print{ $out } "  {\n";
        foreach my $hashpath ( @$overload ) {
          print{ $out } '    [';
          foreach my $hash ( @$hashpath ) {
            print{ $out } $hash, ' ';
          }
          print{ $out } "]\n";
        }
        print{ $out } "  }\n";
      }
    }
  }
}

sub output_overload {
  my( $decl, $hashes ) = @_;
  
  $hashes =~ s/rtti::mpl::mplpack//g;
  $hashes =~ s/rtti::hash::path_end//g;
  $hashes =~ s/rtti::hash::path_node//g;
  
  my @pathes = split('>, <', $hashes);

  my @overload = ();
  
  foreach my $path (@pathes) {
    my @hashpath = split( /[, <>ul]/, $path);
    $overload[@overload] = \@hashpath;
  }
  
  $decl->{overloads} ||= [];
  my $overloads = $decl->{overloads};
  $$overloads[@$overloads] = \@overload;
}
