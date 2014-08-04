#include <stdio.h>
#include "lcp-quicksort.h"

void dumpitems( Item s[], int n) {
  for(int i=0; i < n; i++ ) {
    printf( "%s\n", s[i] );
  };
  printf("----\n\n");
}

int main(int argc, char **argv) {
  /*
  Item s[]={"aab","aaa"};
  stringsort(s,2);
  dumpitems(s, 2);

  Item t[]= {"aaz","aay","aaa","aab"};
  stringsort(t,4);
  dumpitems(t,4);

  Item t2[]= {"aaa","aaa","aaa","aaa"};
  stringsort(t2,4);
  dumpitems(t2,4);
  */
  Item t3[]= {"aaa","aaa","aaa","aaa","bbb","ccc","ddd","eee"};
  stringsort(t3,8);
  dumpitems(t3,8);

  Item t4[]= {"a","b","c","d","e","f","g","h"};
  stringsort(t4,8);
  dumpitems(t4,8);


}
