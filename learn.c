#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//function to print out one dimensional arrays.
void printArray1d(double* arr, int r){
  int i;
  for(i=0;i<r;i++){
    printf("%g,\t",arr[i]);
  }
  printf("\n");
}
//function to print out two dimensional arrays.
void printArray2d(double** arr, int r,int c){
  int i;
  for(i=0;i<r;i++){
    int j;
    for(j=0;j<c;j++){
      printf("%g\t",arr[i][j]);
    }
    printf("\n");
  }
}
//multiply
//multiplies two matrices together returning their product
double ** multiply(double** arr1,int r1,int c1,double** arr2,int r2,int c2){
  //makes 2d array prod[r1][c2]
  double** prod;
  prod = (double**)malloc(r1*sizeof(double*));
  int a;
  for(a=0;a<r1;a++){
    prod[a] = (double*)malloc(c2*sizeof(double));
  }
  //multiply arr*arr2
  int i;
  for(i=0;i<r1;i++){
    int j;
    for(j=0;j<c2;j++){
      prod[i][j]=0;
      int l;
      for(l=0;l<c1;l++){
        prod[i][j]+=arr1[i][l]*arr2[l][j];
      }
    }
  }
  return prod;
}

//transpose
//computes the transpose of a matrix
double** transpose(double** arr,int r,int c){
  //makes 2d array prod[r1][c2]
  double** arrT;
  arrT = (double**)malloc(c*sizeof(double*));
  int a;
  for(a=0;a<c;a++){
    arrT[a] = (double*)malloc(r*sizeof(double));
  }
  //compute transpose
  int i,j;
  for(i=0;i<r;i++){
    for(j=0;j<c;j++){
      arrT[j][i] = arr[i][j];
    }
  }
  
  return arrT;
}
//Helper methods for inverse:
//dbc
//divides row by constant
double** dbc(double** arr,double c, int row, int k){
  if(c == 0){
    printf("divide by zero\n");
    return NULL;
  }
  int i;
  for(i=0;i<2*k;i++){
    arr[row][i] = arr[row][i]/c;
  }
  return arr;
}
//subRow
//subtract row by row times constant
double** subRow(double** arr,int rowJ,int rowI, double c, int k){
  int i;
  for(i=0;i<2*k;i++){
    arr[rowJ][i] = arr[rowJ][i] - arr[rowI][i]*c;
  }
  return arr;
}
//Augmentation method:
//augments a matrix wiht the identity matrix.
double** augment(double** arr,int k){
  //makes 2d array of (k+1)*2(k+1) attributes.
  double** arrAug;
  arrAug = (double**)malloc(k*sizeof(double*));
  int i;
  for(i=0;i<k;i++){
    arrAug[i] = (double*)malloc((2*k)*sizeof(double));
  }
  //fills arrAug with elements of arr
  int j,l;
  for(j=0;j<k;j++){
    for(l=0;l<k;l++){
      arrAug[j][l] = arr[j][l];
    }
  }
  //fill other side with identity matrix
  int x,y;
  for(x=0;x<k;x++){
    for(y=k;y<2*k;y++){
      if(x == y-k){
        arrAug[x][y] = 1;
      }
      else{
        arrAug[x][y] = 0;
      }
    }
  }
  return arrAug;
}
//De-Augmentation method:
//un augments a matrix wiht the identity matrix after inverse is computed.
double** deaugment(double** arrAug,int k){
  //makes 2d array of (k+1)*2(k+1) attributes.
  double** arr;
  arr = (double**)malloc(k*sizeof(double*));
  int i;
  for(i=0;i<k;i++){
    arr[i] = (double*)malloc(k*sizeof(double));
  }
  //fills arr with elements of arrAug after gauss jordian elimination
  int x,y;
  for(x=0;x<k;x++){
    for(y=k;y<(2*k);y++){
      arr[x][y-k] = arrAug[x][y];
    }
  }
  return arr;
}
//inverse
//computes the inverse of a matrix by using gauss-jordan elimination
double** inverse(double** arr,int r){
  //r=k+1
  //1. augment with identity matrix
  double** arrAug = augment(arr,r);
  //printf("\n[inverse]arrAug:\n");
  //printArray2d(arrAug,r,2*r);
  //2. perform gauss-jordinian elimination to remove elements below diagonal
  int i,j;
  for(i=0;i<r;i++){
    arrAug = dbc(arrAug,arrAug[i][i],i,r);
    for(j=i+1;j<r;j++){
      arrAug = subRow(arrAug,j,i,arrAug[j][i],r);
    }
  }
  //printf("\n[inverse]step2:\n");
  //printArray2d(arrAug,r,2*r);
  //3. continue G-J elimination to remove elements above diagonal
  int a,b;
  for(a=0;a<r;a++){
    for(b=a+1;b<r;b++){
      arrAug = subRow(arrAug,a,b,arrAug[a][b],r);
    }
  }
  //printf("\n[inverse]step3:\n");
  //printArray2d(arrAug,r,2*r);
  //4.
  double** arrInv = deaugment(arrAug,r);
  //printf("\n[inverse]step4:\n");
  //printArray2d(arrInv,r,r);
  return arrInv;
}
//MAIN
int main(int argc,char ** argv){
  	FILE* f = fopen(argv[1],"r");//opens file
    if(f == 0){
      printf("error");
      return -1;
    }
  	int k;
    int n;
    fscanf(f,"%i\n",&k);
    fscanf(f,"%i\n",&n);
    
    //makes 2d array X of  (n)*(k+1) attributes.
    double** x;
    x = (double**)malloc(n*sizeof(double*));
    int i;
    for(i=0;i<n;i++){
      x[i] = (double*)malloc((k+1)*sizeof(double));
    }
    //makes array Y of n prices.
    double** y = (double**)malloc(n*sizeof(double*));
    int yfill;
    for(yfill=0;yfill<n;yfill++){
      y[yfill] = (double*)malloc(sizeof(double));
    }
    //fill array X with attributes and Y with prices
    //printf("Test 1");
    int j;
    for(j=0;j<n;j++){
      int l;
      x[j][0] = 1;
      for(l=1;l<(k+1);l++){
          fscanf(f,"%lf,",&x[j][l]);
      }
      fscanf(f,"%lf\n",&y[j][0]);     //segfault  
    }
    //arrays filled
    //printf("X:\n");
    //printArray2d(x,n,k+1);
    //printf("Y:\n");
    //printArray2d(y,n,1);
    //W=(XT*X)^-1 * XT * Y
    /*
    x1=xT
    x2=x1*x
    x3=inv(x2)
    x4=x3*x1
    w=x4*y
    */
    double** x1 = transpose(x,n,k+1);
    //printf("\nXT:\n");
    //printArray2d(x1,k+1,n);
    double** x2 = multiply(x1,k+1,n,x,n,k+1);
    //printf("\nXT*X:\n");
    //printArray2d(x2,k+1,k+1);
    double** x3 = inverse(x2,k+1);
    //printf("\n(XT*X)^-1:\n");
    //printArray2d(x3,k+1,k+1);
    //double** xTest = multiply(x3,k+1,k+1,x2,k+1,k+1);
    //printf("\n(XT*X)^-1 * (XT*X):\n");
    //printArray2d(xTest,k+1,k+1);
    double** x4 = multiply(x3,k+1,k+1,x1,k+1,n);
    //printf("\n(XT*X)^-1*XT:\n");
    //printArray2d(x4,k+1,n);
    double** w = multiply(x4,k+1,n,y,n,1);
    //w filled.
    //printf("\nw filled:\n");
    //printArray2d(w,k+1,1);
    fclose(f);
    //begin predicting house prices from testcases:
    FILE* f2 = fopen(argv[2],"r");//opens file
    if(f2 == 0){
      printf("error");
      return -1;
    }
    int numHouses;
    fscanf(f2,"%i\n",&numHouses);
    for(i=0;i<numHouses;i++){
      double housePrice = 1*w[0][0];
      int j;
      for(j=1;j<k+1;j++){
        double attribute;
        if(j==k){
          fscanf(f2,"%lf\n",&attribute);
        }
        else{
          fscanf(f2,"%lf,",&attribute);
        }
        //printf("attribute[%i]: %lf\n",j,attribute);
        //printf("weight[%i]: %lf\n",j,w[j][0]);
        housePrice += (attribute * w[j][0]);
      }
      printf("%0.0f\n",housePrice);
    }
    
    fclose(f2);
	  return 0;
}
