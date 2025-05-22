#include <stdlib.h>
#include <stdio.h>

int **result(int matrixA[3][3], int matrixB[3][3],int n){
    int** matrixC = malloc(n * sizeof(int*));
    for(int i=0; i<n; i++){
        matrixC[i] = malloc(n * sizeof(int));
        for(int j=0; j<n; j++){ 
            matrixC[i][j] = 0; // Initialize

          
              for(int y=0; y<n; y++ ){
                matrixC[i][j]+=matrixA[i][y]*matrixB[y][j];
              }
              
           
         
        }
    }

    return matrixC;

}
int main(){
    int matA[3][3]={{1,3,4},{1,3,5},{3,5,6}};
    int matB[3][3]={{1,3,4},{1,3,5},{3,5,6}};
    int **mat;

    
    mat=result(matA,matB,3);
    for(int i=0; i<3; i++ ){
        for(int j=0; j<3; j++ ){
            printf("%d ",mat[i][j]);
            if(j==2) printf("\n");
          }
       
              }
    return 0;
}