
#include <stdlib.h>
#include <stdio.h>
// three-way partitioning
// from sedgewick
// modified to bin lcp's
// 2 stages
// first bin by lcp <,==,> lcp(pivot)
// then bin == by <,==,> pivot
typedef char * Item;

int lcpstrcmp( char *p, char *q, int *lcp) {
  
  for( int i = *lcp;  p[i] && p[i] == q[i]; i++ ) 
    ;
  *lcp = i;
  return q[i]-p[i]; // >0 ==> q > p
}

#define exch(A,B) { typeof(A) _tmp=A; A=B; B=_tmp; }

void lcpquicksort(Item a[], int lcp[], int l, int r, int direction)
{ int i = l-1, j = r, p = l-1, q = r; Item v = a[r]; int vlcp = lcp[r];
  if (r <= l) return;
  for (;;)
    { 
      if( direction > 0 ) {
        while (lcp[++i] > vlcp ) ;
        while (vlcp > lcp[--j]) if (j == l) break;
      } else {
        while (lcp[++i] < vlcp ) ;
        while (vlcp < lcp[--j]) if (j == l) break;
      }
      if (i >= j) break;
      exch(a[i], a[j]);exch(lcp[i],lcp[j]);
      if (lcp[i] == vlcp) { p++; exch(a[p], a[i]); exch(lcp[p], lcp[i]); }
      if (vlcp == lcp[j]) { q--; exch(a[j], a[q]); exch(lcp[j], lcp[q]); }
    } 
  exch(a[i], a[r]); exch(lcp[i],lcp[r]); j = i-1; i = i+1;
  for (k = l; k < p; k++, j--) { exch(a[k], a[j]); exch(lcp[k],lcp[j]); }
  for (k = r-1; k > q; k--, i++) { exch(a[i], a[k]); exch(lcp[i], lcp[k]); }

  lcpquicksort(a, lcp, l, j, direction);
  lcpquicksort(a, lcp, i, r, direction);

  // comparison-based sort of middle
  // v is at i ?
  l=j+1;r=i-1;
  i = l, j = r-1, p = l-1, q = r;  
  for (;;)
    { 
      int ieq,jeq;
      while ( (ieq=lcpstrcmp(a[i], v, lcp+i)) > 0 ) i++ ;
      while ( (jeq=lcpstrcmp(v, a[j], lcp+j)) > 0 ) if (--j == l) break;
      if (i >= j) break;
      exch(a[i], a[j]);exch(lcp[i],lcp[j]);
      if (ieq == 0) { p++; exch(a[p], a[i]); exch(lcp[p], lcp[i]); }
      if (jeq == 0) { q--; exch(a[j], a[q]); exch(lcp[j], lcp[q]); }
    } 
  exch(a[i], a[r]); exch(lcp[i],lcp[r]); j = i-1; i = i+1;
  for (k = l; k < p; k++, j--) { exch(a[k], a[j]); exch(lcp[k],lcp[j]);}
  for (k = r-1; k > q; k--, i++) { exch(a[i], a[k]); exch(lcp[i], lcp[k]); }
  lcpquicksort(a, lcp, l, j, -1 );
  lcpquicksort(a, lcp, i, r, 1 );
}
