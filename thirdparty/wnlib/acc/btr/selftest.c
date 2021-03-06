/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Will Naylor

****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "wnlib.h"
#include "wnmem.h"
#include "wnasrt.h"
#include "wnrndd.h"

#include "wnbtr.h"
#include "wnbtrl.h"



#define SIZE  100000


local int lo_old_test(void)
{
  wn_btree tree;
  wn_bhandle handle;
  long num;
  int i, j,count,index,index2;
  
  wn_gpmake("general_free");

  wn_mkintbtree(&tree);
  wn_bverify(tree);

  for(i=0;i<SIZE;i++)
  {
    j = (i * 17) % SIZE;

    /*
    num = wn_random_int_between(0,1000000);
    num = 1;
    */
    num = j;
    wn_bins(&handle,tree,(ptr)num);
    handle->contents = (ptr)(num+1);
  }

  wn_bverify(tree);

  for(i=0;i<SIZE;i++)
  {
    num = i;
    wn_bget(&handle,tree,(ptr)num,WN_B_EQ);
    wn_assert((long)(handle->contents) == num+1);
  }

  for(i=0;i<SIZE;i++)
  {
    count = wn_bcount(tree);
    wn_assert(count > 0);

    index = wn_random_int_between(0,count);
    wn_bget_handle_of_index(&handle,tree,index);
    wn_bget_index_of_handle(&index2,tree,handle);
    wn_assert(index == index2);
    wn_bdel(handle,tree);
  }

  wn_bverify(tree);

  count = wn_bcount(tree);
  wn_assert(count == 0);

  wn_bverify(tree);

  wn_gpfree();

  return(0);
}


#define LO_MIN_KEY 123
#define LO_MAX_KEY 12345

#define LO_MIN_SECONDARY_KEY 201
#define LO_MAX_SECONDARY_KEY 257

local int lo_scores[LO_MAX_KEY+1];

local void lo_test_act(wn_bhandle handle)
{
  ++ lo_scores[(long) handle->key];
}


