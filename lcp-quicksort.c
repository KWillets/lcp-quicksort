#include <stdlib.h>
#include <stdio.h>
#include "lcp-quicksort.h"

int lcpstrcmp( char *p, char *q, Lcp *lcp) {
  int ret,i;
  for( i=*lcp; !(ret=q[i]-p[i]) && q[i]; i++ )
    ;
  *lcp = i;
  return ret;
}

void  exch( Item a[], int I, int J) { 
  Item  __tmp = a[I]; a[I]=a[J]; a[J]=__tmp;
}

int median3( Item l[], int a, int b, int c)
{  
  if (l[a].lcp == l[b].lcp)
    return a;
  if (l[c].lcp == l[a].lcp || l[c].lcp == l[b].lcp )
    return c;  
  return l[a].lcp < l[b].lcp ?
    (l[b].lcp < l[c].lcp ? b : (l[a].lcp < l[c].lcp ? c : a ) )
    : (l[b].lcp > l[c].lcp ? b : (l[a].lcp < l[c].lcp ? a : c ) );
}

void lcpquicksort( Item a[], int lo, int hi, int direction ) {
  if (hi <= lo) return;
  int lt = lo, gt = hi;
  int i, n = hi - lo + 1;
  if( n > 1000 ) exch( a, lo,  median3( a, lo, lo+n/2, hi ));
  int v=a[lo].lcp;
  if( direction < 0 ) {  // direction of old pivot determines effect of lcp > or < pivot
    for( i = lo + 1; i <= gt; ) {
      if      (a[i].lcp < v) exch(a, lt++, i++);
      else if (a[i].lcp > v) exch(a, i, gt--);
      else            i++;
      }
  } else {
    for( i = lo + 1; i <= gt; ) { // old pivot was < a[lo:hi]
      if      (a[i].lcp > v) exch(a, lt++, i++); // longer prefix ==> precedes pivot
      else if (a[i].lcp < v) exch(a, i, gt--);
      else            i++;
    }    
  }
  // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi]
  lcpquicksort( a, lo, lt-1, direction );
  lcpquicksort( a, gt+1, hi, direction );

  // now pivot middle by char comparisons
  lo = lt; hi = gt;
  char * vv = a[lo].str;
  for( i = lo+1; i <= gt; )
    {
      int cmpr = lcpstrcmp( a[i].str, vv, &a[i].lcp );
      if      (cmpr > 0) exch(a, lt++, i++);
      else if (cmpr < 0) exch(a, i, gt--);
      else            i++;
    }
  lcpquicksort( a, lo, lt-1, -1 );
  lcpquicksort( a, gt+1, hi,  1 );
}

void stringsort( Item a[], int n ) {
  //  Lcp *lcp = calloc( n, sizeof(Lcp));
  lcpquicksort( a, 0, n-1, 1 );
  //  free(lcp);
}
