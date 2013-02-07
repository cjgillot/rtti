#include "overloads.hpp"
#include "archive.hpp"

#ifndef __OPTIMIZE__
#define __OPTIMIZE__
#endif

#include "keyword.h"
#include "keyword-list.h"
#include "search.h"
#include "version.h"

namespace {

// code taken from gperf/main.cc
class KeywordExt_Factory
: public Keyword_Factory
{
public:
  virtual KeywordExt * create_keyword (const char *allchars, int allchars_length,
                                          const char *rest)
  {
    return new KeywordExt (allchars, allchars_length, rest);
  }
};

static void load_keys(
  KeywordExt_List** list_tail,
  KeywordExt_Factory* factory,
  const dispatch_t& dispatch,
  const arch_declaration& decl
) {
  unsigned lineno = 0;

  for(auto& p : dispatch) {
    const signature_t& sig = p.first;

    std::size_t* mytext = new std::size_t [ decl.vsize ];

    { // fill up mytext
      std::size_t* txt = mytext;
      for(std::size_t pos : decl.argpos)
        *txt++ = sig.array()[pos]->rank;
    }

    /* Allocate Keyword and add it to the list.  */
    KeywordExt *new_kw = factory->create_keyword((const char*)mytext, decl.vsize * sizeof(std::size_t), empty_string);
    new_kw->_lineno = lineno++;
    *list_tail = new KeywordExt_List (new_kw);
    list_tail = &(*list_tail)->rest();
  }
}

class MMOutput {
public:
  /* Constructor.  */
  MMOutput (Search const& searcher, std::ostream& os)
  : _head(searcher._head)
  , _total_keys(searcher._total_keys)
  , _key_len(searcher._max_key_len)
  , _key_positions(searcher._key_positions)
  , _alpha_inc(searcher._alpha_inc)
  , _asso_values(searcher._asso_values)
  , _stream(os)
  {
    assert(  searcher._max_key_len == searcher._min_key_len );
    assert( !searcher._hash_includes_len );
    assert(  searcher._total_duplicates == 0 );
  }

  /* Generates the hash function and the key word recognizer function.  */
  void output(
    const pole_table_t& pole_table,
    const dispatch_t& dispatch,
    const arch_declaration& decl
  ) {
    rerank(dispatch);

    compute_min_max ();

    _stream << "/* ANSI-C code produced by gperf version " << version_string << "+mmethod */" << std::endl;
    _stream << std::endl;

    output_constants();

    std::size_t const arity = decl.fullarity;

    { // invoker_table
      _stream << "static invoker_t _impl_invoker_table[] = {\n";
      for(std::size_t i = _max_hash_value - _min_hash_value + 1; i; --i)
        _stream << "\t&_rtti_bad_dispatch,\n";
      _stream << "};" << std::endl;
    }
    _stream << "static void _impl_inittable() {" << std::endl;

    /* Generate an array of reserved words at appropriate locations.  */

    for(KeywordExt_List* temp = _head; temp; temp = temp->rest()) {
      KeywordExt *keyword = temp->first();
      std::size_t index = keyword->_hash_value - _min_hash_value;

      const std::size_t* ranks = (const std::size_t*)keyword->_allchars;
      signature_t sig { arity };
      std::transform(
        pole_table.begin(), pole_table.end(),
        sig.array_ref().begin(),
        [](std::vector<klass_t const*> const& v)
        { return v[0]; }
      );
      for(std::size_t i = 0; i < decl.vsize; ++i) {
        std::size_t j = decl.argpos[i];
        sig.array_ref()[j] = pole_table[j][ ranks[i] ];
      }
      auto& inv = dispatch.at(sig);

      if(!inv)
        continue;

      _stream << "\t_impl_invoker_table[" << index << "] = ";
      auto& b = inv->array();
      _stream << "MMETHOD::overload<detail::mpl::mplpack_c<0";
      for(klass_t const* k : b)
        _stream << ", " << k->hash << "ul";
      _stream << ">>::address";

      _stream << "/* ";
      for(std::size_t i = 0; i < decl.vsize; ++i)
        _stream << ranks[i] << " ";
      _stream << "*/";

      _stream << ";" << std::endl;
    }

    _stream << "}" << std::endl;
  }

private:
  void                  rerank (dispatch_t const& dispatch) {
    for(auto& p : dispatch) {
      signature_t const& sig = p.first;
      auto& array = sig.array();

      PositionIterator iter = _key_positions.iterator(_key_len);
      int key_pos = iter.next();

      std::vector<std::size_t> rankhashes ( array.size(), 0ul );

      for (; key_pos != PositionIterator::EOS; key_pos = iter.next()) {
        std::size_t index = key_pos / sizeof(std::size_t);

        std::size_t old = array[index]->rank;
        rankhashes[index] += rerank_asso_values_ref( old, key_pos );
      }

      for(std::size_t index = 0; index < array.size(); ++index)
        const_cast<klass_t*>( array[index] )->rankhash = rankhashes[index];
    }
  }

