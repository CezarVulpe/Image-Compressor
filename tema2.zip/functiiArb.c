#include "tarb.h"
/*-- TcoadaLD1.c -- coada este reprezentata ca Lista DESCHISA
                 -- Operatii de baza --*/

TCoada* InitQ ()  /* creeaza coada vida cu elemente de dimensiune d;
				   anumite implementari pot necesita si alti parametri */
{ 
  TCoada* c;          /* spatiu pentru descriptor coada */
  c = (TCoada*)malloc(sizeof(TCoada));
  if ( ! c ) return NULL;                  /* nu exista spatiu -> "esec" */

  c->inc = c->sf = NULL;
  return c;          /* intoarce adresa descriptorului cozii */
}

int IntrQ(TCoada *c, TArb arb, int nivel)  /* adauga element la sfarsitul cozii */
{ 
  TLista aux;
  aux = (TLista)malloc(sizeof(TCelula));      /* aloca o noua celula */
  if ( ! aux) return 0;             /* alocare imposibila -> "esec" */
  aux->arb=arb;
  aux->nivel=nivel;
  aux->urm=NULL;
  if (c->sf != NULL)          /* coada nevida */
    c->sf->urm = aux;                   /* -> leaga celula dupa ultima din coada */
  else                              /* coada vida */
    c->inc = aux;                    /* -> noua celula se afla la inceputul cozii */
  c->sf = aux;  	            /* actualizeaza sfarsitul cozii */
  return 1;                         /* operatie reusita -> "succes" */
}


int ExtrQ(TCoada *c, TArb *arb, int *nivel)  /* extrage primul element din coada la adresa ae */
{ 
  if (c->inc==NULL) return 0; /* verific daca am elemente in coada si returnez ca nu s-a realizat extragerea */
  TLista aux;
  aux=c->inc;
  *arb = aux->arb;/* pastrez o copie a varfului pentru a putea dezaloca (mut varful pe urmatoarea celula si se pierde adresa varfului initial) */
  *nivel=aux->nivel;
  if(c->sf==aux)  /* verific daca mai am un singur element(in acest caz, fac coada vida) */
  {
    c->inc=NULL; 
    c->sf=NULL;
  }
  else
  {
    c->inc=c->inc->urm; /* trec la urmatorul element din coada */
  }
  free(aux);
  return 1;                      
}

void DistrQ(TCoada **c) /* distruge coada */
{
  TLista p, aux;
  p = (*c)->inc;
  while(p)
  {
    aux = p;
    p = p->urm;
    free(aux);
  }
  free(*c);
  *c = NULL;
}
