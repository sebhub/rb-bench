# Red-Black Tree Implementation Benchmark

The following implementations are tested

* [Boost](http://www.boost.org/doc/libs/1_57_0/doc/html/boost/intrusive/rbtree.html)
* [BSD](https://github.com/freebsd/freebsd/blob/master/sys/sys/tree.h)
* BSD for RTEMS - Maybe the next implementation for RTEMS
* [Chain](https://github.com/RTEMS/rtems/blob/master/cpukit/score/include/rtems/score/chain.h)
* [Eternally Confuzzled](http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx)
* [Jason Evans - Newer](http://www.canonware.com/download/rb/rb_newer/)
* [Jason Evans - New](http://www.canonware.com/download/rb/rb_new/)
* [Jason Evans - Old](http://www.canonware.com/download/rb/rb_old/)
* [JFFS2](https://github.com/RTEMS/rtems/blob/master/cpukit/libfs/src/jffs2/include/linux/rbtree.h)
* [Left-Leaning](http://www.25thandclement.com/~william/projects/llrb.h.html)
* [Linux](https://github.com/torvalds/linux/blob/master/include/linux/rbtree.h)
* RTEMS Compact - Colour is encoded in the parent pointer
* [RTEMS](https://github.com/RTEMS/rtems/blob/master/cpukit/score/include/rtems/score/rbtree.h)

The test program produces an XML file.  Use the plot.py script to print some
diagrams.

Some sample reports:

![Freescale P1020RDB](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-qoriq_p1020rdb.png)
![Altera Cyclone V](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-altcycv_devkit.png)
![Xilinx Zynq](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-xilinx_zynq_zc702.png)
![Aeroflex Gaisler NGMP](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-ngmp.png)
![Freescale MCF5484](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-m5484FireEngine.png)
![Linux](https://raw.githubusercontent.com/sebhub/rb-bench/master/reports/test-linux.png)
