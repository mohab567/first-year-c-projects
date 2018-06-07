#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 200

char result[MAXSIZE];
int i, counter=0;
void convert(long long x,int base)
{


  if(x>0){
        if (x%base<10){
   result[counter]=(x%base)+'0';}
        else
        result[counter]=(x%base)-10+'A';
   counter++;
   convert(x/base,base);

          }
  else {


    for(i=counter-1;i>=0;i--)
    {
      printf("%c",result[i]);
    }
  }
}



int main()
{
    long long x;
    int base;
    printf("Enter the number :\n");
    scanf("%lld",&x);
    printf("Enter the base :\n");
    scanf("%d",&base);
    printf("your coversion answer :\n");
    if (x==0)
    printf("%d",x);
    else if (x<0)
    {
        printf("-");
        convert(-x,base);
    }
    else
    convert(x,base);

    return 0;
}
