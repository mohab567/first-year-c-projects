#include <stdio.h>
#include <stdlib.h>

int main()
{
    #define PI 3.14159
    float r,d,c,a;
    printf("enter circle's radius ");
    scanf("%f",&r);
    d=2*r;
    c=d*PI;
    a=PI*r*r;
    printf("circle diameter = %f\ncircle's circumference = %f\ncircle's area = %f",d,c,a);
    return 0;
}