  /* Computes the minimum and maximum hash values, and stores them
     in _min_hash_value and _max_hash_value.  */
  void                  compute_min_max () {
    /* Since the list is already sorted by hash value all we need to do is
      to look at the first and the last element of the list.  */

    _min_hash_value = _head->first()->_hash_value;

    KeywordExt_List *temp;
    for (temp = _head; temp->rest(); temp = temp->rest())
      ;
    _max_hash_value = temp->first()->_hash_value;
  }

  /* Outputs the maximum and minimum hash values etc.  */
  void                  output_constants () const {
    _stream << "enum {" << std::endl;
    _stream <<  "\tTOTAL_KEYWORDS  = " << _total_keys << std::endl;
    _stream << ",\tWORD_LENGTH     = " << _key_len << std::endl;
    _stream << ",\tMIN_HASH_VALUE  = " << _min_hash_value << std::endl;
    _stream << ",\tMAX_HASH_VALUE  = " << _max_hash_value << std::endl;
    _stream << "};" << std::endl;
    _stream << "/* maximum key range = " << _max_hash_value - _min_hash_value + 1 << " */" << std::endl << std::endl;
  }

  /* Generates the image of an asso_values[] reference.  */
  std::size_t           rerank_asso_values_ref (std::size_t in, int const pos) const {
    static_assert(Positions::LASTCHAR == -1, "Oops!");
    int bitpos;
    if(pos == Positions::LASTCHAR)
      bitpos = sizeof(std::size_t) - 1;
    else
      bitpos = pos % sizeof(std::size_t);

    unsigned char index = reinterpret_cast<unsigned char*>(&in)[bitpos];
    if(pos != Positions::LASTCHAR) index += _alpha_inc[pos];
    return _asso_values[index];
  }

private:
  /* Linked list of keywords.  */
  KeywordExt_List *     _head;

  /* Total number of keys, counting duplicates. */
  int const             _total_keys;
  /* Length of the keywords. */
  int const             _key_len;
  /* Key positions.  */
  Positions const       _key_positions;
  /* Adjustments to add to bytes add specific key positions.  */
  const unsigned int * const _alpha_inc;
  /* Minimum hash value for all keywords. */
  int                   _min_hash_value;
  /* Maximum hash value for all keywords. */
  int                   _max_hash_value;
  /* Value associated with each character. */
  const int * const     _asso_values;

  std::ostream&         _stream;
};

} // anonymous namespace

void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const arch_declaration& decl,
  std::ostream& f
) {
  if(decl.vsize == 1) return;

  KeywordExt_Factory factory; KeywordExt_List* list;
  load_keys(&list, &factory, dispatch, decl);

  option.set_asso_iterations(10); option.set_size_multiple(1./10.); option.set(NOLENGTH);

  {
    Search searcher { list };
    searcher.optimize();
    list = searcher._head;

    MMOutput out { searcher, f };
    out.output(pole_table, dispatch, decl);
  }

  /* Also delete the list that was allocated inside Input and reordered
      inside Search.  */
  for(KeywordExt_List *ptr = list; ptr; ptr = ptr->rest()) {
    for(KeywordExt *keyword = ptr->first(), *next_keyword; keyword; keyword = next_keyword) {
      next_keyword = keyword->_duplicate_link;

      delete[] keyword->_selchars;
      if (keyword->_rest != empty_string)
        delete[] keyword->_rest;
      delete[] (std::size_t*)keyword->_allchars;
      delete keyword;
    }
  }
  delete_list(list);
}
