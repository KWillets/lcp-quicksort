#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include "lcp-quicksort.h"

typedef long int Tcache;

// branchless abs 
Tcache my_abs(Tcache a) {
  Tcache mask = (a >> (sizeof(Tcache) * 8 - 1));
  return (a + mask) ^ mask;
}

#define LCP_BITS  16
#define STR_BYTES (sizeof(Tcache)-LCP_BITS/8)
#define SIGN_BIT ( ((Tcache)1)<<( 8*sizeof(Tcache) - 1 ))

#define c_to_lcp(c) ((c<<1) >> (8*sizeof(Tcache)-LCP_BITS+1))
#define lcp_to_c(c) (((c) << (8*sizeof(Tcache)-LCP_BITS))&~SIGN_BIT)

#define c_to_char(c,ii) ((unsigned char)(0xFF & (c >> (8*(STR_BYTES-ii-1)))))
#define char_to_c(c, ii) (((Tcache)c) << 8*(STR_BYTES-ii-1))

#define nonterminal(c) ((Tcache) 0xFF & c)


Tcache lcpstrcmp( unsigned char  *  p, unsigned char  *  q, int i) {

  for( ; q[i] == p[i] && q[i]; i++ )
    ;

  Tcache d;
  if( q[i] == p[i] )
    return 0;
  else if( q[i] < p[i] )
    d =   SIGN_BIT | lcp_to_c( i+STR_BYTES );
  else
    d = -(SIGN_BIT | lcp_to_c( i+STR_BYTES ));

  Tcache m=0;
  for( int j = 0; j < STR_BYTES; j++ )  // unroll me please
    {
      if( !q[i+j] )
	break;
      m |= char_to_c( q[i+j], j );
    }

    return d|m;
}

inline void exch( unsigned char  *strings[], Tcache lcps[], int I, int J) { 
  std::swap(strings[I],strings[J]);
  std::swap(lcps[I],lcps[J]);
}

void strsort(unsigned char  * strings[], Tcache lcps[], int lo, int hi );

void lcpsort(unsigned char  * strings[], Tcache lcps[], int lo, int hi ) {
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  Tcache pivot = lcps[lo];
  for( int i = lo + 1; i <= gt; ) {
    if      ( lcps[i] > pivot ) exch( strings, lcps, i, gt--);
    else if ( lcps[i] < pivot ) exch( strings, lcps, lt++, i++);
    else            i++;
  }

  if( nonterminal( pivot ))
    strsort( strings, lcps, lt, gt );
  lcpsort( strings, lcps, lo, lt-1 );
  lcpsort( strings, lcps, gt+1, hi );
};

void strsort(unsigned char  * strings[], Tcache lcps[], int lo, int hi )
{
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  unsigned char  *  pivotStr = strings[lo];
  int d = my_abs(c_to_lcp(lcps[lo]));
  for( int i = lo + 1; i <= gt; )
    {
      lcps[i] = lcpstrcmp( pivotStr, strings[i], d );
      if      (lcps[i] < 0) exch( strings, lcps, lt++, i++);
      else if (lcps[i] > 0) exch( strings, lcps, i, gt--);
      else            i++;
    }

  lcpsort( strings, lcps, lo, lt-1 );
  lcpsort( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( unsigned char  * strings[], int n ) {
  Tcache *lcps = (Tcache *) calloc( n, sizeof(Tcache)); 
  strsort( strings, lcps, 0, n-1 );
  free(lcps);
}
