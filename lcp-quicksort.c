#include <stdlib.h>
#include <stdio.h>
#include "lcp-quicksort.h"

int lcpstrcmp( char *p, char *q, Lcp *lcp) {
  char *pi=p+*lcp, *qi=q+*lcp;
  while( *pi && !(*qi-*pi) ) {
    pi++;
    qi++;
  }
  *lcp = pi-p;
  return *qi-*pi;
}

void  exch( Item a[], Lcp lcp[], int I, int J) 
{ //printf( "exch %d %d (%s %s)\n", I, J, a[I], a[J] );
  Item  __tmp = a[I]; a[I]=a[J]; a[J]=__tmp;
  Lcp  __itmp = lcp[I]; lcp[I]=lcp[J], lcp[J]=__itmp; } 

int med3func(Lcp va, Lcp vb, Lcp vc)
{  
  if (va == vb)
    return va;
  if (vc == va || vc == vb)
    return vc;  
  return va < vb ?
    (vb < vc ? vb : (va < vc ? vc : va ) )
    : (vb > vc ? vb : (va < vc ? va : vc ) );
}

void lcpinsertionsort( Item a[], Lcp lcp[], int lo, int hi, int direction) {
  int i,j;

  if( direction > 0 )
    for( i = lo+1; i <= hi; i++ ) 
      for( j= i; j > lo && lcp[j] > lcp[j-1]; j-- )
	exch(a, lcp, j, j-1);
  else
    for( i = lo+1; i <= hi; i++ ) 
      for( j= i; j > lo && lcp[j] < lcp[j-1]; j-- )
	exch(a, lcp, j, j-1);

  // sort runs of identical lcp
  for( i = lo+1; i <= hi; i++ ) 
    ;
  for(  ; j > lo && lcp[j] == lcp[j-1] && lcpstrcmp( a[j-1], a[j], lcp+j) < 0; j-- )
    exch(a, lcp, j, j-1);	

}

void lcpquicksort( Item a[], Lcp lcp[], int lo, int hi, int direction ) {
  if (hi <= lo) return;
  int lt = lo, gt = hi;
  int n = hi - lo + 1;
  int v;
  v = n > 1000  ? med3func( lcp[lo], lcp[lo+n/2], lcp[hi] ) :
    lcp[lo];

  int i = lo + 1;
  if( direction < 0 ) {  // direction of old pivot determines effect of lcp > or < pivot
    while (i <= gt) {
      int t = lcp[i];
      if      (t < v) exch(a, lcp, lt++, i++);
      else if (t > v) exch(a, lcp, i, gt--);
      else            i++;
    }
  } else {
    while (i <= gt) { // old pivot was < a[lo:hi]
      int t = lcp[i];
      if      (t > v) exch(a, lcp, lt++, i++); // longer prefix ==> precedes pivot
      else if (t < v) exch(a, lcp, i, gt--);
      else            i++;
    }
  }
  // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi]
  lcpquicksort( a, lcp, lo, lt-1, direction );
  lcpquicksort( a, lcp, gt+1, hi, direction );

  // now pivot middle by char comparisons
  lo = lt; hi = gt;
  Item vv = a[lo];
  i = lo+1;
  while( i <= gt )
    {
      int cmpr = lcpstrcmp( a[i], vv, lcp+i );
      if      (cmpr > 0) exch(a, lcp, lt++, i++);
      else if (cmpr < 0) exch(a, lcp, i, gt--);
      else            i++;
    }
  lcpquicksort( a, lcp, lo, lt-1, -1 );
  lcpquicksort( a, lcp, gt+1, hi,  1 );
}

void stringsort( Item a[], int n ) {
  Lcp *lcp = calloc( n, sizeof(Lcp));
  lcpquicksort( a, lcp, 0, n-1, 1 );
  free(lcp);
}
