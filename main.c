//Vulpe Cezar-Andrei-315CB
#include "tarb.h"
typedef struct matrice
{
	unsigned char rosu, verde, albastru;
} imagine; // aceasta este matricea de pixeli
void setarenull(TArb arb) //functie pentru a initializa vectorii arb cu urmasii NULL
{
	arb->stj = NULL;
	arb->sts = NULL;
	arb->drj = NULL;
	arb->drs = NULL;
}
void umplerearbore(TArb arb, int x, int y, int nivel, int nivelmax, imagine **matrice, int dimensiune, int factor) // aceasta functie are rolul de lua informatia din matrice si de a o pune in arbore
{ //x si y reprezinta coordonatele lui arb in matricea de pixeli
	int i=1,j=1;
	if (nivel == nivelmax + 1)
		return; //conditia de oprire a recursivitatii
	unsigned long long int size = dimensiune;
	for ( i = 1; i < nivel; i++)
		size = size / 2; //aflu size-ul unei portiuni pe baza nivelului nodului( nivelul incepe cu numaratoarea de la 1)
	unsigned long long int rosu = 0, verde = 0, albastru = 0, mean = 0; // aici voi stoca valorile medii pentru saturatiea fiecarei culori
	for ( i = x; i < size + x; i++)
	{
		for ( j = y; j < size + y; j++)
		{
			rosu = rosu + matrice[i][j].rosu;  //for in for pentru a face suma saturatiilor pe o portiune de matrice
			verde = verde + matrice[i][j].verde;
			albastru = albastru + matrice[i][j].albastru;
		}
	}
	rosu = rosu / (size * size);
	verde = verde / (size * size); // calculul valorilor medii
	albastru = albastru / (size * size);
	for ( j = x; j < size + x; j++)
	{
		for ( i = y; i < size + y; i++)
		{
			mean = mean + (rosu - matrice[j][i].rosu) * (rosu - matrice[j][i].rosu);
			mean = mean + (albastru - matrice[j][i].albastru) * (albastru - matrice[j][i].albastru); 
			mean = mean + (verde - matrice[j][i].verde) * (verde - matrice[j][i].verde);
		} // suma patratelor erorilor 
	}
	mean = mean / (size * size * 3); // eroarea medie
	if (mean <= (unsigned long long int)factor) //verificam daca ne aflam sau nu in range-ul factorului
	{
		arb->info = '1';
		arb->rosu = rosu;   //daca ne aflam in range, setam nodul ca frunza
		arb->verde = verde;
		arb->albastru = albastru;
	}
	else
	{
		arb->info = '0';
		TArb aux;
		aux = malloc(sizeof(TNod));
		arb->sts = aux;
		setarenull(aux); //in acest caz se continua introducerea de noduri noi in arbore
		aux = malloc(sizeof(TNod)); // se aloca celule noi, sunt satati urmasii celulelor noi la null
		arb->drs = aux; //se realizeaza legaturile 
		setarenull(aux);
		aux = malloc(sizeof(TNod));
		arb->drj = aux;
		setarenull(aux);
		aux = malloc(sizeof(TNod));
		arb->stj = aux;
		setarenull(aux);
		umplerearbore(arb->sts, x, y, nivel + 1, nivelmax, matrice, dimensiune, factor); //apelul pentru recursivitate in care actualizam coordonatele nodului in matrice
		umplerearbore(arb->drs, x, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune, factor);
		umplerearbore(arb->drj, x + size / 2, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune, factor);
		umplerearbore(arb->stj, x + size / 2, y, nivel + 1, nivelmax, matrice, dimensiune, factor);
	}
}

