
#include <iostream>

#include <mm/cache_set.hpp>

typedef mm::cache_set< std::string> Set;

void lookup( const Set& set, const std::string& word )
{
    Set::iterator it = set.find( word );
    
    std::cout << word << ": "
              << ( it != set.end() ? "present" : "==== not present ====" )
              << std::endl;
}

int main()
{
    Set set;
    set.resize( 100 );
    
    set.insert( "kiwi" );
    set.insert( "plum" );
    set.insert( "apple" );
    set.insert( "mango" );
    set.insert( "apricot" );
    set.insert( "banana" );

    lookup( set, "mango" );
    lookup( set, "apple" );
    lookup( set, "durian" );

    std::cout << std::endl << " - All the elements - "  << std::endl;

    Set::const_iterator it;
    for ( it = set.begin(); it != set.end(); ++it )
        std::cout << *it << " ";

    std::cout << std::endl;
}
