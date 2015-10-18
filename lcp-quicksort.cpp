#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef int Lcp;

inline void lcpstrcmp( char * p, char * q, Lcp &i) {
  for( ; q[i] == p[i] && q[i]; i++ )
    ;
  //  return q[i]-p[i];
  return;
}

inline void exch( char *strings[], Lcp lcps[], int I, int J) { 
  std::swap(strings[I],strings[J]);
  std::swap(lcps[I],lcps[J]);
}

void strsort(char * strings[], Lcp lcps[], int lo, int hi );

template <bool ascending>
void lcpsort( char * strings[], Lcp lcps[], int lo, int hi ) {
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

void strsort(char * strings[], Lcp lcps[], int lo, int hi )
{
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  char * pivotStr = strings[lo];
  for( int i = lo + 1; i <= gt; )
    {
      lcpstrcmp( pivotStr, strings[i], lcps[i] ); // scan lcp only
      if      (pivotStr[lcps[i]] > strings[i][lcps[i]] ) exch( strings, lcps, lt++, i++);
      else if (pivotStr[lcps[i]] < strings[i][lcps[i]]) exch( strings, lcps, i, gt--);
      else            i++;
    }

  lcpsort<true> ( strings, lcps, lo, lt-1 );
  lcpsort<false>( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( char* strings[], int n ) {
  Lcp *lcps = (Lcp *) calloc( n, sizeof(Lcp)); 
  strsort( strings, lcps, 0, n-1 );
  int totlcp = 0;
  for( int i = 0; i < n; i++ )
    totlcp += lcps[i];

                   fprintf(stderr, "avg lcp=%f\n", (totlcp*1.0)/n );
  free(lcps);
}
