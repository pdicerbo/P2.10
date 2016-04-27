#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * This function is the trivial implementation
 * of the Factorial function.
 */

int MyFactorial(int n){
  int prod;
  if(n == 1 || n == 0)
    return 1;
  else{
    prod = MyFactorial(n-1);
    return prod*n;
  }
}

/*
 * This function returns the value of the
 * Taylor expanction (at order n) of e(x).
 * The expanction is centered in x_0 = 0
 */
double ExponentialTaylor(double x, int n){

  int j;
  double fac;
  double value = 0.;
  
  for(j = 0; j < n+1; j++){
    fac = (double) MyFactorial(j);
    value += pow(x, j)/fac;
  }
  return value;
}

/*
 * This function returns the value of the
 * Pade' approximation (at order [p/q]) of e(x).
 * The expanction is centered in x_0 = 0
 */
double ExponentialPade(double x, int p, int q){
  double num = 0.;
  double denom = 0.;
  int j;
  
  for(j = 0; j < p+1; j++)
    num += ((double) (MyFactorial(p + q - j)*MyFactorial(p)) * pow(x, j))/
      ((double) (MyFactorial(p+q)*MyFactorial(j)*MyFactorial(p-j)));

  for(j = 0; j < q+1; j++)
    denom += ((double) (MyFactorial(p + q - j)*MyFactorial(q)) * pow(-x, j))/
      ((double) (MyFactorial(p+q)*MyFactorial(j)*MyFactorial(q-j)));

  return num/denom;
  
}

int main(int argc, char** argv){

  double RealExp, TaylorExp5, TaylorExp7, PadeExp;
  double xmin, xmax, delta;
  int NPoints, j;
  
  FILE* fp;

  
  xmin = -10.;
  xmax = 10.;
  NPoints = 100;
  
  fp = fopen("MyResults.dat", "w");
  delta = (xmax - xmin) / NPoints;

  for(j = 0; j < NPoints; j++){
    RealExp = exp(xmin + j*delta);
    TaylorExp5 = ExponentialTaylor(xmin + j*delta, 5);
    TaylorExp7 = ExponentialTaylor(xmin + j*delta, 7);
    PadeExp = ExponentialPade(xmin+j*delta, 4, 4);
    fprintf(fp, "%.10g\t%.10g\t%.10g\t%.10g\t%.10g\n", xmin + j*delta, RealExp, TaylorExp5, TaylorExp7, PadeExp);
  }
  fclose(fp);
  
  return 0;

}
