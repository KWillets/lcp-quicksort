#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n) {
  int i;
  for( i=0; i < n; i++ ) {
    printf( "%s", s[i] );
  };
  //  printf("\n----\n\n");
}

int main(int argc, char **argv) {

  if( argc >1 ) {
    FILE * fd = fopen(argv[1], "r");
    if( fd == NULL )
      printf("fopen %s failed\n", argv[1]);
    Item a[500000];
    int n=0;
    char buf[2000];
    while( fgets(buf, 2000, fd) ) {
      a[n++] = strdup(buf);
    }
    //printf("n=%d\n", n);
    int t=clock();
    stringsort(a,n);
    float secs = (clock()-t)*1.0/CLOCKS_PER_SEC;
    fprintf(stderr, "time=%f\n", secs );

    dumpitems(a,n);
  }
  else printf("usage: %s <filename>\n", argv[0]);
}
