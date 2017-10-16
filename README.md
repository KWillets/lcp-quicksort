lcp-quicksort
=============

It only takes o(n log n) bits to sort n strings, we just don't know which ones.

LCP Quicksort uses string comparisons against a pivot, but then uses the length of the longest common prefix against the pivot (LCP) to subpartition the data after each pass.  Each subpartition is then recursively sorted starting from its known common prefix.

This subpartitioning works because, in sorted order, LCP is nondecreasing for keys less than a pivot, and nonincreasing for keys greater than it.  So we place the subpartitions in ascending order in the lower partition, and descending order in the upper, and obtain a partial ordering of the data.  We then recursively sort each subpartition in place.

The primary benefit of this method is that it finds one bit of difference in each comparison, and o(n) bits of difference in each pass, so it only needs to do o(log n) passes on average, regardless of how deeply buried these distinguishing bits are in the data. Radix-based sorts do not have this guarantee, so they can require in the worst case cache misses proportional to total key length (O(D)).

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




