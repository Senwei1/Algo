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

// On initialise nos variables
int pos = 1 ;
int cpt = 0 ;
int i = 0;

	while (pos <= (n-m+1)) {
    	i = 1 ;
		
		while (( i<= m) && P[i] == T[pos + i-1]) {
    		i++ ;
			cpt++; // si les deux conditions sont présentes, on ajoute 1 au compteur et on avance i de 1.
		}
		
		if (i==m+1) { //quand i arrive à la dernière position du texte, on fini la lecture
    		cout <<"P apparait à la position :  " << pos << endl;
		}
		if (i <= m) // dans la grande boucle while, tant que i < m+1 on ajoute 1 au compteur (les comparaisons continuent)
			cpt++;
	pos = pos + 1 ;
	}
	cout <<"valeur du compteur : "<< cpt << endl; //41 comptages check

}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// MP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_MP_next(string P, int m)
{
int	*MP_next = new int[m+2] ; //MP_next sert a declarer un pointeur vers un tableau d'entier.
  					// initialisation des autres variables
MP_next[1] = 0;
int j = 0;
int i = 1;

	for (i=1;i<=m;i++) {
    	while(j>0 && P[i] != P[j]) {
        	j = MP_next[j]; //j = position sur T 
    	}
		j++;
		MP_next[i+1] =j;
	}
	return MP_next ; //Type de retour = int*, qui est le type de l'objet MP_next.
}

