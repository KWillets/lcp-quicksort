lcp-quicksort
=============

An string quicksort with O(D+n*log n) average character comparisons based on reuse of the longest common prefix length from each string comparison.  LCP length is used to 
subpartition the data after each scan of string comparisons.

The algorithm requires fewer key accesses (ie cache misses) than radix-iterating algorithms.  For datasets with long fixed or low-entropy prefixes (eg urls),
this algorithm has less recursive overhead and fewer cache misses than Multikey Quicksort.  The latter algorithm makes one recursive call for each node on a trie 
built from the data, while this algorithm only makes one call for each node on a Patricia tree built from the data.  



The Algorithm
=============

Recently a number of classic sorting algorithms have been modified to use lcp (longest common prefix) information during string comparisons.
The key observation is that if two strings A,B have already been compared to a third key C, 
and the lcp's from that comparison have been retained, then
the ordering of A and B can be determined by looking at lcp length, and then character comparisons:

Lemma 1 (from lcp merge sort paper https://www.jstage.jst.go.jp/article/ipsjdc/4/0/4_0_69/_article):  

If A > C and B > C, and lcp(A,C) > lcp(B,C), then A < B. Proof follows from the fact that the first character
after the lcp is the tie breaker for the comparison, and if lcp(A,C) > lcp(B,C), then B[lcp(B,C)] > C[lcp(B,C)] == A[lcp(B,C)], so B > A.


If we imagine the per-character strcmp results as follows (either = or >), we see why a longer lcp matters:

    C: XXXXXXXXXXX... (some chars)
    A: ========>
    B: ===>

(Figure 1)

The character point where B > C is also the point where B > A.

We can reverse > and < if needed (ascending or descending), if C > A,B.  Use of a pivot typically creates two ranges with opposite direction.

This algorithm works in two phases: (I) partitioning by string comparisons against a pivot into <,=,>; and (II) subsorting the < and > ranges by
the LCP's found in (I), and then recursively applying (I) to each subpartition found.  



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


Shorter/Rounded lcp's

A maximum on lcp length should just incur more comparisons but not sink the whole algorithm.  The impact 
is that a) more keys will agree on lcp and incur character comparisons in the middle, and b) character comparisons 
will be duplicated at positions beyond the maximum, but pivoting will be correct at each level.

Rounding is also possible, with similar impact of more comparisons but correct behavior.
lcp's can be small integers in units of k characters, eg k = 16 or the cache line size, to save
bits.  Tests show some slowdown from the extra comparisons.

Other lcp sorting algorithms

This algorithm uses integer quicksort to order the lcp's, but other algorithms will work as well.  

Disposing of lcp's

The range of lcp's is typically << n, so a bucket sort might allow the keys to be bucketed immediately after
pivot string comparison without the need for per-key lcp values.  In random strings the distribution is geometric, so average 
number of buckets needed is probably o(log n).

Caching

Some multikey quicksorts etc. have introduced caching of one or more characters to avoid key accesses.  The same can be done with this algorithm.

SSE/multicharacter instructions

Since this algorithm reads as many characters as necessary in each comparison, SSE multicharacter comparison instructions can be used.