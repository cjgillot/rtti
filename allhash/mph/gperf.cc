#include "overloads.hpp"

#include <memory.h>

#define __OPTIMIZE__

#include "keyword.h"
// #include "keyword.icc"
#include "keyword-list.h"
// #include "keyword-list.icc"
#include "search.h"
#include "options.h"
// #include "options.icc"
// #include "positions.icc"
#include "version.h"

#undef __OPTIMIZE__

#include <iostream>
#include <iomanip>

namespace {

// code taken from gperf/main.cc
class KeywordExt_Factory : public Keyword_Factory
{
public:
  virtual KeywordExt * create_keyword (const char *allchars, int allchars_length,
                                          const char *rest)
  {
    return new KeywordExt (allchars, allchars_length, rest);
  }
};

static void load_keys(KeywordExt_List** list_tail, KeywordExt_Factory* factory, const dispatch_t& dispatch, size_t arity)
{
  size_t lineno = 0;

  for(auto& p : dispatch)
  {
    const signature_t& sig = p.first;

    size_t* mytext = new size_t [ arity ];

    for(size_t i = 0; i < arity; ++i)
      mytext[i] = sig.array()[i]->rank;

    /* Allocate Keyword and add it to the list.  */
    KeywordExt *new_kw = factory->create_keyword((char*)mytext, arity * sizeof(size_t), empty_string);
    new_kw->_lineno = lineno++;
    *list_tail = new KeywordExt_List (new_kw);
    list_tail = &(*list_tail)->rest();
  }
}

/* Returns the smallest unsigned C type capable of holding integers
   up to N.  */
static const char *
smallest_integral_type (int n)
{
  if (n <= UCHAR_MAX) return "unsigned char";
  if (n <= USHRT_MAX) return "unsigned short";
  return "unsigned int";
}

class Output
{
public:
  /* Constructor.  */
  Output (Search& searcher, std::ostream& os)
  : _head(searcher._head)
  , _total_keys(searcher._total_keys)
  , _max_key_len(searcher._max_key_len)
  , _min_key_len(searcher._min_key_len)
  , _hash_includes_len(searcher._hash_includes_len)
  , _key_positions(searcher._key_positions)
  , _alpha_inc(searcher._alpha_inc)
  , _total_duplicates(searcher._total_duplicates)
  , _alpha_size(searcher._alpha_size)
  , _asso_values(searcher._asso_values)
  , _stream(os)
  {}

  /* Generates the hash function and the key word recognizer function.  */
  void                  output ()
  {
    compute_min_max ();

    _stream << "/* ANSI-C code produced by gperf version " << version_string << "+mmethod */" << std::endl;
//     option.print_options ();
//     _stream << std::endl;
//     if (!option[POSITIONS])
//       {
//         _stream << "/* Computed positions: -k'";
//         _key_positions.print();
//         _stream << "' */" << std::endl;
//       }
    _stream << std::endl;

    output_constants();

    _stream
      << "/* maximum key range = " << _max_hash_value - _min_hash_value + 1
      << ", duplicates = " << _total_duplicates << " */" << std::endl << std::endl;

    output_hash_function ();

    _stream << std::flush;
  }

private:
  /* Computes the minimum and maximum hash values, and stores them
     in _min_hash_value and _max_hash_value.  */
  void                  compute_min_max ()
  {
    /* Since the list is already sorted by hash value all we need to do is
      to look at the first and the last element of the list.  */

    _min_hash_value = _head->first()->_hash_value;

    KeywordExt_List *temp;
    for (temp = _head; temp->rest(); temp = temp->rest())
      ;
    _max_hash_value = temp->first()->_hash_value;
  }


  /* Returns the number of different hash values.  */
  int                   num_hash_values () const
  {
    /* Since the list is already sorted by hash value and doesn't contain
      duplicates, we can simply count the number of keywords on the list.  */
    int count = 0;
    for (KeywordExt_List *temp = _head; temp; temp = temp->rest())
      count++;
    return count;
  }

  /* Outputs the maximum and minimum hash values etc.  */
  void                  output_constants () const
  {
    _stream << "enum {" << std::endl;
    _stream <<  "\tTOTAL_KEYWORDS  = " << _total_keys << std::endl;
    _stream << ",\tMIN_WORD_LENGTH = " << _min_key_len << std::endl;
    _stream << ",\tMAX_WORD_LENGTH = " << _max_key_len << std::endl;
    _stream << ",\tMIN_HASH_VALUE  = " << _min_hash_value << std::endl;
    _stream << ",\tMAX_HASH_VALUE  = " << _max_hash_value << std::endl;
    _stream << "};" << std::endl;
  }

