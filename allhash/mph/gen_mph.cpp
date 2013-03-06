#ifdef GEN_MPH_CPP

#include "overloads.hpp"

extern "C" {
#include "gen_mph/perfect.h"
#include "gen_mph/recycle.h"
}

/* get the list of keys */
static void getkeys(key** keys, ub4* nkeys, reroot* keyroot, reroot* textroot, hashform* form, const dispatch_t& dispatch, ub4 arity)
// key      **keys;                                         /* list of all keys */
// ub4       *nkeys;                                          /* number of keys */
// reroot    *textroot;                          /* get space to store key text */
// reroot    *keyroot;                                    /* get space for keys */
// hashform  *form;                                          /* user directives */
{
  key  *mykey;

  size_t *mytext;
  mytext = (size_t*)renew(textroot);

  *keys = 0;
  *nkeys = 0;
  for(auto& p : dispatch)
  {
    const signature_t& sig = p.first;

    mykey = (key *)renew(keyroot);

    for(size_t i = 0; i < arity; ++i)
      mytext[i] = sig.array()[i]->hash;

//     if (form->mode == AB_HM)
//     {
//       sscanf(mytext, "%lx %lx ", &mykey->a_k, &mykey->b_k);
//     }
//     else if (form->mode == ABDEC_HM)
//     {
//       sscanf(mytext, "%ld %ld ", &mykey->a_k, &mykey->b_k);
//     }
//     else if (form->mode == HEX_HM)
//     {
//       sscanf(mytext, "%lx ", &mykey->hash_k);
//     }
//     else if (form->mode == DECIMAL_HM)
//     {
//       sscanf(mytext, "%ld ", &mykey->hash_k);
//     }
//     else
    {
      mykey->name_k = (ub1 *)mytext;
      mytext = (size_t*)renew(textroot);
      mykey->len_k  = (ub4)(arity * sizeof(size_t)/sizeof(ub1));
    }
    mykey->next_k = *keys;
    *keys = mykey;
    ++*nkeys;
  }
  redel(textroot, mytext);
}

void gen_mph(const dispatch_t& dispatch, size_t arity, std::ostream& f)
{
  hashform form = {
    hashform::NORMAL_HM,
    hashform::STRING_HT,
    hashform::MINIMAL_HP,
    hashform::SLOW_HS
  };

  reroot* textroot = remkroot(arity * sizeof(size_t));
  reroot* keyroot  = remkroot(sizeof(key));

  key* keys; ub4 nkeys;

  getkeys(&keys, &nkeys, keyroot, textroot, &form, dispatch, arity);

  driver(&form, keys, nkeys, (void*)(std::ostream*)&f);
}

#include <stdarg.h>

void ostream_printf(void* f, const char* fmt, ...)
{
  std::ostream* os = (std::ostream*)f;

  va_list lst;
  va_start(lst, fmt);

  constexpr size_t sz = 1024;
  char buffer[sz];
  int count = vsnprintf(buffer, sz, fmt, lst);

  if(count > sz)
  {
    char* buf2 = new char[ count ];
    vsnprintf(buffer, sz, fmt, lst);
    os->write(buf2, count);
    delete[] buf2;
  }
  else
    os->write(buffer, count);

  va_end(lst);
}

#endif
