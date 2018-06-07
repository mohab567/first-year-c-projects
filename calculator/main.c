#include <stdio.h>
#include <stdlib.h>
int main()
{
    char o;
    float num1,num2;
    int num3;
    printf("Enter value to calculate : \n");
    scanf("%f%c%f",&num1,&o,&num2);

    switch(o) {
        case '+':
            printf("%.0f + %.0f = %.0f",num1, num2, num1+num2);
            break;
        case '-':
            printf("%.0f - %.0f = %.0f",num1, num2, num1-num2);
            break;
        case '*':
            printf("%.0f * %.0f = %.0f",num1, num2, num1*num2);
            break;
        case '/':{if (num2==0)
        printf("error divided by zero isnt allawed");
        else
            printf("%.0f / %.0f = %.0f",num1, num2, num1/num2);}
            break;
        case '%':

            if (num2==0)
                printf("error modulus by zero is not allawed");
            else
            {
                num3=(int)num1%(int)num2;
                printf("%.0f %c %.0f = %d",num1,o,num2, num3);}
             break;
        default:
            printf("error : unsupported operator");
            break;
    }
    return 0;
}
