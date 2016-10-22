lcp-quicksort
=============

A string-sorting algorithm which uses longest common prefix (LCP) found during string comparisons to sub-sort the data after each pass.  The primary benefit of this method is that it allows the entire LCP of each key pair to be consumed in each pass, thereby outperforming algorithms which increase the radix by only a fixed amount.  For strings with long LCP's (long distinguishing prefixes) this difference is significant in the number of passes through the data (log n vs. D/n), cache complexity, instruction count, and other overheads such as stack depth.  It is in-place except for an auxillary array of n LCP values, and the stack.  

While the optimal number of comparisons O(D + n log n) has already been reached by a number of algorithms, the benefit here is a smaller coefficient on D (asymptotically the maximum throughput of SIMD string instructions, currently around 5.33 bytes/cycle).  Very long keys, such as files or disk blocks, can be sorted for only a fraction of a cycle per byte.  

Building
========

$ make lcpsort
$ make lcpsort-sse


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

This algorithm works in two phases: (I) partitioning by string comparisons against a pivot into <,=,>; and (II) subsorting the upper and lower partitions by
the LCP's found in (I), and then recursively applying (I) to each subpartition found, using its common prefix length as the starting depth.    




