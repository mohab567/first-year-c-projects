#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("enter 2 numbers :  ");
    int x,y,b,a,i;
    scanf("%d%d",&x,&y);
    if (x<0)
        a=x*(-1);
    else
    a=x;
    if (y<0)
    b=y*(-1);
    else
    b=y;
    i=b;
    while (((a%i)!=0)||((b%i)!=0))
        {i--;
    }
    printf("GCD(%d\,%d) = %d",x,y,i);


    return 0;
}
