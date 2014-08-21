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

void  exch( Item a[], Lcp lcp[], int I, int J) 
{ //printf( "exch %d %d (%s %s)\n", I, J, a[I], a[J] );
  Item  __tmp = a[I]; a[I]=a[J]; a[J]=__tmp;
  Lcp  __itmp = lcp[I]; lcp[I]=lcp[J], lcp[J]=__itmp; } 

int med3func( Lcp l[], int a, int b, int c)
{  
  if (l[a] == l[b])
    return a;
  if (l[c] == l[a] || l[c] == l[b])
    return c;  
  return l[a] < l[b] ?
    (l[b] < l[c] ? b : (l[a] < l[c] ? c : a ) )
    : (l[b] > l[c] ? b : (l[a] < l[c] ? a : c ) );
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
  int r=hi, lcpr = lcp[hi];  // start from right
  for( i = hi-1; i >= lo; i-- ) 
    if( lcp[i] == lcpr ) {
      int cmpr = lcpstrcmp( a[i], a[i+1], lcp+i );
      if( cmpr < 0 ) {
	for( j = i; j < r && lcp[j] < lcp[j+1] ; j++ )
	  {
            if((lcp[j] == lcp[j+1] && lcpstrcmp( a[j],a[j+1], lcp+j)) >= 0) {  // if same lcp do compare
              // if j <= j+1 swap result lcp to j+1
              Lcp t = lcp[j];
              lcp[j]=lcp[j+1];
              lcp[j+1]=t;
              break;
            }
            exch(a, lcp, j, j+1); 
	  }
      }
    } else 
      lcpr = lcp[r = i];

}

void lcpquicksort( Item a[], Lcp lcp[], int lo, int hi, int direction ) {
  if (hi <= lo) return;
  int lt = lo, gt = hi;
  int n = hi - lo + 1;
  
  int v;
  if( n > 1000 ) exch( a, lcp, lo,  med3func( lcp, lo, lo+n/2, hi ));

  v=lcp[lo];
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
