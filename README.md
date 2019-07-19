# ACM Summer School Workshop 2019
# Cache and Memory Compression Techniques

# Introduction

* Baseline
* WKdm and LZ4
* BDI implementation

# Algorithms

We ran several algorithms on the test data: SC^2, 0block, ZeroPoint, BDI, FPC, DuD, ZCA, WKdm, and LZ4. All but the last two were covered to some extent during today's tutorial, so we will not describe them here. The Wilson-Kaplan WKdm algorithm is used by Apple for compressing the swap file[3], and we decided to try it after seeing its performance in a paper on LZ4m [4] (which is an LZ4 variant specifically for compressing memory, and we did not have time to implement it in time for this contest). LZ4 is a compression algorithm with decompression speeds approaching that of main memory [5].

# Experiment, Results, and Analysis

# Conclusion

# Sources


[1] https://bitbucket.org/eldstal/acm_workshop_2019/src/master/

[2] https://github.com/berkus/wkdm

[3] https://arstechnica.com/gadgets/2013/10/os-x-10-9/17/#compressed-memory

[4] http://csl.skku.edu/papers/icce17.pdf

[5] https://github.com/lz4/lz4

