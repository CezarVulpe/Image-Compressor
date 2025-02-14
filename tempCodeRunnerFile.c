/**-- test-ab.c --  prelucreaza arbori binari cu chei intregi --*/
#include "tarb.h"
typedef struct matrice
{
	unsigned char rosu, verde, albastru;
} imagine;
void setarenull(TArb arb) 
{
	arb->stj = NULL;
	arb->sts = NULL;
	arb->drj = NULL;
	arb->drs = NULL;
}
void umplerearbore(TArb arb, int x, int y, int nivel, int nivelmax, imagine **matrice, int dimensiune, int factor)
{
	if (nivel == nivelmax + 1)
		return;
	unsigned long long int size = dimensiune;
	for (int i = 1; i < nivel; i++)
		size = size / 2;
	unsigned long long int rosu = 0, verde = 0, albastru = 0, mean = 0;
	for (int i = x; i < size + x; i++)
	{
		for (int j = y; j < size + y; j++)
		{
			rosu = rosu + matrice[i][j].rosu;
			verde = verde + matrice[i][j].verde;
			albastru = albastru + matrice[i][j].albastru;
		}
	}
	rosu = rosu / (size * size);
	verde = verde / (size * size);
	albastru = albastru / (size * size);
	for (int j = x; j < size + x; j++)
	{
		for (int i = y; i < size + y; i++)
		{
			mean = mean + (rosu - matrice[j][i].rosu) * (rosu - matrice[j][i].rosu);
			mean = mean + (albastru - matrice[j][i].albastru) * (albastru - matrice[j][i].albastru);
			mean = mean + (verde - matrice[j][i].verde) * (verde - matrice[j][i].verde);
		}
	}
	mean = mean / (size * size * 3);
	printf("%lld\n", mean);
	if (mean <= (unsigned long long int)factor)
	{
		arb->info = '1';
		arb->rosu = rosu;
		arb->verde = verde;
		arb->albastru = albastru;
	}
	else
	{
		arb->info = '0';
		TArb aux;
		aux = malloc(sizeof(TNod));
		arb->sts = aux;
		setarenull(aux);
		aux = malloc(sizeof(TNod));
		arb->drs = aux;
		setarenull(aux);
		aux = malloc(sizeof(TNod));
		arb->drj = aux;
		setarenull(aux);
		aux = malloc(sizeof(TNod));
		arb->stj = aux;
		setarenull(aux);
		umplerearbore(arb->sts, x, y, nivel + 1, nivelmax, matrice, dimensiune, factor);
		umplerearbore(arb->drs, x, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune, factor);
		umplerearbore(arb->drj, x + size / 2, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune, factor);
		umplerearbore(arb->stj, x + size / 2, y, nivel + 1, nivelmax, matrice, dimensiune, factor);
	}
}
void verificaresubfactor(TArb arb, float factor)
{
	float rosu = 0, verde = 0, albastru = 0;
	rosu = arb->sts->rosu + arb->drs->rosu + arb->drj->rosu + arb->stj->rosu;
	verde = arb->sts->verde + arb->drs->verde + arb->drj->verde + arb->stj->verde;
	albastru = arb->sts->albastru + arb->drs->albastru + arb->drj->rosu + arb->stj->rosu;
}
void nivelmax(TArb arb, int nivel, int *nivelmaxim, int *nrzone, int *patratmare, int nrmaxnoduri, int dimensiune1)
{
	if (arb == NULL)
		return;
	if (nivel > *nivelmaxim)
		*nivelmaxim = nivel;
	if (arb->info == '1')
	{
		int s = dimensiune1;
		for (int i = 1; i < nivel; i++)
		{
			s = s / 2;
		}
		if (*patratmare < s)
			*patratmare = s;
		*nrzone = *nrzone + 1;
	}
	nivelmax(arb->drj, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
	nivelmax(arb->drs, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
	nivelmax(arb->stj, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
	nivelmax(arb->sts, nivel + 1, nivelmaxim, nrzone, patratmare, nrmaxnoduri, dimensiune1);
}

void inserareincoada(TCoada *c, FILE *fout, int dimensiune1)
{
	int nivel = 1;
	TArb arb=NULL;
	fwrite(&dimensiune1, sizeof(int), 1, fout);
	while (c->inc != NULL)
	{
		ExtrQ(c, &arb, &nivel);
		if (arb->info == '1')
		{
			char x = 1;
			fwrite(&x, sizeof(unsigned char), 1, fout);
			fwrite(&arb->rosu, sizeof(unsigned char), 1, fout);
			fwrite(&arb->verde, sizeof(unsigned char), 1, fout);
			fwrite(&arb->albastru, sizeof(unsigned char), 1, fout);
		}
		else
		{
			char x = 0;
			fwrite(&x, sizeof(unsigned char), 1, fout);
			IntrQ(c, arb->sts, nivel + 1);
			IntrQ(c, arb->drs, nivel + 1);
			IntrQ(c, arb->drj, nivel + 1);
			IntrQ(c, arb->stj, nivel + 1);
		}
	}
}

void citirecoadatask3(TCoada *c, FILE *fin)
{
	int nivel = 1;
	unsigned char x;
	TArb arb=NULL, aux=NULL;
	while (c->inc != NULL)
	{
		ExtrQ(c, &arb, &nivel);
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
			fread(&arb->verde, sizeof(unsigned char), 1, fin);
			fread(&arb->albastru, sizeof(unsigned char), 1, fin);
		}
	}
}
void umplerematrice(TArb arb, int x, int y, int nivel, int nivelmax, imagine **matrice, int dimensiune)
{
	if (nivel == nivelmax + 1)
		return;
	unsigned long long int size = dimensiune;
	for (int i = 1; i < nivel; i++)
		size = size / 2;
	unsigned long long int rosu = 0, verde = 0, albastru = 0, mean = 0;
	if (arb->info == '1')
	{
		for (int i = x; i < size + x; i++)
		{
			for (int j = y; j < size + y; j++)
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
		umplerematrice(arb->drs, x+ size / 2, y, nivel + 1, nivelmax, matrice, dimensiune);
		umplerematrice(arb->drj, x + size / 2, y + size / 2, nivel + 1, nivelmax, matrice, dimensiune);
		umplerematrice(arb->stj, x , y+ size / 2, nivel + 1, nivelmax, matrice, dimensiune);
	}
}
void eliberarearbore(TArb arb)
{
	if(arb==NULL) return;
	eliberarearbore(arb->drj);
	eliberarearbore(arb->drs);
	eliberarearbore(arb->stj);
	eliberarearbore(arb->sts);
	free(arb);

}

int main(int argc, char const *argv[])
{
	if (strcmp(argv[1], "-d") == 0)
	{
		FILE *fin, *fout;
		fin = fopen(argv[2], "rb");
		fout = fopen(argv[3], "wb");
		int dimensiune1;
		fread(&dimensiune1, sizeof(int), 1, fin);
		TArb arb = NULL;
		arb = malloc(sizeof(TNod));
		setarenull(arb);
		TCoada *c;
		c = InitQ();
		IntrQ(c, arb, 1);
		citirecoadatask3(c, fin);
		fprintf(fout, "%s\n", "P6");
		fprintf(fout, "%d ", dimensiune1);
		fprintf(fout, "%d\n", dimensiune1);
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
		for (int i = 0; i < dimensiune1; i++)
		{
			matrice[i] = malloc(dimensiune1 * sizeof(imagine));
		}
		umplerematrice(arb, 0, 0, 1, cntnivele, matrice, dimensiune1);
		for (int i = 0; i < dimensiune1; i++)
		{
			for (int j = 0; j < dimensiune1; j++)
			{
				fwrite(&(matrice[j][i].rosu), sizeof(unsigned char), 1, fout);
				fwrite(&(matrice[j][i].verde), sizeof(unsigned char), 1, fout);
				fwrite(&(matrice[j][i].albastru), sizeof(unsigned char), 1, fout);
			}
		}
			fclose(fout);
			fclose(fin);
		for (int i = 0; i < dimensiune1; i++)
		{
			free(matrice[i]);
		}
		while(c->inc!=NULL)
		{
			TLista auxiliar=c->inc;
			c->inc=c->inc->urm;
			free(auxiliar);
		}
		free(c);
		free(matrice);
		eliberarearbore(arb);
	}
	else
	{
		char tipfisier[3];
		int dimensiune1, dimensiune2, saturatiemax;

		TArb arb = NULL;
		arb = malloc(sizeof(TNod));
		arb->stj = NULL;
		arb->sts = NULL;
		arb->drj = NULL;
		arb->drs = NULL;
		FILE *fin, *fout;
		int factor;
		factor = atoi(argv[2]);
		printf("%d", factor);
		fin = fopen(argv[3], "rb");
		fscanf(fin, "%s\n", tipfisier);
		tipfisier[3] = '\0';
		fscanf(fin, "%d ", &dimensiune1);
		fscanf(fin, "%d\n", &dimensiune2);
		fscanf(fin, "%d", &saturatiemax);
		fseek(fin, 1, SEEK_CUR);
		if (strcmp(argv[1], "-c1") == 0)
			fout = fopen(argv[4], "w");
		if (strcmp(argv[1], "-c2") == 0)
			fout = fopen(argv[4], "wb");
		int s = 1, cntnivele = 1;
		while (s < dimensiune1)
		{
			s = s * 2;
			cntnivele++;
		}
		imagine **matrice;
		matrice = malloc(dimensiune1 * sizeof(imagine *));
		for (int i = 0; i < dimensiune1; i++)
		{
			matrice[i] = malloc(dimensiune1 * sizeof(imagine));
		}
		for (int j = 0; j < dimensiune1; j++)
		{
			for (int i = 0; i < dimensiune1; i++)
			{
				fread(&(matrice[j][i].rosu), sizeof(unsigned char), 1, fin);
				fread(&(matrice[j][i].verde), sizeof(unsigned char), 1, fin);
				fread(&(matrice[j][i].albastru), sizeof(unsigned char), 1, fin);
			}
		}
		umplerearbore(arb, 0, 0, 1, cntnivele, matrice, dimensiune1, factor);
		int nivelmaxim = 0, nrzone = 0, patratmare = 0;
		nivelmax(arb, 1, &nivelmaxim, &nrzone, &patratmare, cntnivele, dimensiune1);
		if (strcmp(argv[1], "-c1") == 0)
		{
			fprintf(fout, "%d\n%d\n%d\n", nivelmaxim, nrzone, patratmare);
			fclose(fout);
		}

		TCoada *c= InitQ();
		IntrQ(c, arb, 1);
		if (strcmp(argv[1], "-c2") == 0)
		{
			inserareincoada(c, fout, dimensiune1);
			fclose(fout);
		}
		for (int i = 0; i < dimensiune1; i++)
		{
			free(matrice[i]);
		}
		while(c->inc!=NULL)
		{
			TLista auxiliar=c->inc;
			c->inc=c->inc->urm;
			free(auxiliar);
		}
		free(c);
		free(matrice);
		eliberarearbore(arb);
		fclose(fin);

	}
	return 0;
}
