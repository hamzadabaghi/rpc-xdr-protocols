/*     
	Le programme contient trois fonctions : 

	1-Une fonction de calcul du factoriel d'un nombre.
	2-Une fonction de calcul de la somme de 2 entiers.
	3-Une fonction qui prend en entrée 5 nombres réels et qui retourne le plus petit, le plus grand et la moyenne de ces nombres. 
*/

#include<stdio.h>
#include<rpc/rpc.h>
#include<rpc/xdr.h>
#include<math.h>

#define PROGRAMME_NUMERO 0x23456789
#define VERSION 2
#define FACTORIELLE_NUMERO 1
#define SOMME 2
#define CALCUL 3


/* --------------------- Factorielle --------------------- */

/* structure de donnes entree et sortie */

typedef struct factorielle {
	int a;
	int factorielle;
}Factorielle;


/* procedure de factorielle */
int * factorielle(int *a){
	static int resultat;
	int f=1,i;
	for (i = 1; i <= (*a) ; i++)
   	   	f = f * i;
	resultat = f;
	return &resultat;
}


/* decodage */
bool_t xdr_factorielle(XDR * ptr_xdr, Factorielle *p){
	return (xdr_int(ptr_xdr,&p->a) && 
		xdr_int(ptr_xdr,&p->factorielle)
		);
}

/* --------------------- fin Factorielle --------------------- */


/* --------------------- Somme --------------------- */

/* structure de donnes entree et sortie */

typedef struct somme {
	int a;
	int b;
	int somme;
}Somme;


/* procedure de factorielle */
Somme * somme(Somme *s){
	static Somme som ;
	som.a=s->a;
	som.b=s->b;
	som.somme = som.a + som.b;
	return &som;
}


/* decodage */
bool_t xdr_somme(XDR * ptr_xdr, Somme *p){
	return (xdr_int(ptr_xdr,&p->a) && 
		xdr_int(ptr_xdr,&p->b) &&
		xdr_int(ptr_xdr,&p->somme)
		);
}


/* --------------------- fin Somme --------------------- */


/*  ------------- moyenne , min , max --------------------- */

/* structure de donnes entree et sortie */

typedef struct calcul {
	float T[5];
	float moyenne;
	int min_ind;
	int max_ind;
}Calcul;


/* procedure de calcul */
Calcul * calcul(Calcul *a){

	/* valeur de retour */
	static Calcul c;
	int i , min=0 , max=0;
	float sum=0;
	
	
	/* traitement */
	 for (i=0; i<5; i++)
	     {
	      sum+=a->T[i];
	      if(a->T[i]>a->T[max]) max=i;
	      if(a->T[i]<a->T[min]) min=i;
	     }
	c.moyenne=sum/5;
	c.min_ind=min;
	c.max_ind=max;
	/* sortie */
	return &c;
}


/* decodage */
bool_t xdr_calcul(XDR * ptr_xdr, Calcul *p){
	return (xdr_int(ptr_xdr,&p->min_ind) && xdr_int(ptr_xdr,&p->max_ind) &&  xdr_float(ptr_xdr,&p->moyenne) && xdr_vector(ptr_xdr , (char *)p->T , 5 , sizeof(float)
		, (xdrproc_t)xdr_float));
}

/*  ------------- fin moyenne , min , max --------------------- */





int main(){

int fact,som,calc ;


/* desenregistrement de programme entre deux executions consecutives */
pmap_unset(PROGRAMME_NUMERO , VERSION);

/* enregistrement de fonction factorielle sur le portmapper */
fact = registerrpc(PROGRAMME_NUMERO , VERSION , FACTORIELLE_NUMERO , factorielle , xdr_int , xdr_int );

/* enregistrement de fonction somme sur le portmapper */
som = registerrpc(PROGRAMME_NUMERO , VERSION , SOMME , somme , xdr_somme , xdr_somme );

/* enregistrement de fonction calcul sur le portmapper */
calc = registerrpc(PROGRAMME_NUMERO , VERSION , CALCUL , calcul , xdr_calcul , xdr_calcul );


/* lancement d'ecoute */
svc_run();



}


















