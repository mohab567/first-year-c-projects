#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n,m,l,i,j,s;
    printf("enter n m l : ");
    scanf("%d%d%d",&n,&m,&l);
    if(m<1||n<1||l<1)
        printf("error");
    else{
    int a[n][m],b[m][l],c[n][l];
    printf("enter first matrix :\n");
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            scanf("%d",&a[i][j]);
        }
    }
    printf("enter second matrix :\n");
    for(i=0;i<m;i++){
        for(j=0;j<l;j++){
            scanf("%d",&b[i][j]);
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<l;j++){c[i][j]=0;
            for(s=0;s<m;s++){
                c[i][j]+=(a[i][s]*b[s][j]);
            }
        }
    }
    printf("\n");
     for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%d   ",a[i][j]);

        }
        printf("\n");}
        printf("x\n\n");
     for(i=0;i<m;i++){
        for(j=0;j<l;j++){
            printf("%d   ",b[i][j]);

        }
        printf("\n");}
        printf("=\n");
    for(i=0;i<n;i++){
        for(j=0;j<l;j++){
            printf("%d   ",c[i][j]);

        }
        printf("\n");}

    }
    return 0;
}