void MP(string P, int m, string T, int n)
{
int cpt = 0;	
int i = 1 ;
int j = 1;
int *MP_next = calcule_MP_next(P,m); //on déclare le pointeur vers MP_next
while (j <= n) {
    while((i>0) && (P[i]!=T[j])){
        i = MP_next[i];
		cpt++; //même logique pour les compteurs
    }
	if(i > 0) {
		    cpt++;
		}
    i++;
    j++;
	
    if(i==(m+1)) { //quand i arrive à la dernière position du texte, on fini la lecture
        cout<< "P apparait a la position " << j-i+1 << endl;
        i = MP_next[i];
	} 
		
	
}
cout <<"valeur du compteur : "<< cpt << endl;  //validé 33 comptages

delete[] MP_next; //On supprime la table après chaque utilisation pour éviter des problèmes quand on relance le programme plusieurs fois et que les valeurs ne sont pas réinitialiser
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// KMP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_KMP_next(string P, int m)
{
int *KMP_next = new int[m+2]; // même logique que pour MP_Next

int j = 0;
int i = 1;
KMP_next[1] = 0;


	for (i=1;i<=m;i++){
    	while (j>0 && P[i]!=P[j]) {
        	j = KMP_next[j];
		}
	    j++;
    	if ((i==m) || P[i+1]!= P[j])
	        KMP_next[i+1] = j;
    	else
        	KMP_next[i+1] = KMP_next[j];
	}
return KMP_next;

}

void KMP(string P, int m, string T, int n)
{
int *KMP_next = calcule_KMP_next(P,m); //déclaration du pointeur vers KMP_next
int i = 1;
int j = 1;
int cpt = 0;

 while(j <= n) {
		while(i > 0 && P[i] != T[j]) {
		    i = KMP_next[i];
		    cpt++;
		}
	
		if(i > 0) {
		    cpt++;
		}	

	i++;
	j++;

	if(i == (m+1)) {
   	    cout<< "P apparait a la position " << j-i+1 << endl;
       	i = KMP_next[i];
		
	} 	
}
cout<< "valeur du compteur :" << cpt << endl; //validé 31

delete[] KMP_next; // même logique que pour MP_Next

}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Boyer-Moore ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int* calcule_suff(string P, int m) {
    int* suff = new int[m + 1];
//Note à soi : A boyer-moore on commence par la fin du pattern, on avance par pas de -1 et on "saute"  des gros segments du texte quand il est possible :
//après avoir validé les règles du bon suffixe et du mauvais caractère --> complexité de l'ordre O(mn) donc très efficace pour les longs textes avec moins de comparaison 
// que les trois algo précédents
    suff[m] = m;  
    int g = m;
    int f;

    for(int i = m - 1; i >= 1; i--) {
		if(i > g && suff[i+m-f] != i-g) {
		    suff[i+m-f] < i-g ? suff[i] = suff[i+m-f] : suff[i] = i-g;
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
    for(i = 1; i < m+1; i++) {
		cout << suff[i];
    }
    cout << "\n";

    int* D = new int[m+1];

    for(i = 1; i <= m; i++) {
		D[i] = m;
    }
    // Cas n°2
    i = 0;
	int j = 0;

    for(j = m; j > 0; j--) {
		if(j-1 == 0 || suff[j-1] == j-1) {
		    while(i <= m-j +1) {
				D[i] = m-j +1;
				i++;
		    }
		}
    }

    // cas n°1
    for(j = 1; j <= m - 1; j++) {
		D[m-suff[j]] = m-j;
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
    int cpt = 0;

    while(pos <= n - m + 1) {
		i = m;

		while(i > 0 && P[i] == T[pos + i - 1]) {
	    	i--;
		    cpt++;
		}

		if(i > 0) {
		    cpt++;
		}

		if(i == 0) {
		    cout << "P apparait a la position : " << pos << endl;
		    pos += D[1];
		} else {
		    D[i] > i - R[T[pos + i - 1]] ? pos += D[i] : pos += i - R[T[pos + i - 1]];
		}
    }

    cout << "Nombre de comparaison : " << cpt << endl;

	delete[] D;
}

void BM_ameliore(string P, int m, string T, int n) {
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
    int cpt = 0;
	int k = 0;

	while(pos <= n - m + 1) {
		i = m;

		while(i > 0 && P[i] == T[pos + i - 1]) {
	    	i--;
		    cpt++;
		}

		if(i > 0) {
		    cpt++;
		}

		if(i == 0) {
		    cout << "P apparait a la position : " << pos << endl;
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

    cout << "Nombre de comparaison : " << cpt << endl;

	delete[] D;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// main ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   if (argc != 3)
      {
	 cout<<"Mauvaise utilisation du programme "<<argv[0]<<", on attend :"<<endl;
	 cout<<argv[0]<<" ficMotif ficTexte"<<endl;
      }
   else
      {
	 /* Lecture du motif */
	 string nomFicMotif (argv[1]);
	 ifstream IfFicMotif (nomFicMotif.c_str(), ios::in);
	 if (!IfFicMotif)
	    {
	       cout<<endl<<"Impossible d'ouvrir le fichier : "<<nomFicMotif<<endl<<endl;
	       return(0);
	    }
	 string P; char c;
	 //On lit le fichier caract�re par caract�re
	 while(IfFicMotif.get(c)){
	    P += c;
	 }
	 //On �limine l'�ventuel retour � la ligne en fin de fichier
	 if (P.size() >1 && (int)P[P.size()-1]==10)
	    P=P.substr(0,P.size()-1);

	 P='#'+P; //Pour que la premi�re lettre de P soit � l'indice 1
	 int m=P.size()-1;

	 /* Lecture du texte */
	 string nomFicTexte(argv[2]);
	 ifstream IfFicTexte (nomFicTexte.c_str(), ios::in);
	  if (!IfFicTexte)
	    {
	       cout<<endl<<"Impossible d'ouvrir le fichier : "<<nomFicTexte<<endl<<endl;
	       return(0);
	    }
	  string T;
	  //On lit le fichier caract�re par caract�re
	  while(IfFicTexte.get(c)){
	     T += c;
	  }
	  //On �limine l'�ventuel retour � la ligne en fin de fichier
	  if (T.size() >1 && (int)T[T.size()-1]==10)
	     T=T.substr(0,T.size()-1);

	  T='#'+T; //Pour que la premi�re lettre de T soit � l'indice 1
	  int n=T.size()-1;

	  cout<<"********************************************************************"<<endl;
	  cout<<"Recherche du motif de taille "<<m<<" P="<<P.substr(1,P.size())<<endl;
	  //On n'affiche plus le texte car il peut �tre long
  	 cout<<"Dans un texte de taille "<<n<<endl;//<<" T="<<T.substr(1,T.size())<<endl;
	  cout<<"********************************************************************"<<endl<<endl<<endl;

	  cout<<"************* Recherche na�ve *************"<<endl;
	  naif(P,m,T,n);
	  cout<<"###############################"<<endl<<endl;

	  cout<<"************* Recherche avec MP *************"<<endl;
	  MP(P,m,T,n);
	  cout<<"################################"<<endl<<endl;

	  cout<<"************* Recherche avec KMP *************"<<endl;
	  KMP(P,m,T,n);
	  cout<<"################################"<<endl<<endl;

	  cout<<"************* Recherche avec Boyer-Moore *************"<<endl;
	  BM(P,m,T,n);
	  cout<<"################################"<<endl;

	  cout<<"************* Recherche avec Boyer-Moore ameliore *************"<<endl;
      BM_ameliore(P,m,T,n);
	  cout<<"################################"<<endl;
      }
}
