#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef int Lcp;

inline void exch( unsigned char *strings[], Lcp lcps[], int I, int J) { 
  std::swap(strings[I],strings[J]);
  std::swap(lcps[I],lcps[J]);
}

void strsort(unsigned char * strings[], Lcp lcps[], int lo, int hi );

template <bool ascending>
void lcpsort( unsigned char * strings[], Lcp lcps[], int lo, int hi ) {
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  Lcp pivot = lcps[lo];
  for( int i = lo + 1; i <= gt; ) {
    if      ( ascending ? lcps[i] > pivot : lcps[i] < pivot ) exch( strings, lcps, i, gt--);
    else if ( ascending ? lcps[i] < pivot : lcps[i] > pivot ) exch( strings, lcps, lt++, i++);
    else            i++;
  }

  strsort( strings, lcps, lt, gt );
  lcpsort<ascending>( strings, lcps, lo, lt-1 );
  lcpsort<ascending>( strings, lcps, gt+1, hi );
};

void qexch( unsigned char ** s, Lcp *l, int i, int dst, unsigned char *stmp, Lcp ltmp ) {
  l[i]=l[dst];
  s[i]=s[dst];
  l[dst]=ltmp;
  s[dst]=stmp;
}

void strsort(unsigned char * strings[], Lcp lcps[], int lo, int hi )
{
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  unsigned char * p = strings[lo];
  Lcp j,k= lcps[lo];

  for( int i = lo + 1; i <= gt; )
    {
      unsigned char *q = strings[i];
      for( j = k; p[j] == q[j] && p[j] ; j++ )
	;
      if      (p[j] > q[j] ) qexch( strings, lcps, i++, lt++, q, j );
      else if (p[j] < q[j] ) qexch( strings, lcps, i,   gt--, q, j );
      else            i++;
    }

  lcpsort<true> ( strings, lcps, lo, lt-1 );
  lcpsort<false>( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( unsigned char* strings[], int n ) {
  Lcp *lcps = (Lcp *) calloc( n, sizeof(Lcp)); 
  strsort( strings, lcps, 0, n-1 );
  int totlcp = 0;
  //  for( int i = 0; i < n; i++ )
  //  totlcp += lcps[i];

  //                 fprintf(stderr, "avg lcp=%f\n", (totlcp*1.0)/n );
  free(lcps);
}
