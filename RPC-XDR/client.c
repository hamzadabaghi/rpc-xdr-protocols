#include<stdio.h>
#include<stdlib.h>
#include<rpc/rpc.h>
#include<rpc/xdr.h>

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



/* decodage */
bool_t xdr_calcul(XDR * ptr_xdr, Calcul *p){
	return (xdr_int(ptr_xdr,&p->min_ind) && xdr_int(ptr_xdr,&p->max_ind) &&  xdr_float(ptr_xdr,&p->moyenne) && xdr_vector(ptr_xdr , (char *)p->T , 5 , sizeof(float)
		, (xdrproc_t)xdr_float));
}

/*  ------------- fin moyenne , min , max --------------------- */


int main(){
	
	/* declaration des variables */
	int a,c,operation,i,resultFact;
	Calcul cal , resultCal;
	Somme som , resultSom;
	Factorielle fact;

	char hostname[255];
	XDR *xdr;
	strcpy(hostname,"localhost");

	/* test */
	while(1){
		printf("\n\n\n----------------------------\n");
		printf("choisir une operation : \n");
		printf("choix [1] : factorielle  \n");
		printf("choix [2] : somme de deux valeurs entieres\n");
		printf("choix [3] : le minimum , maximum et moyenne de 5 valeurs reelles \n");
		scanf("%d",&operation);
		switch(operation) {


		case(1) :

			printf("\nsaisir une valeur entiere : ");
			scanf("%d",&fact.a);
			c = callrpc(hostname,PROGRAMME_NUMERO,VERSION,FACTORIELLE_NUMERO,(xdrproc_t)xdr_int,(char*)&fact.a,(xdrproc_t)xdr_int,(char*)&resultFact);
			if(c != 0)
			perror("erreur ");
			else
			printf("\nle factorielle de %d est %d\n",fact.a,resultFact);
			break;

		case(2) : 
	
			printf("\nsaisir deux valeurs entieres \n");
			printf("\nsaisir la premiere valeur : ");
			scanf("%d",&som.a);
			printf("\nsaisir la deuxieme valeur : ");
			scanf("%d",&som.b);
			c = callrpc(hostname,PROGRAMME_NUMERO,VERSION,SOMME,xdr_somme,(char*)&som,xdr_somme,(char*)&resultSom);
			if(c != 0)
			perror("erreur ");
			else
			printf("\nla somme de %d et %d est %d\n",som.a, som.b,resultSom.somme);
			break;


		case(3) : 	
			
			printf("\nsaisir 5 valeurs reelles : \n");
			for(i=0;i<5;i++)
			{
				printf("\nsaisir T[%d] : ",i+1);
				scanf("%f",&cal.T[i]);
			}
			
			callrpc(hostname,PROGRAMME_NUMERO,VERSION,CALCUL,xdr_calcul,(char*)&cal,xdr_calcul,(char*)&resultCal);
			if(c != 0)
			perror("erreur ");
			else
			printf("les resultats sont : min = %.3f , max = %.3f , moyenne = %.3f ", cal.T[resultCal.min_ind] ,  cal.T[resultCal.max_ind] , resultCal.moyenne);
			break;

			
		}
		}

}	
