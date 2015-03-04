#include <stdlib.h>
#include <algorithm>
#include "lcp-quicksort.h"

typedef int Lcp;

inline int lcpstrcmp( const char * const s, const char * const t, Lcp &rlcp ) {
  long res;
  long lcp = rlcp;
  __asm__ __volatile__
    (
     "sub        $16, %[lcp]                    \n" 
     "1:\n"
     "add        $16, %[lcp]                     \n"
     "0:\n"
     "movdqu        (%[s],%[lcp]), %%xmm0 \n"    
     "pcmpistri    $0x18, (%[t],%[lcp]), %%xmm0  \n"    //  EQUAL_EACH(0x08) + NEGATIVE_POLARITY(0x10)                                             
     "ja 1b                                \n"
     "jnc 2f                                 \n"
     "add %%rcx, %[lcp] \n"
     "movzbq (%[t],%[lcp]), %[res]     \n"
     "movzbq (%[s],%[lcp]), %%rcx      \n"
     "sub %%rcx, %[res] \n"
     "jmp 3f                              \n"    
     "2:\n"
     "pxor %%xmm1, %%xmm1 \n"  // find the \0                                                                                                      
     "pcmpistri    $0x08, (%[t],%[lcp]), %%xmm1  \n"   // each + positive polarity                                                                 
     "add %%rcx, %[lcp] \n"
     "xor %[res], %[res]                     \n"
     "3:\n"
     : [lcp] "+r"(lcp), [res] "=&r" (res)
     : [s] "r"(s), [t] "r"(t)
     : "xmm0","xmm1", "rcx" );
  rlcp = (Lcp) lcp;
  return (int) res;
}

inline void exch( char const *strings[], Lcp lcps[], int I, int J) { 
  std::swap(strings[I],strings[J]);
  std::swap(lcps[I],lcps[J]);
}

void strsort(char const * strings[], Lcp lcps[], int lo, int hi );

template <bool ascending>
void lcpsort( char const * strings[], Lcp lcps[], int lo, int hi ) {
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

void strsort(char const * strings[], Lcp lcps[], int lo, int hi )
{
  if ( hi <= lo ) return;
  int lt = lo, gt = hi;

  char const * const pivotStr = strings[lo];
  for( int i = lo + 1; i <= gt; )
    {
      int cmpr = lcpstrcmp( pivotStr, strings[i], lcps[i] );
      if      (cmpr < 0) exch( strings, lcps, lt++, i++);
      else if (cmpr > 0) exch( strings, lcps, i, gt--);
      else            i++;
    }

  lcpsort<true> ( strings, lcps, lo, lt-1 );
  lcpsort<false>( strings, lcps, gt+1, hi );  
};

extern "C" void stringsort( char const * strings[], int n ) {
  Lcp *lcps = (Lcp *) calloc( n, sizeof(Lcp)); 
  strsort( strings, lcps, 0, n-1 );
  free(lcps);
}
