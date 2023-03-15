/*========================================================================
Fichier Canevas_TP.cc
Cours d'algorithmique du texte HMSN116M - Universit� de Montpellier
Ann�e 2017-18

Cr�� par S�verine B�rard, modifi� par VOTRE_NOM_ICI
=========================================================================*/
#include <iostream>
#include <fstream> //pour lire les fichiers
#include <string>
#include <map> //pour les tableaux associatifs

using namespace std;


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// NA�F ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void naif(string P, int m, string T, int n)
{
int pos = 1 ;
int cpt = 0 ;
int i = 0;

	while (pos <= (n-m+1)) {
    	i = 1 ;
		
		while (( i<= m) && P[i] == T[pos + i-1]) {
    		i++ ;
			cpt++;
			}
		if (i==m+1) {
    		cout <<"P apparait à la position :  " << pos << endl;
		}
		if (i <= m)
			cpt++;
	pos = pos + 1 ;
	}
	cout <<"comptage : "<< cpt << endl; //41 comptages check

}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// MP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_MP_next(string P, int m)
{
int	*MP_next = new int[m+2] ;
int j = 0;
int i = 1;

for (i=1;i<=m;i++) {
    while(j>0 && P[i]!=P[j]) {
        j = MP_next[j];
    }
	j++;
	MP_next[i+1] =j;
}
return MP_next ;
}

void MP(string P, int m, string T, int n)
{
int cpt = 0;	
int i = 1 ;
int j = 1;
int *MP_next = calcule_MP_next(P,m);
while (j <= n) {
    while((i>0) && (P[i]!=T[j])){
        i = MP_next[i];
		cpt++;
    }
    i++;
    j++;
	
    if(i==(m+1)) {
        cout<< "P apparait a la position " << j-i+1 << endl;
        i = MP_next[i];
		cpt++;} 
		
	
}
cout <<"comptage : "<< cpt << endl;  //validé 23 comptages
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// KMP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_KMP_next(string P, int m)
{
int *KMP_next = new int[m+2];
int j = 0;
int i = 1;

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
int *KMP_next = calcule_KMP_next(P,m);
int i = 1;
int j = 1;
int cpt = 0;
while (j<=n)  {
	while ((i>0) && P[i]!=T[j]) {
		i = KMP_next[i];
		cpt++;

	}		
	i++;
	j++;
	if(i==(m+1)) {
   	    cout<< "P apparait a la position " << j-i+1 << endl;
       	i = KMP_next[i];
		
	} 	
}
cout<< "comptages :" << cpt << endl; 

}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Boyer-Moore ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_suff(string P, int m)
{
int *suff = new int[m];
int g = m;
int i = m-1;
for (i=m-1;i>0;i--){
	if( i > g && suff[i+m-f]!=(i-g))
		suff[i] = min(suff[i+m-f],(i-g));
	else
		f= i;
		g = min(g,i);
		while(g>0 && P[g]==P[g+m-f]){	
			g--;}
		suff[i]= f-g;
			
}
}

int * calcule_D(string P, int m)
{
int *suff = calcule_suff(string P, int m);
int i = 0;
int j = m-1;
int *D = new int[m];
for (i =1; i<=m;i++){
	D[i]= m;}
i = 1;
for (j=m-1;j>=0;j--){
	if ((j==0) || suff[j]=j){
		while(i<=(m-j)){
			D[i] = (m-j);
			i++;
		}	
	}		
}
for (j = 1; j<=m+1;j++)
	D[m-suff[j]] = m-j;
}

map<char,int> calcule_R(string P, int m)
{

}

void BM(string P, int m, string T, int n)
{

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
      }
}
