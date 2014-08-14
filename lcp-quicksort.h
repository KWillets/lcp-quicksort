
typedef char * Item;
typedef unsigned int Lcp;

void lcpquicksort(Item a[], Lcp lcp[], int l, int r, int direction);
void stringsort( Item a[], int n );
void lcpinsertionsort( Item a[], Lcp lcp[], int lo, int hi, int direction);