local void lo_test_main(void)
{
  wn_btree tree;
  wn_bhandle handle;
  int i, j;

  wn_gpmake("general_free");

  wn_mkintbtree(&tree);

  for (i = 0;  i <= LO_MAX_KEY;  ++i)
  {
    j = (i * 7) % (LO_MAX_KEY + 1);
    if (j >= LO_MIN_KEY  &&  j <= LO_MAX_KEY)
    {
      wn_bins(&handle, tree, (ptr) j);
      handle->contents = (ptr) (j + 2);
    }
  }

  wn_bverify(tree);

  wn_bget(&handle, tree, (ptr) 0, WN_B_MIN);
  wn_assert(handle->key == (ptr) LO_MIN_KEY);

  wn_bget(&handle, tree, (ptr) 0, WN_B_MAX);
  wn_assert(handle->key == (ptr) LO_MAX_KEY);

  wn_bget(&handle, tree, (ptr) (LO_MIN_KEY - 5), WN_B_LT);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) (LO_MIN_KEY - 5), WN_B_LE);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) (LO_MIN_KEY - 5), WN_B_EQ);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) (LO_MIN_KEY - 5), WN_B_GE);
  wn_assert(LO_MIN_KEY == (long) handle->key);

  wn_bget(&handle, tree, (ptr) (LO_MIN_KEY - 5), WN_B_GT);
  wn_assert(LO_MIN_KEY == (long) handle->key);

  wn_bget(&handle, tree, (ptr) LO_MIN_KEY, WN_B_LT);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) LO_MAX_KEY, WN_B_GT);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) (LO_MAX_KEY + 5), WN_B_LT);
  wn_assert(LO_MAX_KEY == (long) handle->key);

  wn_bget(&handle, tree, (ptr) (LO_MAX_KEY + 5), WN_B_LE);
  wn_assert(LO_MAX_KEY == (long) handle->key);

  wn_bget(&handle, tree, (ptr) (LO_MAX_KEY + 5), WN_B_GE);
  wn_assert(!handle);

  wn_bget(&handle, tree, (ptr) (LO_MAX_KEY + 5), WN_B_GT);
  wn_assert(!handle);

  for (i = LO_MIN_KEY;  i <= LO_MAX_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);
    wn_assert((ptr) i == handle->key);

    wn_bget(&handle, tree, (ptr) i, WN_B_LE);
    wn_assert((ptr) i == handle->key);

    wn_bget(&handle, tree, (ptr) i, WN_B_GE);
    wn_assert((ptr) i == handle->key);
  }

  for (i = LO_MIN_KEY+1;  i <= LO_MAX_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_LT);
    wn_assert((ptr) (i-1) == handle->key);
  }

  for (i = LO_MIN_KEY;  i <= LO_MAX_KEY-1;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_GT);
    wn_assert((ptr) (i+1) == handle->key);
  }

  for (i = 0;  i <= LO_MAX_SECONDARY_KEY;  ++i)
  {
    j = (i * 11) % (LO_MAX_SECONDARY_KEY + 1);
    if (j >= LO_MIN_SECONDARY_KEY  &&  j <= LO_MAX_SECONDARY_KEY)
    {
      wn_bins(&handle, tree, (ptr) j);
      handle->contents = (ptr) (j+4);
    }
  }

  for (i = 0;  i <= LO_MAX_KEY;  ++i)
  {
    lo_scores[i] = 0;
  }
  wn_bact(tree, lo_test_act, (ptr) LO_MIN_KEY, WN_B_GE,
  /**/			     (ptr) LO_MAX_KEY, WN_B_LE);

  for (i = 0;  i < LO_MIN_KEY;  ++i)
  {
    wn_assert(0 == lo_scores[0]);
  }
  for (i = LO_MIN_KEY;  i < LO_MIN_SECONDARY_KEY;  ++i)
  {
    wn_assert(1 == lo_scores[i]);
  }
  for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
  {
    wn_assert(2 == lo_scores[i]);
  }
  for (i = LO_MAX_SECONDARY_KEY+1;  i <= LO_MAX_KEY;  ++i)
  {
    wn_assert(1 == lo_scores[i]);
  }

  for (i = LO_MIN_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);

    wn_bdel(handle, tree);
  }

  for (i = LO_MIN_KEY;  i < LO_MIN_SECONDARY_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);

    wn_assert(!handle);
  }
  for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);

    wn_assert((ptr) i == handle->key);
  }

  for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);

    wn_bmove(handle, tree, (ptr) -i);
  }


  for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
  {
    wn_bget(&handle, tree, (ptr) i, WN_B_EQ);

    wn_assert(!handle);

    wn_bget(&handle, tree, (ptr) -i, WN_B_EQ);

    wn_assert((ptr) -i == handle->key);
  }

  for (i = 0;  i <= LO_MAX_SECONDARY_KEY - LO_MIN_SECONDARY_KEY;  ++i)
  {
    wn_bget_handle_of_index(&handle, tree, i);

    wn_assert(handle->key == (ptr) (i - LO_MAX_SECONDARY_KEY));

    wn_bget_index_of_handle(&j, tree, handle);

    wn_assert(i == j);
  }

  wn_assert(LO_MAX_KEY + 1 - LO_MIN_SECONDARY_KEY == wn_bcount(tree));

  wn_bverify(tree);

  wn_freebtree(tree);

  wn_gpfree();
} /* lo_test_main */


local void lo_test_for_btree(void) {
  wn_btree btr;
  wn_bhandle handle;
  long i, sum;

  wn_mkintbtree(&btr);

  for (i = 0;  i < 100;  ++i) {
    wn_bins(&handle, btr, (ptr) i);
    handle->contents = (ptr) (i*i);
  }

  i = 0;
  WN_FOR_BTREE(long, v, btr, long, k)
    wn_assert(k == i);
    wn_assert(v == i*i);

    ++i;
  WN_END_FOR_BTREE()
  wn_assert(100 == i);

  i = 99;
  WN_FOR_BTREE_REVERSE(long, v, btr, long, k)
    wn_assert(k == i);
    wn_assert(v == i*i);

    --i;
  WN_END_FOR_BTREE_REVERSE()
  wn_assert(-1 == i);

  sum = 0;
  WN_FOR_BTREE(long, v, btr, long, k)
    sum += k;

    if (4 == k) break;
  WN_END_FOR_BTREE()
  wn_assert(10 == sum);

  sum = 0;
  WN_FOR_BTREE_REVERSE(long, v, btr, long, k)
    sum += k;

    if (97 == k) break;
  WN_END_FOR_BTREE_REVERSE()
  wn_assert(99 + 98 + 97  ==  sum);
} /* lo_test_for_btree */


int main(void)
{

  printf("testing btr...\n");

  lo_old_test();

  lo_test_main();
  lo_test_for_btree();

  printf("  ok!!!!!!\n");

  return 0;
} /* main */
