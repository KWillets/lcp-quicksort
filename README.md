lcp-quicksort
=============

An string quicksort based on reuse of longest common prefix from each string comparison.

Recently a number of classic sorting algorithms have been modified to use lcp (longest common prefix) information during string comparisons.
The key observation is that if two strings A,B have already been compared to a third key C, 
and the lcp's from that comparison have been retained, then
the ordering of A and B can be determined by looking at lcp length, and then character comparisons:

Lemma 1 (from lcp merge sort paper https://www.jstage.jst.go.jp/article/ipsjdc/4/0/4_0_69/_article):  

If A > C and B > C, and lcp(A,C) > lcp(B,C), then A < B. Proof follows from the fact that the first character
after the lcp is the tie breaker for the comparison, and if lcp(A,C) > lcp(B,C), then B[lcp(B,C)] > C[lcp(B,C)] == A[lcp(B,C)], so B > A.

ASCII art version:

If we imagine the per-character strcmp results as follows (either = or >), we see why a longer lcp matters:

    C: XXXXXXXXXXX... (some chars)
    A: ========>
    B: ===>

The character point where B > C is also the point where B > A.

We can reverse > and < if needed (a property which I will call "direction"), if C > A,B.  Use of a pivot typically creates two ranges with opposite direction.

In quicksort, the obvious candidate for C is the pivot P_prev from a previous phase of the sort; every element in a recursive call to quicksort 
has been compared to some P\_prev in the preceding round (except the initial call, where P\_prev can be considered to be 
the empty string).  

Given the direction of P_prev and the lcp's of every element against it, we can 
determine the order of any element A against any new pivot P\_new by comparing lcp(A,P\_prev) and lcp(P\_new, P\_prev).  
Only if the lcp's are equal do we need to do further character comparisons to determine the order of A and P_new, so we aim our 
divide-and-conquer strategy first at splitting out lcp's which do not need any further character comparisons.

    P_prev:XXXXXXXXXXXX....
     a[lo]:========>  (all lcp's longer than P_new's)
     ...  :======>
     ...  :=========>
     a[lt]:=====>  (all lcp's equal to P_new's)
     ...  :=====>
     P_new:=====>
     .... :=====>
     a[gt]:=====>
     .... :=>  (all lcp's shorter than P_new's)
     ...  :==>
      ... :=>
     a[hi]:===>

LCP Quicksort therefore uses two phases:  After a new pivot P_new is selected, elements are partitioned by lcp's 
which are greater than, equal to, or less than P_new's.  In the second phase, the middle partition is split by actual 
character comparisons against P\_new, lcp's are updated, and the two subpartitions are recursively sorted, for a total 
of five partitions and four recursions (the middle partition contains the pivot and all equal elements).  

Where the lcp's are strictly unequal, string comparison against P_new would yield either a shorter lcp or the same length, so 
these sections retain P_prev as their previous pivot.  This key optimization prevents lcp's from decreasing.

Only elements which have equal lcp's against the previous pivot are ambiguous; these are subjected
to string comparison against P_new and given a second phase of (<.=,>) partitioning, and they receive updated lcp's against the new pivot.

The algorithm:

    inputs:
  
    a[] an array of strings
    lcp[] the lcp's of each a[i] against P_prev
    lo, hi:  the subrange of a[] being sorted
    direction:  1 if P\_prev < a[lo:hi], -1 if P\_prev < a[lo:hi]
    
    lcpquicksort( a, lcp, lo, hi, direction )
    0.  If lo >= hi return.
    1.  Pick a pivot P\_new among a[lo:hi].  Set vlcp = lcp(P\_new, P\_old).
    2.  if direction == 1, partition a and lcp into 3 buckets 
    having lcp(a[i], P\_prev) > vlcp, lcp == vlcp, and lcp < vlcp.  Let lt:gt be the range of ==.
    3.  If direction == -1, do as in step 2 but reverse the partition (<, ==, >).
    4.  lcpquicksort( a, lcp, lo, lt-1, direction )
    5.  lcpquicksort( a, lcp, gt+1, hi, direction )
    6.  split the middle partition by doing character comparisons against P_new and update/lengthen each element's lcp
        set lo,hi,lt,gt to the bounds of this split
    7.  lcpquicksort( a, lcp, lo, lt-1, -1 )
    8.  lcpquicksort( a, lcp, gt+1, hi, 1 )
