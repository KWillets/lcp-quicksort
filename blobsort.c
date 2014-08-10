#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
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

// read entire file as blob
char *readitems( char *fname, int *pn ) {
    int n=0;
    char *a = NULL;
    struct stat filestat;
    stat(fname, &filestat);
    FILE * fd = fopen(fname, "r");
    if( fd == NULL ) 
      fprintf(stderr, "fopen %s failed\n", fname);
    else {
      a=malloc(filestat.st_size+1);
      fread( a, filestat.st_size, 1, fd );
      a[filestat.st_size]=0;
    }

    *pn = filestat.st_size;
    return a;
}

int main(int argc, char **argv) {

  if( argc >1 ) {
    int n=0;
    char *s = readitems( argv[1], &n );

    Item *a = calloc( n, sizeof(Item));
    for( int i = 0; i < n; i++ )
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
