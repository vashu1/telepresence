#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Packet {
  uint8_t startMarker;
  uint8_t dataLen;
  uint8_t dataChksum;
  uint8_t data[];
};

 #include <string.h>

 /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  

void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  


int main()
{
struct Packet p;
char s[100];
itoa(sizeof(p), s);
printf(s);
printf("\n");
   return 0;
}

