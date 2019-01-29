***********************************
*                                 *
* Contrast invariant SNR toolbox  *
*                                 *
***********************************

***********************************
DEVELOPERS:
Pierre Weiss, Gabiel Bathie, Paul Escande. 

Troubleshooting: Pierre Weiss
First release: 29/01/2019

Note: this toolbox is based partly on 
https://github.com/pmonasse/flst 
by Pascal Monasse. We just converted the format to double.

***********************************
OVERVIEW:
This toolbox contains Matlab codes to reproduce the experiments from 

Contrast Invariant SNR and Isotonic Regressions.
P. Weiss, P. Escande, G. Bathie, Y. Dong, International Journal of Computer Vision (2019).

Please cite this paper if you use the toolbox for your research.

***********************************
INSTALLATION:

go to Matlab and type:
>> compile

Launch the demo:
>> demo_SNR

***********************************
CONTENTS:
- images/ : a list of test images
- mex_files/ : a list of c++ mex files
   - idcc_mex.cpp: fast computation of connected components of an image
   - project_llt_mex_double.cpp: computes the projection of an image onto the set of images with a given tree of shape
   - isotonic_regression_tree.cpp : solves an isotonic regression on a polytree with dynamic programming
   - the functions have their _double counterpart since the default is to work with 8 bits images
- Matlab main files:
   - demo_isotonic_regression_dp.m : an example that computes the isotonic regression on a polytree and compares the result to interior point methods (the comparison requires CVX being installed)
   - demo_SNR.m : an example to evaluate the different SNRs
   - demo_difference.m : an example to show how the toolbox can be used to compute the difference of images
   - isotonic_regression_iterative.m : solves isotonic regressions with first order methods
   - SNR,SNR_global, SNR_local1, SNR_local2: the different SNRs

***********************************
DETAILS:

- the images need to be quantized over a not too large number of bits, otherwise the stack may be saturated and result in segmentation fault.