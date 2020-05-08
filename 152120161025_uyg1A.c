
#include <stdio.h>

int main(void)
{
    int a, b, c, d,son=0;
 
          
     printf (" Bir sayi giriniz : ");
     scanf  (" %d", &a);
 
         for (b = a+1; b > a; b++)
         {
             d = 1;
             for (c = 2; c < b; c++)
             {
 
                 if (b % c == 0)
                 {
                     d = 0;
 
                     break;
                 }
             }
 
             if (d == 1)
             { son = b; b = 0; }
         }
         printf (" %d \n", son );
 
         for (b = a - 1; b < a; b--)
         {
             d = 1;
             for (c = 2; c < b; c++)
             {
 
                 if (b % c == 0)
                 {
                     d = 0;
 
                     break;
                 }
             }
 
             if (d == 1)
             { son = b; b = a+1; }
         }
         printf (" %d \n", son );


    return 0;
}