void nivelmax(TArb arb, int nivel, int *nivelmaxim, int *nrzone, int *patratmare, int nrmaxnoduri, int dimensiune1) 
{ //aceasta este functia care rezolva taskul 1
	int i=1;
	if (arb == NULL)
		return;
	if (nivel > *nivelmaxim)
		*nivelmaxim = nivel;
	if (arb->info == '1')
	{
		int s = dimensiune1;
		for ( i = 1; i < nivel; i++)
		{
			s = s / 2;
		} // s reprezinta latura reprez pt nodul in cauza
		if (*patratmare < s) //folosim pointer pentru latura deorece parcurgem tot arborele si actualizam cu cea mai mare latura
			*patratmare = s;
		*nrzone = *nrzone + 1; // cand gasim o frunza noua, actulizam numarul de regiuni
	}
	nivelmax(arb->drj, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
	nivelmax(arb->drs, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1); //se realizeaza recursivitatea pentru a parcurge arborele
	nivelmax(arb->stj, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
	nivelmax(arb->sts, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
}

void inserareincoada(TCoada *c, FILE *fout, int dimensiune1) //aceasta este functia pentru realizarea cozii ce are ca rezultat parcurgerea pe nivel
{
	int nivel = 1;
	TArb arb = NULL;
	fwrite(&dimensiune1, sizeof(int), 1, fout); //scriem dimensiunea in fisierul binar
	while (c->inc != NULL) //cat timp mai putem lua arb din coada( este nevida coada)
	{
		ExtrQ(c, &arb, &nivel);
		if (arb->info == '1')
		{
			char x = 1;
			fwrite(&x, sizeof(unsigned char), 1, fout);
			fwrite(&arb->rosu, sizeof(unsigned char), 1, fout); //daca e frunza, scriem si informatii
			fwrite(&arb->verde, sizeof(unsigned char), 1, fout);
			fwrite(&arb->albastru, sizeof(unsigned char), 1, fout);
		}
		else
		{
			char x = 0;
			fwrite(&x, sizeof(unsigned char), 1, fout);
			IntrQ(c, arb->sts, nivel + 1);
			IntrQ(c, arb->drs, nivel + 1); //daca nu e frunza, introducem urmasii in coada(intr un final se va ajunge la toate frunzele ce reprez culori)
			IntrQ(c, arb->drj, nivel + 1);
			IntrQ(c, arb->stj, nivel + 1);
		}
	}
} //in acelasi timp se realizeaza si crearea cozii, dar si afisarea in fisier

void citirecoadatask3(TCoada *c, FILE *fin) //aceasta functie are rolul de a umple coada cu ajutorul fisierului binar
{
	int nivel = 1;
	unsigned char x;
	TArb arb = NULL, aux = NULL;
	while (c->inc != NULL)
	{
		ExtrQ(c, &arb, &nivel); //luam un nod din coada si daca nu este frunza, ii introducem in coada urmasii
		fread(&x, sizeof(unsigned char), 1, fin);
		if (x == 0)
		{
			arb->info = '0';
			aux = malloc(sizeof(TNod));
			arb->sts = aux;
			setarenull(aux);
			IntrQ(c, aux, nivel);
			aux = malloc(sizeof(TNod));
			arb->drs = aux;
			setarenull(aux);
			IntrQ(c, aux, nivel);
			aux = malloc(sizeof(TNod));
			arb->drj = aux;
			setarenull(aux);
			IntrQ(c, aux, nivel);
			aux = malloc(sizeof(TNod));
			arb->stj = aux;
			setarenull(aux);
			IntrQ(c, aux, nivel);
		}
		else
		{
			arb->info = '1';
			fread(&arb->rosu, sizeof(unsigned char), 1, fin);
			fread(&arb->verde, sizeof(unsigned char), 1, fin); //daca este frunza, setam informatiile
			fread(&arb->albastru, sizeof(unsigned char), 1, fin);
		}
	}
}
void umplerematrice(TArb arb, int x, int y, int nivel, int nivelmax, imagine **matrice, int dimensiune) //aici se realizeaza matricea de pixeli ce reprez poza
{
	int i=1,j=1;
	if (nivel == nivelmax + 1)
		return;
	unsigned long long int size = dimensiune;
	for ( i = 1; i < nivel; i++)
		size = size / 2;
	unsigned long long int rosu = 0, verde = 0, albastru = 0, mean = 0;
	if (arb->info == '1')
	{
		for ( i = x; i < size + x; i++)
		{
			for ( j = y; j < size + y; j++)
			{
				matrice[i][j].rosu = arb->rosu;
				matrice[i][j].verde = arb->verde;
				matrice[i][j].albastru = arb->albastru;
			}
		}
	}
	else
	{
		umplerematrice(arb->sts, x, y, nivel + 1, nivelmax, matrice, dimensiune);
		umplerematrice(arb->drs, x + size / 2, y, nivel + 1, nivelmax, matrice, dimensiune);
		umplerematrice(arb->drj, x + size / 2, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune);
		umplerematrice(arb->stj, x, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune);
	}
}
void eliberarearbore(TArb arb) //functie pentru a eliberea o parte din memorie
{
	if (arb == NULL)
		return;
	eliberarearbore(arb->drj);
	eliberarearbore(arb->drs);
	eliberarearbore(arb->stj);
	eliberarearbore(arb->sts);
	free(arb); 
}

int main(int argc, char const *argv[])
{
	int i=1,j=1;
	if (strcmp(argv[1], "-d") == 0)  //verificam daca ne aflam in task ul 3
	{
		FILE *fin, *fout;
		fin = fopen(argv[2], "rb");
		fout = fopen(argv[3], "wb"); //deschidem fisierele
		int dimensiune1;
		fread(&dimensiune1, sizeof(int), 1, fin); //citesc fimensiunea(de 2 ori, desi se stie ca e patrat)
		TArb arb = NULL;
		arb = malloc(sizeof(TNod)); //se creeaza primul nod si este pus in coada
		setarenull(arb);
		TCoada *c;
		c = InitQ();
		IntrQ(c, arb, 1);
		citirecoadatask3(c, fin); //aici incepe prelucrarea cozii(adaugare,scoatere si afisare de elemente)
		fprintf(fout, "%s\n", "P6");
		fprintf(fout, "%d ", dimensiune1);
		fprintf(fout, "%d\n", dimensiune1); //aici incepe afisarea fisierului de tip P6
		int saturatiemax = 255;
		fprintf(fout, "%d\n", saturatiemax);
		int s = 1, cntnivele = 1;
		while (s < dimensiune1)
		{
			s = s * 2;
			cntnivele++;
		}
		imagine **matrice;
		matrice = malloc(dimensiune1 * sizeof(imagine *));
		for ( i = 0; i < dimensiune1; i++)
		{
			matrice[i] = malloc(dimensiune1 * sizeof(imagine));
		}
		umplerematrice(arb, 0, 0, 1, cntnivele, matrice, dimensiune1);
		for ( i = 0; i < dimensiune1; i++)
		{
			for ( j = 0; j < dimensiune1; j++)
			{
				fwrite(&(matrice[j][i].rosu), sizeof(unsigned char), 1, fout);
				fwrite(&(matrice[j][i].verde), sizeof(unsigned char), 1, fout); //afisarea matricii
				fwrite(&(matrice[j][i].albastru), sizeof(unsigned char), 1, fout);
			}
		}
		fclose(fout); //inchidere fisiere si dezalocare memorie
		fclose(fin);
		for ( i = 0; i < dimensiune1; i++)
		{
			free(matrice[i]);
		}
		while (c->inc != NULL)
		{
			TLista auxiliar = c->inc;
			c->inc = c->inc->urm;
			free(auxiliar);
		}
		free(c);
		free(matrice);
		eliberarearbore(arb);
	}
	else //aici ne aflam pe task ul 1 sau 2
	{
		char tipfisier[3];
		int dimensiune1, dimensiune2, saturatiemax;

		TArb arb = NULL;
		arb = malloc(sizeof(TNod));
		arb->stj = NULL;
		arb->sts = NULL;
		arb->drj = NULL;
		arb->drs = NULL; //se seteaza primul nod
		FILE *fin, *fout;
		int factor;
		factor = atoi(argv[2]);
		printf("%d", factor);
		fin = fopen(argv[3], "rb");
		fscanf(fin, "%s\n", tipfisier);
		tipfisier[3] = '\0';
		fscanf(fin, "%d ", &dimensiune1);
		fscanf(fin, "%d\n", &dimensiune2);
		fscanf(fin, "%d", &saturatiemax); //se citeste din fisierul P6
		fseek(fin, 1, SEEK_CUR);
		if (strcmp(argv[1], "-c1") == 0)
			fout = fopen(argv[4], "w");
		if (strcmp(argv[1], "-c2") == 0) //setam pointerul pt out in functie de task
			fout = fopen(argv[4], "wb");
		int s = 1, cntnivele = 1;
		while (s < dimensiune1)
		{
			s = s * 2;
			cntnivele++;
		}
		imagine **matrice;
		matrice = malloc(dimensiune1 * sizeof(imagine *)); //alocam memoria pentru matricea de pixeli
		for ( i = 0; i < dimensiune1; i++)
		{
			matrice[i] = malloc(dimensiune1 * sizeof(imagine));
		}
		for ( j = 0; j < dimensiune1; j++)
		{
			for ( i = 0; i < dimensiune1; i++)
			{
				fread(&(matrice[j][i].rosu), sizeof(unsigned char), 1, fin);
				fread(&(matrice[j][i].verde), sizeof(unsigned char), 1, fin);  //umplem matricea cu info din fisier
				fread(&(matrice[j][i].albastru), sizeof(unsigned char), 1, fin);
			}
		}
		umplerearbore(arb, 0, 0, 1, cntnivele, matrice, dimensiune1, factor); //incepe prelucrarea arborelui(umplerea lui)
		int nivelmaxim = 0, nrzone = 0, patratmare = 0;
		nivelmax(arb, 1, &nivelmaxim, &nrzone, &patratmare, cntnivele, dimensiune1);
		if (strcmp(argv[1], "-c1") == 0)
		{
			fprintf(fout, "%d\n%d\n%d\n", nivelmaxim, nrzone, patratmare); //afisam ce trebuie pentru taskul 1
			fclose(fout);
		}

		TCoada *c = InitQ();
		IntrQ(c, arb, 1);
		if (strcmp(argv[1], "-c2") == 0)
		{
			inserareincoada(c, fout, dimensiune1);  //lucram cu coada si afisam ce trebuie pentru taskul 2
			fclose(fout);
		}
		for ( i = 0; i < dimensiune1; i++)
		{
			free(matrice[i]);
		}
		while (c->inc != NULL)  //elibeream memoria si inchidem fisierele
		{
			TLista auxiliar = c->inc;
			c->inc = c->inc->urm;
			free(auxiliar);
		}
		free(c);
		free(matrice);
		eliberarearbore(arb);
		fclose(fin);
	}
	return 0;
}
