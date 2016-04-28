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

  if(x0*x0 == 0)
    return x0;
  
  for(j = 0; j < N; j++)
    xtmp = xtmp - MyFunc(xtmp, x0)/MyDeriv(xtmp);

  return xtmp;

}

int main(int argc, char** argv){

  int N = 4;
  int NPoint = 100;
  int j, NDef;

  double* MyGrid = (double*)malloc(NPoint*sizeof(double));
  double RealSqrt, MySqrt, MyVal, CheckVal;
  double xmin = 0.;
  double xmax = 20.;
  double delta = (xmax - xmin)/NPoint;
  FILE *fp, *cp;

  // fill MyGrid
  for(j = 0; j < NPoint; j++)
    MyGrid[j] = xmin + j*delta;

  fp = fopen("MyData.dat", "w");
  
  for(j = 0; j < NPoint; j++){
    RealSqrt = sqrt(MyGrid[j]);
    MySqrt = RN_Method(MyGrid[j], N);
    fprintf(fp, "%lg\t%lg\t%lg\n", xmin+j*delta, RealSqrt, MySqrt);
  }
  fclose(fp);

  // convergence check
  NDef = 1;
  
  for(j = 0; j < NPoint; j++){
    MyVal = sqrt(MyGrid[j]);
    N = 1;
    MySqrt = RN_Method(MyGrid[j], N);

    while(fabs(MySqrt - MyVal) > 1.e-14){
      N++;
      MySqrt = RN_Method(MyGrid[j], N);
    }

    if(NDef < N)
      NDef = N;
  }

  printf("\n\tThe maximum number of iteration in order to obtain \n\tfabs(sqrt(x) - MySqrt(x)) < 1.e-14 in the choosed interval is equal to %d\n\n", NDef);
    
  return 0;
}
