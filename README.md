CM4_CIC_Filter
==============

A implementation of a Cascaded Integrator Comb filter for the Cortex M4, designed to have an API similar to CMSIS-DSP.

**NOT CURRENTLY WORKING**

Requirements for Testing
========================

* Python 2.7
* matplotlib
* numpy
* Ipython and Bokeh (for full notebook, not for basic tests)
* GNU make
* A C compiler

Installing/Testing
==================
With python 2.7, matplotlib and numpy installed.

###To graph sin, square waves, frequency response.


```
$> make
$> make test 
```

See ./test_results for some pretty graphs.

###To test a full 2 stage chained filter
(requires ipython)

```
$> ipython notebook
```
Then open the notebook and run each cell in turn.

###Cleanup

```
$> make clean
```



CAVEATS
=======
* cic_decimate_q16 is probably broken, do not use.
* The full two stage filter is only in the ipython notebook. 

