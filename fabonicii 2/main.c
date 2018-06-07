#include <stdio.h>
#include <stdlib.h>
long long fibonacci(int n)
{
    long long fib = 0;
    if (n ==2)     fib= 1;
    else if(n>2)     fib = fibonacci(n-1) + fibonacci(n-2);
    return fib;
}
int main()
{
    int n;

    scanf("%d",&n);
    if(n==0)
    printf("error");
    else
    printf("%lld",fibonacci(n));
    return 0;
}
