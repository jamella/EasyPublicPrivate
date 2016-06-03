#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>

#define MAX_STR_LEN 1000

long int prime(long int);
long int gcd(long int p, long int q);
int publickey(long int p, long int q, long int* exp, long int* mod);
int privatekey(long int p, long int q, long int pubexp, long int* exp, long int* mod);
int encrypt(long int* inmsg, long int, long int, long int* outmsg);
int decrypt(long int* inmsg, long int, long int, long int* outmsg);
int char2long(char* in, long int* out);
int long2char(long int* in, char* out);

int main(void) {

   long int p,q, pube, pubmod, prive, privmod;
   char inmsg[MAX_STR_LEN];
   long int inmsg_l[MAX_STR_LEN];
   char outmsg[MAX_STR_LEN];
   long int outmsg_l[MAX_STR_LEN];
   char decrmsg[MAX_STR_LEN];
   long int decrmsg_l[MAX_STR_LEN];

   //Get inputs
   // - two prime numbers
   // - a message to be encrypted
   std::cout << "ENTER A PRIME NUMBER" << std::endl;
   std::cin >> p;
   if (prime(p)) 
   {
      std::cerr << p << " is not prime." << std::endl;
      return 1;
   }
   std::cout << "ENTER ANOTHER PRIME NUMBER" << std::endl;
   std::cin >> q;
   if (prime(q)) 
   {
      std::cerr << q << " is not prime." << std::endl;
      return 1;
   }

   std::cout << "ENTER A MESSAGE (up to MAX_STR_LEN characters)." << std::endl;
   std::cin.ignore(INT_MAX,'\n');
   std::cin.getline(inmsg, MAX_STR_LEN);
   std:: cout << inmsg << std::endl;

   long int n = p*q;
   long int t = (p-1)*(q-1);
   
   publickey(p,q,&pube,&pubmod);
   std::cout << "public key: " << pube << ", " << pubmod << std::endl;
   privatekey(p,q,pube,&prive,&privmod);
   std::cout << "private key: " << prive << ", " << privmod << std::endl;

   std::cout << "Original text: " << inmsg << std::endl;
   char2long(inmsg, inmsg_l);
   encrypt(inmsg_l, pube, pubmod, outmsg_l);
   long2char(outmsg_l, outmsg);
   std::cout << "Encrypted text: " << outmsg << std::endl;
   decrypt(outmsg_l, prive, privmod, decrmsg_l);
   long2char(decrmsg_l, decrmsg);
   std::cout << "Decrypted text: " << decrmsg << std::endl;
}


long int prime(long int p) 
//returns zero for prime numbers
{
   long int j = sqrt(p);
   for (long int z=2;z<j;z++) if (0==p%z) return z;
   return 0;
}

int publickey(long int p, long int q, long int *exp, long int *mod)
{

   *mod = (p-1)*(q-1);
   //Find a coprime 
   *exp = ((int)sqrt(*mod)+1293)%2541617;
   while (1!=gcd(*exp,*mod))  
   {
      (*exp)++;
   }
   *exp = 7;
   *mod = p*q;
   return 0;
}

int privatekey(long int p, long int q, long int pubexp, long int *exp, long int *mod)
{
   *mod = (p-1)*(q-1);
   *exp = 1;
   long int tmp=pubexp;
   while(1!=tmp%*mod)
   {
      tmp+=pubexp;
      tmp%=*mod;
      (*exp)++;
      //std::cout << pubexp << "*" << *exp << " % " << *mod << " = " << tmp << std::endl;
   }
   *mod = p*q;
   return 0;
}

int encrypt(long int* in, long int exp, long int mod, long int* out)
{
   //Note: pc is a pointer
   for (long int *pc = in; *pc != '\0';pc++,out++)
   {
      long int c = *pc;
      //std::cout << "c = " <<  c << std::endl;
      for (int z=1;z<exp;z++)
      {
         c *= *pc;
         c %= mod; 
      }
      //std::cout << "c^exp = " <<  c << std::endl;
      //std::cout << "out = " <<  c << std::endl;
      *out = c; 
   }
   *out='\0';
   return 0;
}

int decrypt(long int* in, long int exp, long int mod, long int* out)
{
   for (long int *pc = in; *pc != '\0';pc++,out++)
   {
      long int c = *pc;
      for (int z=1;z<exp;z++)
      {
         c *= *pc;
         c %= mod; 
      }
      //std::cout << "c^exp = " <<  c << std::endl;
      //std::cout << "out = " << c << std::endl;
      *out = c; 
   }
   *out='\0';
   return 0;
}



long int gcd(long int p, long int q) 
//greatest common factor by Euclid's method
{
   if (p<q) {long int tmp=p;p=q;q=tmp;}
   while (q!=0)
   {
      //std::cout << p << " % " << q << " = " << p%q << std::endl;
      long int tmp = q;
      q = p%q;
      p = tmp;
   }

   //std::cout << "gcd = " << p << std::endl; 
   return p;
}

int long2char(long int* in, char* out)
{
   while(*in != 0) *out++ = (char)(*in++);
   *out = '\0';
   return 0;
}

int char2long(char* in, long int* out)
{
   while(*in != '\0') *out++ = (long int)(*in++);
   *out = 0;
   return 0;
}
