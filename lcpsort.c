#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n) {
  int i;
  for( i=0; i < n; i++ ) {
    printf( "%s", s[i] );
  };
  printf("\n----\n\n");
}

int main(int argc, char **argv) {

  if( argc >1 ) {
    FILE * fd = fopen(argv[1], "r");
    if( fd == NULL )
      printf("fopen %s failed\n", argv[1]);
    Item a[100000];
    int n=0;
    char buf[1000];
    while( fgets(buf, 1000, fd) ) {
      a[n++] = strdup(buf);
    }
    printf("n=%d\n", n);
    stringsort(a,n);
    dumpitems(a,n);
  }
  else printf("usage: %s <filename>\n", argv[0]);
}
