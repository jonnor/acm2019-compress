# ACM Summer School Workshop 2019
# Cache and Memory Compression Techniques

Sean Isaac Geronimo Anderson and Jon Nordby
July 19, 2019

# Introduction

For this competition, we sought to evaluate the provided compression schemes, try new schemes from a literature search, and implement our own scheme for educational purposes. The new schemes were WKdm and LZ4, and we based our scheme on BDI.

# Algorithms

We ran several algorithms on the test data: SC^2, 0block, ZeroPoint, BDI, FPC, DuD, ZCA, WKdm, and LZ4. All but the last two were covered to some extent during today's tutorial, so we will not describe them here. The Wilson-Kaplan WKdm algorithm is used by Apple for compressing the swap file[3]. LZ4 is a compression algorithm with decompression speeds approaching that of main memory[5].

Additionally, we attempted implementing BDI for educational purposes. Our implementation checks if a given word's value can be represented as an offset from half of the word size, or not. If so, we store the offset, else we store the word as-is. An indicator bit establishes whether a given value is compressed.

## LZ4 in hardware
We searched the literature for implementations of LZ4 that could be applicable to memory compresssion.

Xilinx provides a LZ4 implentation for their FPGAs [6].
On a Alveo U200 running at 256 MHz, it can reach over 1.6 GB/s throughput for decompression and compression,
using a total of 110k LUTs, 105k REG, and 250 RAM blocks.

There are also several papers showing hard-ware acceleration for variation of the LZ4 algorithm.

LZ4m [4] is a LZ4 variant in software designed specifically for compressing memory, 
that performs favorably over LZ4, LZO1x and WKdm.

In "Design of Hardware Accelerator for Lempel-Ziv 4 (LZ4) Compression" [7]
authors designed a n accelerator for LZ4 using FPGA and fabricated using 65nm CMOS technology.
It supports up to 4Gbit/s compression with 75MHz clock, with 392 K gate counts.

In "Data Compression Device based on Modified LZ4 Algorithm" [8] several variations of
an accelerator nicknamed nicknamed MLZ4-X were designed.
They got up to 1.92Gbps on a 260MHz FPGA.


# Experiment, Results, and Analysis

We used the provided compression tools for evaluating all schemes, except WKdm and LZ4, for which we used code from the respective project pages. The test data were as provided on the tutorial network share, except that we extracted the memory area from each core dump and tested each algorithm only on this memory area. Here are tables and graphs illustrating our results:

![Results](https://raw.githubusercontent.com/jonnor/acm2019-compress/master/results.png "Results")


As shown, WKdm and LZ4 perform admirably, and a high-level code analysis yields that these algorithms should fit within the contest requirements, with some caveats: We did not find literature on WKdm (besides the source code) during the contest period, and so are not able to argue strongly about whether the algorithm could operate in 20 or fewer cycles. It uses a small number of temporary variables, certainly within 64 KiB in total, so the area constraint should be satisfied. It also performs about as quickly as the provided compression schemes, so if 0block is fast enough to satisfy the throughput constraint (32 GiB/s), then so should WKdm.

There is another significant caveat which we could not resolve during the contest: Both the WKdm and LZ4 code operate on page-sized blocks, and thus have a considerable advantage compared with the provided compression tools (which operate on cache line-sized blocks). As such, the results for these schemes appear promising, but are unfortunately unfair comparisons with the majority of the compression tools.

As for our BDI-based scheme (cons8), we expect that its low performance is due to most values not exhibiting value locality near our chosen base value (half word size). This simple approach certainly takes fewer than 20 cycles (one cycle to test the value, and another to subtract the base value as necessary), and clearly fits within the other constraints. But the test data show that the scheme cannot achieve a worthwhile compression ratio. There are other ideas for improvement which we cover in the next section.

# Conclusion

The tutorial and contest today have been an interesting and enjoyable introduction to cache and memory system compression. We found baseline numbers for the tutorial-provided algorithms, and compared them with new algorithms from a literature search, albeit inadvertently unfairly. Our simple BDI-based scheme was interesting to invent, discuss, and implement within the provided tutorial code.

Future work would entail finishing limiting WKdm and LZ4 to use cache line-sized inputs, and modifying the simple BDI-based scheme to calculate a more useful base value.


# Sources


[1] https://bitbucket.org/eldstal/acm_workshop_2019/src/master/

[2] https://github.com/berkus/wkdm

[3] https://arstechnica.com/gadgets/2013/10/os-x-10-9/17/#compressed-memory

[4] http://csl.skku.edu/papers/icce17.pdf

[5] https://github.com/lz4/lz4

[6] https://github.com/Xilinx/Applications/tree/master/data_compression/xil_lz4

[7] https://doi.org/10.1587/elex.14.20170399

[8] https://doi.org/10.1109/TCE.2018.2810480

