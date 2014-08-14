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
determine the order of any element A against any new pivot P\_new by first comparing lcp(A,P\_prev) and lcp(P\_new, P\_prev), and only on equality doing character comparisons.  This process is essentially a two-key comparison.

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
which are greater than, equal to, or less than P_new's, and the two unequal partitions are recursively sorted.  In the second phase, the middle partition is split by actual 
character comparisons against P\_new, lcp's are updated, and the two unequal subpartitions are 
recursively sorted, for a total of five partitions and four recursions (the middle partition contains the pivot and all equal elements, as with multikey quicksort).  

In Phase 1, where the lcp's are strictly unequal, string comparison against P_new would yield either a shorter lcp or the same length, so 
these sections are sorted separately, retaining P_prev as their previous pivot, instead of merging them with the adjacent partitions from Phase 2.  This is equivalent to two rounds of multikey quicksort, where the first key is (decreasing or increasing) lcp, and the second is actual character comparisons.  This key optimization prevents lcp's from decreasing.

Only elements which have equal lcp's against the previous pivot are ambiguous; these are subjected
to string comparison against P_new and given a second phase of (<.=,>) partitioning, and they receive updated lcp's against the new pivot.

The algorithm:

    inputs:
  
    a[] an array of strings
    lcp[] the lcp's of each a[i] against P_prev
    lo, hi:  the subrange of a[] being sorted
    direction:  1 if P_prev < a[lo:hi], -1 if P_prev < a[lo:hi]
    
    lcpquicksort( a, lcp, lo, hi, direction )
    0.  If lo >= hi return.
    1.  Pick a pivot P_new among a[lo:hi].  Set vlcp = lcp(P_new, P_old).
    2.  if direction == 1, partition a and lcp into 3 buckets 
    having lcp(a[i], P\_prev) > vlcp, lcp == vlcp, and lcp < vlcp.  Let lt:gt be the range of ==.
    3.  If direction == -1, do as in step 2 but reverse the partition (<, ==, >).
    4.  lcpquicksort( a, lcp, lo, lt-1, direction )
    5.  lcpquicksort( a, lcp, gt+1, hi, direction )
    6.  split the middle partition by doing character comparisons against P_new and update/lengthen each element's lcp
        set lo,hi,lt,gt to the bounds of this split
    7.  lcpquicksort( a, lcp, lo, lt-1, -1 )
    8.  lcpquicksort( a, lcp, gt+1, hi, 1 )


TODO: 

Consecutive LCP's

This version of the algorithm does not attempt to construct LCP's of consecutive keys (where lcp[i] = lcp(a[i],a[i-1]) for i>0, lcp[0]=0),
 since it mainly saves LCP's vs. the last pivot each key has encountered.  However it may be possible to build consecutive LCP's 
while unrolling the recursion and return a useful list of consecutive lcp's to the caller.

If we make the recursive assumption that lcpquicksort has returned consecutive LCP subarrays from each of the four recursive calls above, we
simply need to modify elements at the boundaries of the subarrays to make lcp[lo..hi] consecutive as well.

For direction == 1:

For the LCP-partitioned ranges sorted in steps 4 and 5, for lcp(a[gt-1], a[gt]) is the maximum lcp(a[i], P_prev) for gt < i <= hi.
This follows from the partitioning and the fact that if A,B > P_prev and lcp(A, P_prev) > lcp(B,P_prev), then lcp(A,B)=lcp(B,P_prev).  So
if we remember the maximum value while partitioning, we can apply it after the recursion has returned.

Similarly lcp(a[lt-1], a[lt]) == lcp(a[lt], P_prev) == vlcp.

In steps 7 and 8, lcp( a[lt-1], a[lt] ) is the maximum of lcp(a[i], P_new) for lo <= i < lt, and similar for the lcp(a[gt],a[gt+1]), since
the closest key to the pivot in either direction always has the longest lcp of all the comparands.


Direction can be encoded in the sign bit of lcp[i]

To obviate the direction flag, the result of lcpstrcmp can flip the sign bit of 
the lcp returned based on whether a[i] > or < the pivot.  The lcp's passed in will
be all >= 0 or all <= 0 when direction == -1 or 1, resp., and regular int comparisons
will put them in the correct order (instead of the two cases using separate code).  

lcpstrcmp has to use abs(*lcp) in this case.  


Shorter/Rounded lcp's

A maximum on lcp length should just incur more comparisons but not sink the whole algorithm.  The impact 
is that a) more keys will agree on lcp and incur character comparisons in the middle, and b) character comparisons 
will be duplicated at positions beyond the maximum, but pivoting will be correct at each level.

Rounding may also work, with similar impact of more comparisons but correct behavior.
lcp's can be small integers in units of k characters, eg k = 16 or the cache line size, to save
bits.
