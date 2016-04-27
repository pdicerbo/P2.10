=================================================================
Exercise 1
=================================================================

First task: approximate the natural exponential e(x) with a Taylor approximation, for x in [-10; 10]
----------------------------------------------------------------------------------------------------
 
  - we are just going to inspect the result graphically
  - Write a program that prints the value of the built-in exponential function and its Taylor approximation on a grid of points between x=-10 and x=10
  - Do it with the Taylor polynomial of degree 5 and of degree 7
  - First we have to remember how the Taylor series looks like: e(x) = Sum x^n/n!
  - In this first exercise we just look qualitatively at the results: where does the approximation go REALLY wrong?
  - Where does it look like to work best? Why? How is it connected to our choice of the Taylor expansion?
  - Increasing further the degree of the polynomial, what do you expect to happen?

Second task: Pade` approximation
--------------------------------
  - We are going to use the order [4/4]
    - See the formula for the coefficients in the case of the exponential

  - Where is this approximation failing?
  - Where does it work well? 

Third task: compare
-------------------

  - Compare the Taylor and Pade` approximations: which works better? In which region?
