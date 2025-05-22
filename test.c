
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define N 4
typedef struct 
{   int data;
    int acc;
    bool valid;    
}PE;

void acumlate(PE *pe){
  pe->acc+=pe->acc;
}

int compute(int A, int B){
     return A*B;
}


int main(){
    
    PE pe[N][N];
    int MatrizA[N][N]={{1,2,3},{1,2,3},{1,2,3}};
    int MatrizB[N][N]={{1,2,3},{1,2,3},{1,2,3}};
 for(int x=0; x<2*N-1;x++){
     for(int i=0; i<N;i++){
        for(int j=0; j<N; j++ ){
            int k=x-i;
            
              if (k >= 0 && k < N){
                int data=compute(MatrizA[i][k], MatrizB[k][j]);
                pe[i][j].data=data;
                acumlate(&pe);
              }
           

        }
    
    }


 }
   
    return 0;
}



