typedef int Lcp;

typedef struct _Item {
  char *str;
  Lcp lcp;
} Item;


void lcpquicksort(Item a[], int l, int r, int direction);
void stringsort( Item a[], int n );
//void lcpinsertionsort( Item a[], Lcp lcp[], int lo, int hi, int direction);