  /* Generates a C expression for an asso_values[] reference.  */
  void                  output_asso_values_ref (int pos) const
  {
    _stream << "asso_values[";
    /* Always cast to unsigned char.  This is necessary when the alpha_inc
      is nonzero, and also avoids a gcc warning "subscript has type 'char'".  */
    _stream << "(unsigned char)";
    if (pos == Positions::LASTCHAR)
      _stream << "str[len - 1]";
    else
      {
        _stream << "str[" << pos << "]";
        if (_alpha_inc[pos])
          _stream << "+" << _alpha_inc[pos];
      }
    _stream << "]";
  }


  /* Generates C code for the hash function that returns the
     proper encoding for each keyword.  */
  void                  output_hash_function () const
  {
    /* Output the function's head.  */
    _stream
      << "static inline size_t hash(register const unsigned char *str) {\n"
      << "\tenum { len = ARITY * sizeof(size_t) };"
      << std::endl;

    /* First the asso_values array.  */
    if (_key_positions.get_size() > 0)
      {
        _stream
          << "\tstatic const "
          << smallest_integral_type(_max_hash_value + 1)
          << " asso_values[] = {";

        const int columns = 10;

        /* Calculate maximum number of digits required for MAX_HASH_VALUE.  */
        int field_width = 2;
        for (int trunc = _max_hash_value; (trunc /= 10) > 0;)
          field_width++;

        const unsigned tbl_size = /*_alpha_size,*/ compute_table_size();
        _stream << std::hex;
        for (unsigned int count = 0; count < tbl_size; count++)
          {
            if (count > 0) _stream << ",";
            if ((count % columns) == 0) _stream << std::endl << "\t\t";
            _stream << "0x" << _asso_values[count];
          }
        _stream << std::dec;

        _stream << std::endl << "\t};" << std::endl;
      }

    if (_key_positions.get_size() == 0)
      {
        /* Trivial case: No key positions at all.  */
        _stream << "\treturn " << (_hash_includes_len ? "len" : "0") << ";" << std::endl;
      }
    else
      {
        /* Iterate through the key positions.  Remember that Positions::sort()
          has sorted them in decreasing order, with Positions::LASTCHAR coming
          last.  */
        PositionIterator iter = _key_positions.iterator(_max_key_len);
        int key_pos;

        /* Get the highest key position.  */
        key_pos = iter.next ();

        if (key_pos == Positions::LASTCHAR || key_pos < _min_key_len)
          {
            /* We can perform additional optimizations here:
              Write it out as a single expression. Note that the values
              are added as 'int's even though the asso_values array may
              contain 'unsigned char's or 'unsigned short's.  */

            _stream << "\treturn " << (_hash_includes_len ? "len + " : "");

            if (_key_positions.get_size() == 2
                && _key_positions[0] == 0
                && _key_positions[1] == Positions::LASTCHAR)
              /* Optimize special case of "-k 1,$".  */
              {
                output_asso_values_ref (Positions::LASTCHAR);
                _stream << " + ";
                output_asso_values_ref (0);
              }
            else
              {
                for (; key_pos != Positions::LASTCHAR; )
                  {
                    output_asso_values_ref (key_pos);
                    if ((key_pos = iter.next ()) != PositionIterator::EOS)
                      _stream << " + ";
                    else
                      break;
                  }

                if (key_pos == Positions::LASTCHAR)
                  output_asso_values_ref (Positions::LASTCHAR);
              }

            _stream << ";" << std::endl;
          }
        else
          {
            /* We've got to use the correct, but brute force, technique.  */
            _stream
              << "\tregister int hval = " << (_hash_includes_len ? "len" : "0") << ";" << std::endl << std::endl
              << "\tswitch(" << (_hash_includes_len ? "hval" : "len") << ") {" << std::endl
              << "\t\tdefault:" << std::endl;

            while (key_pos != Positions::LASTCHAR && key_pos >= _max_key_len)
              if ((key_pos = iter.next ()) == PositionIterator::EOS)
                break;

            if (key_pos != PositionIterator::EOS && key_pos != Positions::LASTCHAR)
              {
                int i = key_pos;
                do
                  {
//                     if (i > key_pos)
//                       printf ("      /*FALLTHROUGH*/\n"); /* Pacify lint.  */
                    for ( ; i > key_pos; i--)
                      _stream << "\t\tcase " << i << ":" << std::endl;

                    _stream << "\t\t\thval += ";
                    output_asso_values_ref (key_pos);
                    _stream << ";" << std::endl;

                    key_pos = iter.next ();
                  }
                while (key_pos != PositionIterator::EOS && key_pos != Positions::LASTCHAR);

//                 if (i >= _min_key_len)
//                   printf ("      /*FALLTHROUGH*/\n"); /* Pacify lint.  */
                for ( ; i >= _min_key_len; i--)
                  _stream << "\t\tcase " << i << ":" << std::endl;
              }

            _stream << "\t\t\tbreak;" << std::endl
                    << "\t}" << std::endl
                    << "\treturn hval";
            if (key_pos == Positions::LASTCHAR)
              {
                _stream << " + ";
                output_asso_values_ref (Positions::LASTCHAR);
              }
            _stream << ";" << std::endl;
          }
      }
    _stream << "}" << std::endl << std::endl;
  }


