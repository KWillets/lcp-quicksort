#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n, Lcp lcp[]) {
  int i;
  for( i=0; i < n; i++ ) {
    printf( "%s", s[i] );
  };
  //  printf("\n----\n\n");
}

Item *readitems( char *fname, int *pn ) {
    int n=0;
    Item *a = NULL;
    FILE * fd = fopen(fname, "r");
    if( fd == NULL ) 
      fprintf(stderr, "fopen %s failed\n", fname);
    else {
      int nalloc = 1<<15;
      a=calloc(sizeof(Item), nalloc);

      char buf[8096];
      while( fgets(buf, 8096, fd) ) {
	if( n >= nalloc ) {
	  nalloc <<=1;
	  a = realloc( a, sizeof(Item) * nalloc);
	}
	a[n++] = strdup(buf);
      }
    }
    *pn = n;
    return a;
}

int main(int argc, char **argv) {

  if( argc >1 ) {
    int n=0;
    Item *a = readitems( argv[1], &n );

    Lcp *lcp = calloc( n, sizeof(Lcp));

    int t=clock();

    lcpquicksort( a, lcp, 0, n-1, 1 );

    //    stringsort(a,n);
    float secs = (clock()-t)*1.0/CLOCKS_PER_SEC;
    fprintf(stderr, "time=%f\n", secs );

    //    dumpitems(a,n,lcp);
    exit(0);
  }
  else printf("usage: %s <filename>\n", argv[0]);
}
