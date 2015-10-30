
#include <stdlib.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef long Lcp;

inline void lcpstrcmp(  unsigned char *s, unsigned char *t, Lcp &rlcp ) {
  long lcp = rlcp;
  __asm__ __volatile__
    (
     "sub        $16,   %[lcp]                    \n"
     "1:                                          \n"
     "add        $16,   %[lcp]                    \n"
     "movdqu     (%[s], %[lcp]), %%xmm0           \n"    
     "pcmpistri  $0x18, (%[t],%[lcp]), %%xmm0     \n"    //  EQUAL_EACH(0x08) + NEGATIVE_POLARITY(0x10)
     "ja 1b                                       \n"
     "and        $0xF,  %%rcx                     \n"  // rcx == 16 means str == and null found
     "add        %%rcx, %[lcp]                    \n"
     : [lcp] "+r"(lcp)  
     : [s] "r"(s), [t] "r"(t)
     : "xmm0", "rcx" );
  rlcp = (Lcp) lcp;
  return ;
}

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

void qexch( unsigned char **s, Lcp *l, int i, int dst, unsigned char *stmp, Lcp ltmp ) {
  s[i]=s[dst];
  l[i]=l[dst];
  s[dst]=stmp;
  l[dst]=ltmp;
}

void strsort(unsigned char * strings[], Lcp lcps[], int lo, int hi ) {
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  unsigned char * pivotStr = strings[lo];
  long lcp=lcps[lo];

  for( int i = lo + 1; i <= gt; ) {
      Lcp j = lcp;
      unsigned char *s = strings[i];
      lcpstrcmp( pivotStr, s, j );
      if      ( s[j] < pivotStr[j] ) qexch( strings, lcps, i++, lt++, s, j);
      else if ( s[j] > pivotStr[j] ) qexch( strings, lcps, i, gt--, s, j);
      else            i++;
    }

  lcpsort<true> ( strings, lcps, lo, lt-1 );
  lcpsort<false>( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( unsigned char  * strings[], int n ) {
  Lcp *lcps = (Lcp *) calloc( n, sizeof(Lcp)); 
  strsort( strings, lcps, 0, n-1 );
  free(lcps);
}