  unsigned int          compute_table_size() const
  {
    unsigned int ret = 0;
    for(KeywordExt_List* temp = _head; temp; temp = temp->rest())
    {
      KeywordExt *keyword = temp->first();
      for(int i = 0; i < keyword->_allchars_length; ++i)
        ret = std::max(ret, (unsigned char)keyword->_allchars[i] + _alpha_inc[i]);
    }
    return std::min(ret, _alpha_size);
  }

  /* Linked list of keywords.  */
  KeywordExt_List *     _head;

  /* Total number of keys, counting duplicates. */
  int const             _total_keys;
  /* Maximum length of the longest keyword. */
  int const             _max_key_len;
  /* Minimum length of the shortest keyword. */
  int const             _min_key_len;
  /* Whether the hash function includes the length.  */
  bool                  _hash_includes_len;
  /* Key positions.  */
  Positions const       _key_positions;
  /* Adjustments to add to bytes add specific key positions.  */
  const unsigned int * const _alpha_inc;
  /* Total number of duplicate hash values. */
  int const             _total_duplicates;
  /* Minimum hash value for all keywords. */
  int                   _min_hash_value;
  /* Maximum hash value for all keywords. */
  int                   _max_hash_value;
  /* Size of alphabet. */
  unsigned int const    _alpha_size;
  /* Value associated with each character. */
  const int * const     _asso_values;

  std::ostream&         _stream;
};

static void print_table(
  KeywordExt_List* list,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch, size_t arity,
  std::ostream& f
) {
  const size_t min_hash = list->first()->_hash_value;
  size_t index;
  KeywordExt_List *temp;

  f << "static invoker_t invoker_table [] = {" << std::endl;

  /* Generate an array of reserved words at appropriate locations.  */

  for (temp = list, index = min_hash; temp; temp = temp->rest())
  {
    KeywordExt *keyword = temp->first();

    if (index > min_hash)
      f << ",\t";
    else
      f << "\t";

    while(index < (size_t)keyword->_hash_value)
    {
      /* Some blank entries.  */
      f << "nullptr/*fill*/\n,\t";
      ++index;
    }

    const size_t* ranks = (const size_t*)keyword->_allchars;
    signature_t sig { arity };
    for(size_t i = 0; i < arity; ++i)
      sig.array()[i] = pole_table[i][ ranks[i] ];
    auto& inv = dispatch.at(sig);

    if(!inv)
      f << "nullptr";
    else
    {
      auto& b = inv->array();
      f << "MMETHOD::overload<0";
      std::for_each(
        b.begin()
      , b.end()
      , [&f](const klass_t* k)
        { f << ", " << k->hash << "ul"; }
      );
      f << ">::address";
    }

    f << "/* ";
    for(size_t i = 0; i < arity; ++i)
      f << ranks[i] << " ";
    f << "*/" << std::endl;

    ++index;
  }

  f << "};" << std::endl << std::endl;
}

} // anonymous namespace

void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch, size_t arity,
  std::ostream& f
) {
  if(arity == 1)
    return;

  KeywordExt_Factory factory;

  KeywordExt_List* list;
  load_keys(&list, &factory, dispatch, arity);

  option.set_asso_iterations(10); option.set_size_multiple(1./10.);

  {
    Search searcher { list };
    searcher.optimize();
    list = searcher._head;

    Output out { searcher, f };
    out.output();
  }

  print_table(list, pole_table, dispatch, arity, f);

  /* Also delete the list that was allocated inside Input and reordered
      inside Search.  */
  for (KeywordExt_List *ptr = list; ptr; ptr = ptr->rest())
    {
      KeywordExt *keyword = ptr->first();
      do
        {
          KeywordExt *next_keyword = keyword->_duplicate_link;
          delete[] const_cast<unsigned int *>(keyword->_selchars);
          if (keyword->_rest != empty_string)
            delete[] const_cast<char*>(keyword->_rest);
          delete[] const_cast<char*>(keyword->_allchars);
          delete keyword;
          keyword = next_keyword;
        }
      while (keyword != NULL);
    }
  delete_list (list);
}
