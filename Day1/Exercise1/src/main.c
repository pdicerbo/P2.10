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
 * taylor expanction (at order n) of e(x).
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

int main(int argc, char** argv){

  double RealExp, TaylorExp5, TaylorExp7;
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
    fprintf(fp, "%lg\t%lg\t%lg\t%lg\n", xmin + j*delta, RealExp, TaylorExp5, TaylorExp7);
  }
  fclose(fp);
  
  return 0;

}
