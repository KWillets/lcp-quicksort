#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n, int lcp[]) {
  int i;
  for( i=0; i < n; i++ ) {
    printf( "%.20s\n", s[i] );
  };
  //  printf("\n----\n\n");
}

// read dbs file 
char *readitems( char *fname, int *pn ) {
    int n=0;
    char *a = NULL;
    FILE * fd = fopen(fname, "r");
    if( fd == NULL ) 
      fprintf(stderr, "fopen %s failed\n", fname);
    else {
      int nalloc = 1<<20;
      a=calloc(sizeof(char), nalloc);

      char buf[8096];
      fgets(buf, 8096, fd); // skip first line
      while( fgets(buf, 8096, fd) && buf[0] != '>' ) {
	int k = strlen(buf)-1; // skip \n
	if( n+k >= nalloc-1 ) {
	  nalloc <<=1;
	  a = realloc( a, sizeof(char) * nalloc);
	}
	strncpy( a+n, buf, k);
	n+= k;
      }
    }
    a[n]=0; // null-terminate
    fprintf(stderr, "%d bytes read\n", n );
    *pn = n;
    return a;
}

int main(int argc, char **argv) {

  if( argc >1 ) {
    int n=0;
    char *s = readitems( argv[1], &n );
    Item *a = calloc(sizeof(Item), n );
    for( int i =0; i < n; i++ )
      a[i] = s+i;

    int *lcp = calloc( n, sizeof(int));

    int t=clock();

    lcpquicksort( a, lcp, 0, n-1, 1 );

    //    stringsort(a,n);
    float secs = (clock()-t)*1.0/CLOCKS_PER_SEC;
    fprintf(stderr, "time=%f\n", secs );

    dumpitems(a,n,lcp);
  }
  else printf("usage: %s <filename>\n", argv[0]);
}
