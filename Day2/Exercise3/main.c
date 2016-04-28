#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double MyFunc(double x, double x0){
  return x*x - x0;
}

double MyDeriv(double x){
  return 2.*x;
}

double RN_Method(double x0, int N){
  int j;
  double xtmp = x0;

  if(x0 == 0)
    return x0;
  
  for(j = 0; j < N; j++)
    xtmp = xtmp - MyFunc(xtmp, x0)/MyDeriv(xtmp);

  return xtmp;

}

int main(int argc, char** argv){

  int N = 4;
  int NPoint = 100;
  int j;

  double* MyGrid = (double*)malloc(NPoint*sizeof(double));
  double RealSqrt, MySqrt, MyVal, CheckVal;
  double xmin = 0.;
  double xmax = 20.;
  double delta = (xmax - xmin)/NPoint;

  // fill MyGrid
  for(j = 0; j < NPoint; j++)
    MyGrid[j] = xmin + j*delta;

  FILE* fp;
  fp = fopen("MyData.dat", "w");
  
  for(j = 0; j < NPoint; j++){
    RealSqrt = sqrt(MyGrid[j]);
    MySqrt = RN_Method(MyGrid[j], N);
    fprintf(fp, "%lg\t%lg\t%lg\n", xmin+j*delta, RealSqrt, MySqrt);
  }
  fclose(fp);

  // check convergence calculating sqrt(5)
  CheckVal = 15.;
  MyVal = sqrt(CheckVal);
  N = 1;
  MySqrt = RN_Method(CheckVal, N);

  while(fabs(MySqrt - MyVal) > 1.e-14){
    N++;
    MySqrt = RN_Method(CheckVal, N);
  }

  printf("\n\tIn order to obtain abs(sqrt(x) - MySqrt(x)) < 1.e-14 we need %d iteration\n", N);
  printf("\tsqrt(%lg) = %.16g, MySqrt = %.16g\n\n", CheckVal, MyVal, MySqrt);
  
  return 0;
}
