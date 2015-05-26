#include <stdlib.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef int Lcp;
typedef unsigned int Tcache;

typedef struct {
  Lcp lcp;
  Tcache cache;
} StrInfo;

inline int lcpstrcmp( char const * const p, char const * const q, Lcp &i) {
  for( ; !(q[i] - p[i]) && p[i]; i++ )
    ;
  return q[i]-p[i];
}

inline void exch( char const *strings[], StrInfo lcps[], int I, int J) { 
  std::swap(strings[I],strings[J]);
  std::swap(lcps[I],lcps[J]);
}

void strsort(char const * strings[], StrInfo lcps[], int lo, int hi );

template <bool ascending>
void lcpsort( char const * strings[], StrInfo lcps[], int lo, int hi ) {
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  Lcp pivot = lcps[lo].lcp;
  for( int i = lo + 1; i <= gt; ) {
    if      ( ascending ? lcps[i].lcp > pivot : lcps[i].lcp < pivot ) exch( strings, lcps, i, gt--);
    else if ( ascending ? lcps[i].lcp < pivot : lcps[i].lcp > pivot ) exch( strings, lcps, lt++, i++);
    else            i++;
  }

  strsort<false>( strings, lcps, lt, gt );
  lcpsort<ascending>( strings, lcps, lo, lt-1 );
  lcpsort<ascending>( strings, lcps, gt+1, hi );
};

template<bool cacheDirty>
void strsort(char const * strings[], StrInfo lcps[], int lo, int hi )
{
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  char const * const pivotStr = strings[lo];
  StrInfo pInfo = lcps[lo];
  for( int i = lo + 1; i <= gt; )
    {
      int cmpr;
      if( cacheDirty || lcps[i].cache == pInfo.cache ) {  // 0 ok?
	// char comp + extend cache
	lcps[i].lcp += cacheDirty ? 0 : sizeof(Tcache);
	cmpr = lcpstrcmp( pivotStr, strings[i], lcps[i].lcp ); 
	// set new cache
	Tcache c = 0;
	for( int j = 0; j < sizeof( Tcache ) && strings[i][j]; j++ ) {
	  c |= strings[i][j] << (sizeof( Tcache )-j-1)*8;
	}
	lcps[i].cache = c;
      } else
	cmpr = lcps[i].cache > pInfo.cache ? 1 : -1; 


      if      (cmpr < 0) exch( strings, lcps, lt++, i++);
      else if (cmpr > 0) exch( strings, lcps, i, gt--);
      else            i++;
    }

  lcpsort<true> ( strings, lcps, lo, lt-1 );
  lcpsort<false>( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( char const * strings[], int n ) {
  StrInfo *lcps = (StrInfo *) calloc( n, sizeof(StrInfo)); 
  strsort<true>( strings, lcps, 0, n-1 );
  free(lcps);
}
