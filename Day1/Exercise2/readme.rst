=================================================================
Exercise 2
=================================================================

Linear spline interpolation
---------------------------
           
  - An initial code is provided in the Input directory 
  - Two main files: tester.c and spline.c
  - It prints error and timing information
  - Spline coefficients are calculated in genspline

Task: write the subroutine that uses linear spline interpolation to calculate exp(x) (in spline.c) 
--------------------------------------------------------------------------------------------------

 - Compile with ``make 32bit-fpu-gcc``
 - Run with ``./Obj_32bit-fpu-gcc/tester num repnum``, where num is the number of points x for which exp(x) is calculated, repnum is the number of repetitions
 - A good choice is ``./Obj_32bit-fpu-gcc/tester 1000 1``

--------------------------

  - We would like to understand how the performance and accuracy of our interpolation depend on the parameters used; 
  - Increase the nmax number of intervals in which the [-10,10] is devided, from 100 to 2000: 
  - How is the error changing? How is the execution time changing?
