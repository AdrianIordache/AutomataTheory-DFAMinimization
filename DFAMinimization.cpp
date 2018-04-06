#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#define N 20

using namespace std;

inline int codifica(char c, int n, char *v)
{
	for(int i = 0; i < n; ++i)
		if(v[i] == c ) return i;
	return 0;
}

inline char decodifica(int c, char* v)
{
	return v[c];
}

inline char* decodifica(int c, char** v)
{
	return v[c];
}

int main()
{
	char s[] = "minimization1.in";
	ifstream fin(s);
	int nrStari, nrAlfabet, nrTranzitii, nrFinal;

	fin >> nrStari;
	char *stari = new char[nrStari];
	for(int i = 0; i < nrStari; ++i)
		fin >> stari[i];

	cout<<"Starile automatului sunt: \n";
	for(int i = 0; i < nrStari; ++i)
		cout<< stari[i] <<' ';
	cout<<'\n';

	fin >> nrAlfabet;
	char *alfabet = new char[nrAlfabet];
	for(int i = 0; i < nrAlfabet; ++i)
		fin >> alfabet[i];

	cout<<"Alfabetul automatului este: \n";
	for(int i = 0; i < nrAlfabet; ++i)
		cout<< alfabet[i] <<' ';
	cout<<'\n';

	fin >> nrTranzitii;
	if(nrTranzitii != nrStari * nrAlfabet) 
		{ cout<<"Automatul nu este valid\n"; return 0;}

	int** tranzitieInt = new int* [nrStari];
	for(int i = 0 ; i < nrStari; ++i )
		tranzitieInt[i] = new int [nrAlfabet];

	char** tranzitieChar = new char* [nrStari];
	for(int i = 0 ; i < nrStari; ++i )
		tranzitieChar[i] = new char [nrAlfabet];

	char stareInitiala, tranzitie, stareFinala;
	for(int i = 0; i < nrTranzitii; ++i)
	{
		fin >> stareInitiala >> tranzitie >> stareFinala;
		int k = codifica(stareInitiala, nrStari, stari);
		int t = codifica(tranzitie, nrAlfabet, alfabet);

		tranzitieChar[k][t] = stareFinala;
		tranzitieInt[k][t]  = codifica(stareFinala, nrStari, stari);
	}

	cout<<"Matricea functiilor de tranzitie codificata\n";
	for(int i = 0; i < nrStari; ++i)
	{
		cout<<i<<": ";
		for(int j = 0; j < nrAlfabet; ++j)
			cout<<tranzitieInt[i][j]<<' ';
		cout<<'\n';
	}

	cout<<"Matricea functiilor de tranzitie decodificata\n";
	for(int i = 0; i < nrStari; ++i)
	{
		cout<<decodifica(i, stari)<<": ";
		for(int j = 0; j < nrAlfabet; ++j)
			cout<<tranzitieChar[i][j]<<' ';
		cout<<'\n';
	}

	cout<<"Vectorul de booli:\n";
	bool *Final = new bool[nrStari]; char aux;
	for(int i = 0; i < nrStari; ++i)
	{
		fin >> aux;
		if(aux == 'f') Final[i] = false;
					else Final[i] = true;
	}

	for(int i = 0; i < nrStari; ++i)
	{
		if(Final[i] == true) cout<<'1'<<' ';
					else cout<<'0'<<' ';
	}
	cout<<'\n';

	bool **matrice = new bool* [nrStari];
	for(int i = 1; i < nrStari; ++i)
		matrice[i] = new bool [i];

	for(int i = 1; i < nrStari; ++i)
		for(int j = 0; j < i; ++j)
		{
			//cout<<decodifica(i, stari)<<' '<<decodifica(j, stari)<<'\n';
			if((Final[i] == true && Final[j] == false) || (Final[i] == false && Final[j] == true))
				matrice[i][j] = true;
			else
				matrice[i][j] = false;
				
		}
	cout<<"Generare Matrice Myhill Nerode initial marcata\n";
	for(int i = 1; i < nrStari; ++i)
	{
		for(int j = 0; j < i; ++j)
			if(matrice[i][j] == true) cout<<'X'<<' ';
							else cout<<'O'<<' ';
		cout<<'\n';
	}
 	
 	cout<<"Proces marcare perechi matrice\n";
	while(true)
	{
		int nrMarcari = 0;
		for(int i = 1; i < nrStari; ++i)
			for(int j = 0; j < i; ++j)
				if(matrice[i][j] == false)
			{
				//char X = decodifica(i, stari);
				//char Y = decodifica(j, stari);
				//cout<<X<<' '<<Y<<'\n';
				for(int k = 0; k < nrAlfabet; ++k)
				{
					char X = decodifica(i, stari);
					char Y = decodifica(j, stari);
					//cout<<X<<' '<<Y<<" -> ";
					//cout<<decodifica(tranzitieInt[i][k], stari)<<' '<<decodifica(tranzitieInt[j][k], stari)<<'\n';
					int t, s;
					t = tranzitieInt[i][k];
					s = tranzitieInt[j][k];
					if(matrice[t][s] == true) {matrice[i][j] = true; nrMarcari++;}
				}
			}
		if(nrMarcari == 0) break;
	}

	cout<<"Matricea dupa terminarea marcarilor\n";
	for(int i = 1; i < nrStari; ++i)
	{
		for(int j = 0; j < i; ++j)
			if(matrice[i][j] == true) cout<<'X'<<' ';
							else cout<<'O'<<' ';
		cout<<'\n';
	}

	char **automatMin = new char* [nrStari];
	for(int i = 0; i < nrStari; ++i)
		automatMin[i] = new char [nrStari];

	int nemarcate = 0;
	for(int i = 1; i < nrStari; ++i)
		for(int j = 0; j < i; ++j)
			if(matrice[i][j] == false)
			{	
				int k = 0;
				automatMin[nemarcate][k] = decodifica(i, stari); k++;
				automatMin[nemarcate][k] = decodifica(j, stari); k++;
				automatMin[nemarcate][k] = '\0';
				nemarcate++;
			}

	cout<<"Elementele nemarcate din Matricea Myhill Nerode ce vor forma noile stari\n";
	for(int i = 0; i < nemarcate; ++i)
		cout<<automatMin[i]<<' ';
	cout<<'\n';

	cout<<"Colorare stari in functie de componentele din care apartin\n";
	int* culoare = new int [nemarcate];
	for(int i = 0; i < nemarcate; ++i)
		culoare[i] = i;

	for(int i = 0; i < nemarcate - 1; ++i)
		for(int j = i + 1; j < nemarcate; ++j)
			for(int k = 0; k < strlen(automatMin[j]); ++k)
				if(strchr(automatMin[i], automatMin[j][k]) != 0) culoare[j] = culoare[i];

	/*for(int i = 0; i < nemarcate; ++i)
		cout<<culoare[i]<<' ';
	cout<<'\n';*/

	int maxCuloare = (1<<31);
	for(int i = 0; i < nemarcate; ++i)
		if (culoare[i] > maxCuloare) maxCuloare = culoare[i];

	int* stariAutomat = new int [nrStari];
	for(int i = 0; i < nemarcate; ++i)
	{
		for(int j = 0; j < nrStari; ++j)
			{
				//cout<<automatMin[i]<<' '<<stari[j]<<' '<<culoare[i]<<'\n';
				if(strchr(automatMin[i], stari[j]) != 0) {stariAutomat[j] = culoare[i];}
			}
	}

	char *auxiliar = new char[nrStari*nrStari];
	for(int i = 0; i < nemarcate; ++i)
		{
			if(i == 0) strcpy(auxiliar, automatMin[i]);
			else
				strcat(auxiliar,automatMin[i]);
		}
	
	for(int i = 0; i < nrStari; ++i)
		if(strchr(auxiliar, stari[i]) == 0) 
			{ maxCuloare++;  stariAutomat[i] = maxCuloare;}

	for(int i = 0; i < nrStari; ++i)
		cout<<stariAutomat[i]<<' ';
	cout<<'\n'; 

	char **componente = new char* [maxCuloare + 1];
	for(int i = 0; i <= maxCuloare ; ++i)
		componente[i] = new char [nrStari];

	for(int i = 0; i <= maxCuloare; ++i)
	{
		int k = 0;
		for(int j = 0; j < nrStari; ++j)
			{
				if(stariAutomat[j] == i) 
					{ componente[i][k] = decodifica(j, stari); k++; }
			}
	}

	cout<<"Componentele noului automat vor fi:\n";
	for(int i = 0; i <= maxCuloare; ++i)
		cout<<componente[i]<<' ';
	cout<<'\n';
	
	int** tranzitieFinal = new int* [maxCuloare + 1];
	for(int i = 0; i < maxCuloare + 1; ++i)
		tranzitieFinal[i] = new int [nrAlfabet];

	for(int i = 0; i < maxCuloare + 1; ++i)
	{
		char x = componente[i][0];
		int y = codifica(x, nrStari, stari);
		for(int j = 0; j < nrAlfabet; ++j)
		{
			int z = tranzitieInt[y][j];
			tranzitieFinal[i][j] = stariAutomat[z];
		}
	}

	/*for(int i = 0; i < maxCuloare + 1; ++i)
	{
		cout<<i<<": ";
		for(int j = 0; j < nrAlfabet; ++j)
			cout<<tranzitieFinal[i][j]<<' ';
		cout<<'\n';
	}*/

	cout<<"Matricea de tranzitie a Automatului Minimal\n";
	for(int i = 0; i < maxCuloare + 1; ++i)
	{
		cout<<decodifica(i, componente)<<": ";
		for(int j = 0; j < nrAlfabet; ++j)
			cout<<decodifica(tranzitieFinal[i][j], componente)<<' ';
		cout<<'\n';
	}

	delete[] stari;
	delete[] matrice;
	delete[] tranzitieInt;
	delete[] tranzitieChar;
	delete[] tranzitieFinal;
	delete[] Final;
	delete[] componente;
	delete[] stariAutomat;
	delete[] culoare;
	delete[] automatMin;
	delete[] auxiliar;
}