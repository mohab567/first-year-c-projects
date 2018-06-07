#include <stdio.h>
#include <stdlib.h>
int main()
{
    int mat[100][3];
    int n=0,x=0,y=0,z=0,i,j;
     while(n<1||n>100){
      printf("please enter number of forces  ");
      scanf("%d",&n);}
     printf("please Enter forces in form of (x y z)\n");
    for(i=0;i<n;i++){
        for(j=0;j<3;j++){
            scanf("%d",&mat[i][j]);
    }}
    for(i=0;i<n;i++){
       x=x+mat[i][0];
    }
    for(i=0;i<n;i++){
       y=y+mat[i][1];
    }
    for(i=0;i<n;i++){
       z=z+mat[i][2];
    }
    if(x==0&&y==0&&z==0)
        printf("YES");
    else
        printf("NO");
    return 0;
}
