#include <stdio.h>
#include <stdlib.h>
typedef char * Item;

int lcpstrcmp( char *p, char *q, int *lcp) {
  int i;
  for( i = *lcp;  p[i] && p[i] == q[i]; i++ ) 
    ;
  *lcp = i;
  return q[i]-p[i]; // >0 ==> q > p
}

void  exch( Item a[], int lcp[], int I, int J) 
{ Item  __tmp = a[I]; a[I]=a[J]; a[J]=__tmp;
  int  __itmp = lcp[I]; lcp[I]=lcp[J], lcp[J]=__itmp; } 

// check lcp's are partitioned in opposite order of direction
// longer lcp, positive direction => lt
// etc.
void check_partition( int lcp[], int lo, int hi, int lt, int gt, int direction, int v ) {
  // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi]
  int i;
  for(  i = lo; i <= hi; i++ ) {
    if( i < lt && -direction*lcp[i] >= -direction*v ) {
      printf("smaller elt %d  is >= v\n", i);
      break;
    } else if( i >= lt && i <= gt && lcp[i] != v ) {
      printf("equal elt %d is != v\n", i);
      break;
    } else if( i > gt && -direction*lcp[i] <= -direction*v ) {
      printf("larger elt %d is <= v\n",i);
      break;
    }
  }
  if( i <= hi ) {
    printf("i = %d, lcp[i] = %d, lo = %d, lt = %d, gt = %d, hi = %d\n", i, lcp[i], lo, lt, gt, hi );
    for( i=lo; printf(","), i<=hi; i++ ) {
      printf("%d",lcp[i]);
    }
    printf("\n");
  }
}

void lcpquicksort( Item a[], int lcp[], int lo, int hi, int direction ) {
  if (hi <= lo) return;
  int lt = lo, gt = hi;
  int v = lcp[lo];
  int i = lo + 1;
  while (i <= gt)
    {
      int t = lcp[i];
      if( direction < 0 ) {  // direction of old pivot determines effect of lcp > or < pivot
	if      (t < v) exch(a, lcp, lt++, i++);
	else if (t > v) exch(a, lcp, i, gt--);
	else            i++;
      } else { // old pivot was < a[lo:hi]
	if      (t > v) exch(a, lcp, lt++, i++); // longer prefix ==> precedes pivot
	else if (t < v) exch(a, lcp, i, gt--);
	else            i++;
      }
    }
  // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi]
  check_partition( lcp, lo, hi, lt, gt, direction, v );
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
  int *lcp = calloc( n, sizeof(int));
  lcpquicksort( a, lcp, 0, n-1, 1 );
  free(lcp);
}

