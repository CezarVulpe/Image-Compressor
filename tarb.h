//Vulpe Cezar-Andrei-315CB
/*--- tarb.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ARBORE_BINAR

typedef struct nod {
	unsigned char info;
	unsigned char rosu,verde,albastru; 
	struct nod *sts, *drs, *stj,*drj;
} TNod, *TArb;

/*-- Operatii elementare --*/
TArb InitA();           /* initializare arbore */
TArb ConstrNod(int x, TArb s, TArb d);  /* -> adresa nod (cu info x, fiu stanga s, fiu dreapta d)
sau NULL daca nu exista spatiu */
TArb ConstrFr(int x); /* -> adresa frunza cu informatia x, sau NULL daca nu exista spatiu */
TArb ConstrAA (int lim, int nn, int v1, int v2);
void DistrArb(TArb*);    /* distruge arbore */
int NrNoduri(TArb);     /* numar noduri */

typedef struct Celula{
    struct Celula *urm; /* adresa urmatoarei celule */
	TArb arb;
	int nivel;
} TCelula, *TLista;
/* TCelula este un o variabila care retine 'info' si 'urm'
   TLista este un pointer catre tipul TCelula 
*/

typedef struct coada
{ 
  TLista inc, sf;       /* adresa primei si ultimei celule */
} TCoada;
/* coada este reprezentata ca Lista DESCHISA --*/

TCoada* InitQ () ;
int ExtrQ(TCoada*c, TArb *arb, int *nivel);
int IntrQ(TCoada *c, TArb arb, int nivel);
void DistrQ(TCoada **c);
/*-- Operatii suplimentare --*/
void AfiArb(TArb r); /*- afiseaza arborele r -*/