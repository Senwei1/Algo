/*========================================================================
Fichier Canevas_TP.cc 
Cours d'algorithmique du texte HMSN116M - Universit� de Montpellier
Ann�e 2017-18

Cr�� par S�verine B�rard, modifi� par MECHKOURI Nazim
=========================================================================*/
#include <iostream>
#include <fstream> //pour lire les fichiers
#include <string>
#include <map> //pour les tableaux associatifs
#include <vector> // pour les vecteur

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// NA�F ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void naif(string P, int m, string T, int n) {
	int pos = 1;
	int comp = 0;
	int i = 0;

    while(pos <= n - m + 1) {
		i = 1;
	
		while(i <= m && P[i] == T[pos + i - 1]) {
		    i++;
		    comp++;
		}

		if(i <= m) {
		    comp++;
		}

		if( i == m + 1) {
		    cout << "P apparait a la posisition " << pos << "\n";
		}

		pos++;
    }

	cout << "Nombre de comparaison : " << comp << "\n";
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// MP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int* calcule_MP_next(string P, int m) {
	int* MP_next = new int[m + 2];

	MP_next[1] = 0;
	int j = 0;

   	for(int i = 1; i <= m; i++) {
    	while(j > 0 && P[i] != P[j]) {
			j = MP_next[j];
		}

		j++;
		MP_next[i+1] = j;
	}

	return MP_next;
}

void MP(string P, int m, string T, int n) {
    int* MP_next = calcule_MP_next(P, m);
    int comp = 0;

    cout << "MP_next : ";
    for(int i = 1; i < m + 2; i++) {
		cout << MP_next[i];
    }
    cout << "\n";

    int i = 1;
    int j = 1;
    
    while(j <= n) {
        while(i > 0 && P[i] != T[j]) {
		    i = MP_next[i];
		    comp++;
        }

		if(i > 0) {
		    comp++;
		}

		i++;
		j++;
		if(i == m + 1) {
		    cout << "P apparait a la posisition " << j - i + 1 << "\n";
		    i = MP_next[i];
		}
    }

    cout << "Nombre de comparaison : " << comp << "\n";

	delete[] MP_next;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// KMP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int* calcule_KMP_next(string P, int m) {
    int* KMP_next = new int[m + 2];

    KMP_next[1] = 0;
    int j = 0;

    for(int i = 1; i <= m; i++) {
		while(j > 0 && P[i] != P[j]) {
		    j = KMP_next[j];
		}

		j++;

		if(i == m || P[i+1] != P[j]) {
		    KMP_next[i+1] = j;
		} else {
		    KMP_next[i+1] = KMP_next[j];
		}
    }

    return KMP_next;
}

