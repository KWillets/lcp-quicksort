#include <stdio.h>
#include <stdlib.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n) {
  int i;
  for( i=0; i < n; i++ ) {
    printf( "%s\n", s[i] );
  };
  printf("----\n\n");
}

int main(int argc, char **argv) {

  char * t[] = {"aaa", "aab", "aaa", "aba", "aaa", "aba", "aaa","aba"};
  int *lcp = calloc(sizeof(int), 8 );
  lcpinsertionsort( t, lcp, 0, 7, 1 );
  //stringsort( t, 8 );
  dumpitems(t,8);
  /*  
  char * u[] = {"bbb", "ccc", "aaa", "ddd", "aab", "aac", "aad", "ccd", "bbc", "bbz", "bbd","eee", "aad","aaz" };
  stringsort( u, 14 );
  dumpitems(u,14);


  Item s[]={"aab","aaa"};
  stringsort(s,2);
  dumpitems(s, 2);

  Item t[]= {"aaz","aay","aaa","aab"};
  stringsort(t,4);
  dumpitems(t,4);

  Item t2[]= {"aaa","aaa","aaa","aaa"};
  stringsort(t2,4);
  dumpitems(t2,4);

  Item t3[]= {"aaa","aaa","aaa","aaa","bbb","ccc","ddd","eee"};
  stringsort(t3,8);
  dumpitems(t3,8);

  Item t4[]= {"a","b","c","d","e","f","g","h"};
  stringsort(t4,8);
  dumpitems(t4,8);
  */


}
