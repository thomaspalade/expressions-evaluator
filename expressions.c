#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct arb
{
	int info;
	struct arb *st, *dr;
} TArb, *AArb;

int eroare = 0;
char caractere[13];

typedef struct variabila
{
    char nume[13];
    int val;
} ceva;

int Converteste(char x[12])
{
    int nr = 0, i = 0;
    if (x[0] == '-')
    {
      i = 1;
      while (x[i] != 0)
        nr = nr*10 + (x[i] - '0'), i++;
      nr = -nr;
    }
    else
    {
      i = 0;
      while (x[i] != 0)
        nr = nr*10 + (x[i] - '0'), i++;
    }
    return nr;
}

int E_Numar(char x[12])
{
    int i = 0;
    if (x[1] == 0)
    {
       if (x[0] < '0' || x[0] > '9')
        return 0;
    }
    else
    while (x[i+1] != 0)
    {
        if (!((x[i] - '0' >=0 && x[i] - '0' <= 9) || (x[i] == '-')))
            return 0;
        i++;
    }
    return 1;
}

void Citeste(struct variabila v[10000], int M, FILE *in, FILE *out)
{
    int i = 0;
    char equals[5];
    for (i = 1; i <= M; i++)
    {
        fscanf(in, "%s", v[i].nume);
        fscanf(in, "%s", equals);
        fscanf(in, "%d", &v[i].val);
    }
}

int Gaseste(char x[12], struct variabila v[10000], int M)
{
    int i = 0;
    for (i = 1; i <= M; i++)
        if (strcmp(v[i].nume, x) == 0)
            return i;
    return 0;
}

TArb* Construieste (struct variabila v[10000], int M, FILE *in, FILE *out)
{
    int poz = 0;
    char x[12], rad[12];
    rad[0] = 's'; rad[1] = 'q';
    rad[2] = 'r'; rad[3] = 't';
    rad[4] = 0;
    fscanf(in, "%s", x);

    TArb *aux = (TArb *)malloc(sizeof(TArb));

    if (strcmp(x, rad) == 0)
	{
	   aux->info = -99999999;
	   aux->st = Construieste(v, M, in, out);   /// pun informatia doar in fiul stang
	   aux->dr = NULL;                          /// fiul drept va primi NULL
    }
    else if (x[0] == '+')
    {
	   aux->info = -99999998;
      	   aux->st = Construieste(v, M, in, out); /// membrul stang al adunarii
	   aux->dr = Construieste(v, M, in, out); /// membrul drept al adunarii
    }
    else if (x[0] == '-' && x[1] == 0)
    {
	   aux->info = -99999997;
           aux->st = Construieste(v, M, in, out); /// membrul stang al scaderii
	   aux->dr = Construieste(v, M, in, out); /// membrul drept al scaderii
    }
   else if (x[0] == '*')
	{
	   aux->info = -99999996;
       	   aux->st = Construieste(v, M, in, out); /// membrul stang al inmultirii
	   aux->dr = Construieste(v, M, in, out); /// membrul drept al inmultirii
    }
   else if (x[0] == '/')
 	{
	   aux->info = -99999995;
           aux->st = Construieste(v, M, in, out); /// membrul stang al impartirii
	   aux->dr = Construieste(v, M, in, out); /// membrul drept al impartirii
	}
   else if (x[0] == '^')
 	{
	   aux->info = -99999994;
           aux->st = Construieste(v, M, in, out); /// membrul stang al rid la putere
	   aux->dr = Construieste(v, M, in, out); /// membrul drept al rid la putere
	}
   else
    if (Gaseste(x, v, M) != 0)
   {
           poz = Gaseste(x, v, M);
	   aux->info = v[poz].val;
	   aux->st = NULL;
	   aux->dr = NULL;
   }
   else
   {
       if (E_Numar(x) == 1)
      {
	   aux->info = Converteste(x);
	   aux->st = NULL;
	   aux->dr = NULL;
      }
      if (E_Numar(x) == 0)
      {
           aux->info = -100000001;
           strcpy(caractere, x);
           aux->st = NULL;
	   aux->dr = NULL;
      }
   }

   return aux;
}

int Evalueaza(TArb *arb, FILE *out)
{
    int i, val1, val2, val3;
    if (arb == NULL) return 0;   /// am ajuns la fii unei frunze (care nu exista)
    if (arb->st != NULL)
    {
        if (Evalueaza(arb->st, out) == -100000000)  /// avem o eroare, calcul imposibil
        return -100000000;
        if (Evalueaza(arb->st, out) == -100000001)  /// avem o eroare, variabila nedeclarata
        return -100000001;
    }
    if (arb->dr != NULL)
    {
        if (Evalueaza(arb->dr, out) == -100000000)  /// avem o eroare, calcul imposibil
        return -100000000;
        if (Evalueaza(arb->dr, out) == -100000001)  /// avem o eroare, variabila nedeclarata
        return -100000001;
    }
    if (arb->info == -100000001)
        return -100000001;
    else if (arb->info == -99999998)
        return Evalueaza(arb->st, out) + Evalueaza(arb->dr, out);
    else if (arb->info == -99999997)
        return Evalueaza(arb->st, out) - Evalueaza(arb->dr, out);
    else if (arb->info == -99999996)
        return Evalueaza(arb->st, out) * Evalueaza(arb->dr, out);
    else if (arb->info == -99999995)
    {
        if (Evalueaza(arb->dr, out) == 0)
          return -100000000;
        return Evalueaza(arb->st, out) / Evalueaza(arb->dr, out);
    }
    else if (arb->info == -99999994)
        {
	     /// ridicam la putere manual, nu cu functia pow
             val1 = Evalueaza(arb->st, out);
             val2 = Evalueaza(arb->dr, out);
             val3 = 1;
             for (i = 1; i <= val2; i++)
                 val3 = val3 * val1;
             return val3;
        }
    else if (arb->info == -99999999)
    {
        if (Evalueaza(arb->st, out) < 0)
         return -100000000;
        return sqrt(Evalueaza(arb->st, out));

    }
    else
        return arb->info; /// e frunza, returnez doar propria valoare
}

int main (int argc, char* argv[])
{
    int z;
    int M, N, i;
    struct variabila v[10000];
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    fscanf(in, "%d", &M);
    Citeste(v, M, in, out);
    fscanf(in, "%d", &N);

    for (i = 1; i <= N; i++)
  {
    TArb *L = NULL;
    eroare = 0;
    L = Construieste(v, M, in, out);    //// construiesc arborele necesar
    if (Evalueaza(L, out) == -100000001)
        fprintf(out, "Variable %s undeclared\n", caractere);
    else if (Evalueaza(L, out) == -100000000)
        fprintf(out, "Invalid expression\n");
    else
       {
         z = Evalueaza(L, out);
         fprintf(out, "%d\n", z);
       }
  }
    return 0;
}