void KMP(string P, int m, string T, int n) {
    int* KMP_next = calcule_KMP_next(P, m);
    int comp = 0;

    cout << "KMP_next : ";
    for(int i = 1; i < m + 2; i++) {
		cout << KMP_next[i];
    }
    cout << "\n";

    int i = 1;
    int j = 1;

    while(j <= n) {
		while(i > 0 && P[i] != T[j]) {
		    i = KMP_next[i];
		    comp++;
		}
	
		if(i > 0) {
		    comp++;
		}

		i++;
		j++;

		if(i == m + 1) {
		    cout << "P apparait a la position " << j - i + 1 << "\n";
		    i = KMP_next[i];
		}
    }

    cout << "Nombre de comparaison : " << comp << "\n";

	delete[] KMP_next;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Boyer-Moore ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int* calcule_suff(string P, int m) {
    int* suff = new int[m + 1];

    suff[m] = m;
    int g = m;
    int f;

    for(int i = m - 1; i >= 1; i--) {
		if(i > g && suff[i + m - f] != i - g) {
		    suff[i + m - f] < i - g ? suff[i] = suff[i + m - f] : suff[i] = i - g;
		} else {
		    f = i;
		    if(i < g) {
				g = i;
		    }

		    while(g > 0 && P[g] == P[g + m - f]) {
				g--;
		    }

		    suff[i] = f - g;
		}
    }

    return suff;
}

int* calcule_D(string P, int m) {
	int* suff = calcule_suff(P, m);

	int i = 0;

    cout << "suff : ";
    for(i = 1; i < m + 1; i++) {
		cout << suff[i];
    }
    cout << "\n";

    int* D = new int[m + 1];

    for(i = 1; i <= m; i++) {
		D[i] = m;
    }

    // cas 2
    i = 0;
	int j = 0;

    for(j = m; j > 0; j--) {
		if(j - 1 == 0 || suff[j - 1] == j - 1) {
		    while(i <= m - j + 1) {
				D[i] = m - j + 1;
				i++;
		    }
		}
    }

    // cas 1
    for(j = 1; j <= m - 1; j++) {
		D[m - suff[j]] = m - j;
    }

	delete[] suff;

    return D;
}

map<char, int> calcule_R(string P, int m) {
    map<char, int> R;

    for(int i = 1; i <= m; i++) {
		if(R.find(P[i]) != R.end()) {
		    R[P[i]] = i;
		} else {
		    R.insert({P[i], i});
		}
    }

    return R;
}

map<char, std::vector<int>> calcule_S(string P, int m) {
    map<char, std::vector<int>> S;

    for(int i = m; i > 0; i--) {
		if(S.find(P[i]) != S.end()) {
	    	S[P[i]].push_back(i);
		} else {
		    S.insert({P[i], std::vector<int>()});
			S[P[i]].push_back(i);
		}
    }

    return S;
}

void BM(string P, int m, string T, int n) {
    int* D = calcule_D(P, m);

	int i = 0;

    cout << "D : ";
    for(i = 1; i < m + 1; i++) {
		cout << D[i];
    }
    cout << "\n";

    map<char, int> R = calcule_R(P, m);
    cout << "R : ";
    for(std::map<char, int>::iterator it = R.begin(); it != R.end(); ++it) {
		cout << "{" << it->first << ", " << it->second << "} ";
    }
    cout << "\n";

    int pos = 1;
    int comp = 0;

    while(pos <= n - m + 1) {
		i = m;

		while(i > 0 && P[i] == T[pos + i - 1]) {
	    	i--;
		    comp++;
		}

		if(i > 0) {
		    comp++;
		}

		if(i == 0) {
		    cout << "P apparait a la position : " << pos << "\n";
		    pos += D[1];
		} else {
		    D[i] > i - R[T[pos + i - 1]] ? pos += D[i] : pos += i - R[T[pos + i - 1]];
		}
    }

    cout << "Nombre de comparaison : " << comp << "\n";

	delete[] D;
}

void BM2(string P, int m, string T, int n) {
	int* D = calcule_D(P, m);

	int i = 0;

    cout << "D : ";
    for(i = 1; i < m + 1; i++) {
		cout << D[i];
    }
    cout << "\n";

    map<char, std::vector<int>> S = calcule_S(P, m);
    cout << "S : ";
    for(std::map<char, std::vector<int>>::iterator it = S.begin(); it != S.end(); ++it) {
		cout << "{" << it->first << ", [" << it->second[0];
		for(unsigned int i = 1; i < it->second.size(); i++) {
			std::cout << ", " << it->second[i];			
    	}
		std::cout << "]}, ";
	}
    cout << "\n";

    int pos = 1;
    int comp = 0;
	int k = 0;

	while(pos <= n - m + 1) {
		i = m;

		while(i > 0 && P[i] == T[pos + i - 1]) {
	    	i--;
		    comp++;
		}

		if(i > 0) {
		    comp++;
		}

		if(i == 0) {
		    cout << "P apparait a la position : " << pos << "\n";
		    pos += D[1];
		} else {
			for(k = 0; k < (int)S[T[pos + i - 1]].size(); k++) {
				if(S[T[pos + i - 1]][k] < i) {
					k = S[T[pos + i - 1]][k];
					break;
				}
			}
		    D[i] > i - k ? pos += D[i] : pos += i - k;
		}
    }

    cout << "Nombre de comparaison : " << comp << "\n";

	delete[] D;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// main ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    if(argc != 3) {
	cout << "Mauvaise utilisation du programme " << argv[0] << ", on attend :" << "\n";
	cout << argv[0] << " ficMotif ficTexte" << "\n";
    } else {
	/* Lecture du motif */
	string nomFicMotif(argv[1]);
	ifstream IfFicMotif(nomFicMotif.c_str(), ios::in);
	if(!IfFicMotif) {
	    cout << "\n" << "Impossible d'ouvrir le fichier : " << nomFicMotif << "\n\n";
	    return 0;
	}
	string P;
	char c;
	//On lit le fichier caract�re par caract�re
	while(IfFicMotif.get(c)){
	    P += c;
	}
	//On �limine l'�ventuel retour � la ligne en fin de fichier
	if(P.size() > 1 && (int)P[P.size() - 1] == 10) {
	    P = P.substr(0, P.size() - 1);
	}

	P = '#' + P; //Pour que la premi�re lettre de P soit � l'indice 1
	int m = P.size() - 1;

	/* Lecture du texte */
	string nomFicTexte(argv[2]);
	ifstream IfFicTexte(nomFicTexte.c_str(), ios::in);
	if(!IfFicTexte) {
	    cout << "\n" << "Impossible d'ouvrir le fichier : " << nomFicTexte << "\n\n";
	    return 0;
	}
	string T;
	//On lit le fichier caract�re par caract�re
	while(IfFicTexte.get(c)) {
	    T += c;
	}
	//On �limine l'�ventuel retour � la ligne en fin de fichier
	if(T.size() > 1 && (int)T[T.size() - 1] == 10) {
	     T = T.substr(0, T.size() - 1);
	}
	T= '#' + T; //Pour que la premi�re lettre de T soit � l'indice 1
	int n = T.size() - 1;
	  
	cout << "********************************************************************\n";
	cout << "Recherche du motif de taille " << m << " P = " << P.substr(1, P.size()) << "\n";
	//On n'affiche plus le texte car il peut �tre long
  	cout << "Dans un texte de taille " << n << " T = " << T.substr(1, T.size()) << "\n";
	cout << "********************************************************************\n\n\n";

	cout << "************* Recherche naive *************" << "\n";
	naif(P, m, T, n);
	cout << "###############################" << "\n\n";

	cout << "************* Recherche avec MP *************" << "\n";
	MP(P, m, T, n);
	cout << "################################" << "\n\n";

	cout << "************* Recherche avec KMP *************" << "\n";
	KMP(P, m, T, n);
	cout << "################################" << "\n\n";

	cout << "************* Recherche avec Boyer-Moore *************" << "\n";
	BM(P, m, T, n);
	cout << "################################" << "\n";

	cout << "************* Recherche avec Boyer-Moore ameliore *************" << "\n";
	BM2(P, m, T, n);
	cout << "################################" << "\n";
    }
}
