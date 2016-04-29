=================================================================
Exercise 5
=================================================================

A fast and accurate apprximation to the natural exponential
------------------------------------------------------------

         
  - We are going to explore how to combine different methods we have learnt to obtain a fast and precise approximation for the exponential e(x)
  - We are going to start from the code present in the working directory Taylor0: ``exp.c`` and ``tester.c``
  - ``tester.c``: it calculates the exponential by a number of methods for a number (to be provided) of random points x between -10.0 and 10.0
  - ``exp.c`` contains the subroutines to calculate the exponential
  - ``tester.c`` also calculates execution times for the different methods, and provides an average error with respect to the standard exponential function

------

  - The code in ``Taylor0`` performs a simple Taylor expansion: at what degree?
  - The code in ``Pade0`` performs a simple Pade expansion: what order of approximant is used?
  - Check their performance

------

  - Second task: reduce the range of testing from [-10,10] to [-0.5,0.5]: what has changed? Has the execution time improved? And the error?
  - Third task: exploit the insight from the second task and modify the Pade` code exploiting the other tricks
  - Our Pade` approximation works best in a neighbourhood of 0. The strategy is now to exploit this fact to use these approximation only in a range around 0, also when calculating e(x) for x in [-10,10]
  - Moreover, we are going to work with integers when possible
  - In particular, as an intermediate result we are going to calculate an exponential of 2y, exploiting the fact that ex = 2log(exp(x)) = 2x log(e), where log is the logarithm in base 2: log2(x)

------

  - Which is the fastest method to calculate exp(x)? 

  - Which is the most accurate?
