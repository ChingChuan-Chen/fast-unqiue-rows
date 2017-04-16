
## Fast unique rows written in C++ for MatLab

I compiled this code with Microsoft Visual Studio 2015 and Intel Parallel Studio XE 2017 and linking to armadillo library.

And my matlab version is 2016a. The compile command and example:

``` matlab
MKLROOT = 'C:\Program Files (x86)\IntelSWTools\compilers_and_libraries_2017.1.143\windows\mkl';
ICLLIB = 'C:\Program Files (x86)\IntelSWTools\compilers_and_libraries_2017.1.143\windows\compiler\lib\intel64_win';
OPTIMFLAGS = 'OPTIMFLAGS= /O3 /DNDEBUG /QxHost /Qparallel /Qopenmp /Qprec-div- /Qipo /Qinline-calloc';
LINKER = 'LINKER=xilink';
mex('-f', 'intel_cpp_17_vs2015.xml', '-v', '-largeArrayDims', '-IC:\armadillo-7.800.2\include', ...
    ['-I', MKLROOT, '\include'], ['-L', MKLROOT, '\lib\intel64'], ['-L', ICLLIB], ...
    'unique_rows.cpp', OPTIMFLAGS, LINKER, '-lmkl_intel_lp64', '-lmkl_intel_thread', ...
    '-lmkl_core', '-llibiomp5md')

A = [9 2 9 5; 9 2 9 0; 9 2 9 5; 9 2 9 0; 9 2 9 5];
unique_rows(A)
%      9     2     9     0
%      9     2     9     5
```
