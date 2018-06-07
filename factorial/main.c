#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("enter the number  ");
    int x,i,f;
    f=1;
    scanf("%d",&x);
    i=x;
    if (x<0)
        printf("enter a +ve number next time");
    else{
    while (i>0){
        f=f*i;
        i--;
    }
    printf("its factorial is %d",f);}

    return 0;
}
