#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef size_t Lcp;
typedef unsigned char StrChar;

Lcp strlcp( StrChar *p, StrChar *q, Lcp l )
{
  while( p[l] == q[l] && p[l] )
    l++;
  return l;
}

inline void exch( StrChar *strings[], Lcp lcps[], int I, int J ) {
  std::swap( strings[I], strings[J] );
  std::swap( lcps[I], lcps[J] );
}

void strsort( StrChar * strings[], Lcp lcps[], int lo, int hi, Lcp depth );

enum Order { ascending, descending };
template <Order order>
void lcpsort( StrChar * strings[], Lcp lcps[], int lo, int hi ) {
  int lt = lo, gt = hi;

  Lcp pivot = lcps[lo];

  for( int i = lo + 1; i <= gt; ) {
    if      ( order == ascending ? lcps[i] > pivot : lcps[i] < pivot ) exch( strings, lcps, i,    gt-- );
    else if ( order == ascending ? lcps[i] < pivot : lcps[i] > pivot ) exch( strings, lcps, i++,  lt++ );
    else    i++;
  }

  if( gt > lt )   strsort( strings, lcps, lt, gt, pivot );
  if( lt-1 > lo ) lcpsort<order>( strings, lcps, lo, lt-1 );
  if( hi > gt+1 ) lcpsort<order>( strings, lcps, gt+1, hi );
};

void strexch( StrChar ** strs, Lcp *lcps, int i, int dst, Lcp ltmp ) {
  lcps[i] = lcps[dst]; lcps[dst] = ltmp;
  std::swap( strs[i], strs[dst] );
}

void strsort( StrChar * strings[], Lcp lcps[], int lo, int hi, Lcp depth )
{
  int lt = lo, gt = hi;

  StrChar *pivot = strings[lo];

  for( int i = lo + 1; i <= gt; )
    {
      StrChar *q = strings[i];
      Lcp j = strlcp( pivot, q, depth );
      if      ( pivot[j] < q[j] ) strexch( strings, lcps, i,   gt--, j );
      else if ( pivot[j] > q[j] ) strexch( strings, lcps, i++, lt++, j );
      else    i++;
    }

  if( lt-1 > lo ) lcpsort<ascending> ( strings, lcps, lo, lt-1 );
  if( hi > gt+1 ) lcpsort<descending>( strings, lcps, gt+1, hi );
};

extern "C" void stringsort( StrChar* strings[], int n ) {
  Lcp *lcps = (Lcp *) calloc( n, sizeof(Lcp) );
  strsort( strings, lcps, 0, n-1, 0 );
  free( lcps );
}
