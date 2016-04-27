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

  double RealExp, TaylorExp;
  double xmin, xmax, delta;
  int NPoints, j, n;
  
  FILE* fp;

  
  xmin = -10.;
  xmax = 10.;
  NPoints = 100;
  n = 5; // order of taylor expansion
  
  fp = fopen("MyResults.dat", "w");
  delta = (xmax - xmin) / NPoints;

  for(j = 0; j < NPoints; j++){
    RealExp = exp(xmin + j*delta);
    TaylorExp = ExponentialTaylor(xmin + j*delta, n);
    fprintf(fp, "%lg\t%lg\t%lg\n", xmin + j*delta, RealExp, TaylorExp);
  }
  fclose(fp);
  
  /* int test1, test2; */
  /* test1 = MyFactorial(3); */
  /* test2 = MyFactorial(4); */
  /* printf("\n\tHello World\n\n\t3! = %d, 4z! = %d\n\n", test1, test2); */

  return 0;

}
