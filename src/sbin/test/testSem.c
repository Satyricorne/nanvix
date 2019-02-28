#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "testSem.h"
#include "ArrayList.h"
/*
Pour les sources, se referer à 
https://guyteub.xyz/lucas/SE-Projet_PC/src/branch/v4/Projet%20Eclipse/src/prodcons/v2
Aller sur la branche v4, puis dans le dossier v2 (c'est bizarre mais c'est comme ça mdr)
*/

#define notFull 0
#define notEmpty 1
#define mutex 2


/*###########################################
################  PRODUCER  #################
###########################################*/

// on initialise un producteur
void initProducer(Producer *producer,int mavg,int id){
	producer->done = false;
	producer->msg = malloc(sizeof(Message)*mavg);
	producer->id = id;
}

/*###########################################
############# Ici, on veut une liste de pointeur de producer  ###############
###########################################*/
// on initialise tous les producteurs
void initAllProd(Producer *prod,int mavg,int nbP){
	for (int i = 0; i < nbP; ++i)
	{
		initProducer(&prod[i],mavg,i);
	}
}

/*###########################################
############# Ici, on veut un pointeur de producer  ###############
###########################################*/
// on commence la production d'un producteur
void runProducer(Producer *producer,int mavg,ProdConsBuffer *buffer,int tpsProd) {

	for (int i = 0; i < mavg; i++) {
		/*########################################
		########## equivalent en C de sleep ??? ########
		#########################################*/
		// Thread.sleep(tpsProd); 
		char str[] = "Producer ";
		strcat(str,producer->id);
		strcat(str," , msg N°");
		strcat(str,i+1);

		Message mymsg = malloc(sizeof(Message));
		strcpy(mymsg.msg,str);
		producer->msg[i] = mymsg;
		printf("%s",producer->msg[i].msg);
		put(buffer,producer->msg[i]);

	}
	producer->done = true;
	return;
}

/*###########################################
################  CONSUMER  #################
###########################################*/

// on initialise un consumer
void initConsumer(Consumer *consumer,int Mavg, int id){
	consumer->msg = malloc(sizeof(Message)*Mavg);
	consumer->id = id;
}

/*###########################################
############# Ici, on devrait avoit une liste de pointeur de consumer  ###############
###########################################*/
// on initialise tout les consumer
void initAllCons(Consumer *cons,int mavg, int nbC){ 
	for (int i = 0; i < nbC; ++i)
	{
		initConsumer(&cons[i],mavg, i);
	}
}
/*###########################################
############# Ici, on veut juste un pointeur sur un consumer  ###############
###########################################*/
// on lance la consommation d'un consumer
void runConsumer(Consumer *consumer,ProdConsBuffer *buffer,int tpsCons, int mavg) {
	for (int i = 0; i < mavg; i++) { // on va consommer mavg messages

		// Thread.sleep(tpsCons); // on attend entre chaque get
		consumer->msg[i] = get(buffer);
		printf("Consumer N°%d -> msg recu : \"%s\"",consumer->id, consumer->msg[i].msg);
	}

}

/*###########################################
############# ProdConsBuffer  ###############
###########################################*/



// on initialise le buffer
void initProdConsBuffer(ProdConsBuffer *buffer,int size) {
	buffer->tab = malloc(sizeof(Message)*size);
	buffer->size = size;
	buffer->in = 0;
	buffer->out = 0;
		// Nombre exact de message dans le buffer (prend en compte le N)
	buffer->nbmsgTotal = 0;
		// Contient le nombre d'exemplaire diff�rents de messages
	buffer->nmsg = 0;
	buffer->nbMessCons = 0;
	buffer->nbMessProd = 0;
	buffer->consNExemplaire = true;
}

// mettre un message dans le buffer
void put(ProdConsBuffer *buffer, Message m){
		// On ne peut pas put si le buffer est plein
	semop(notFull,-1);

		// le mutex n'a qu'une seule ressource. Si on arrive a la prendre, on est le
		// seul à executer le code suivant
	semop(mutex,-1);

	buffer->tab[buffer->in] = m;
	buffer->in = (buffer->in + 1) % buffer->size; // buffer circulaire
	buffer->nmsg++;
	buffer->nbMessProd++;

		// On relache la ressource
	semop(mutex,1);
		// On relache aussi
	semopt(notEmpty,1);
}

// on prend un message dans le buffer
Message get(ProdConsBuffer *buffer){
		// On prent la ressource si le semaphore n'est pas vide, car on ne peut pas get
		// si le buffer est vide
	semop(notEmpty,-1);
	semop(mutex,-1);

	Message tempMsg = buffer->tab[buffer->out];
	buffer->out = (buffer->out + 1) % buffer->size; // buffer circulaire
	buffer->nmsg--;
	buffer->nbMessCons++;

	semop(mutex,1);
	semop(notFull,1);
	return tempMsg;
}

// on randomise l'execution des consumers et producer
void Randomize(Producer *prod,Consumer *cons, int nbC, int nbP,ProdConsBuffer *buffer, int tpsCons, int tpsProd, int mavg ) {

	ArrayList l;
	init(&l);

	for (int i = 0; i < nbP; i++) {
		add(&l,(Element) {"prod"});
	}
	for (int j = 0; j < nbC; j++) {
		add(&l,(Element) {"cons"});
	}

	int min = 0;
	int max = sizeL(&l);
	int x;
	int iProd = 0;
	int iCons = 0;
	bool prodDone = false;
	bool consDone = false;
	while (!isEmpty(&l)) {
		x = (rand()%(max - min+1)) + min;
		Element *e = getE(&l,x);
		/*###########################################
############# Probleme de type  ###############
###########################################*/
		char s[] = e->data; 
		if (strcmp(s,"prod") == 0) {
			if (!prodDone && iProd < nbP) {
				/*########################################
				########## ce n'est pas un thread ########
				#########################################*/				
				runProducer(prod[iProd++],buffer, tpsProd,mavg);
				if (iProd == nbP)
					prodDone = true;
			}
		} else if (strcmp(s,"cons") == 0) {
			if (!consDone && iCons < nbC) {
				/*########################################
				########## ce n'est pas un thread ########
				#########################################*/
				runConsumer(cons[iCons++],buffer, tpsCons,mavg);
				if (iCons == nbC)
					consDone = true;
			}

		}
		removeE(&l,s);
		max = l.size;
	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL)); 

	int nbP = 5;
	int nbC = 5;
	int BufSz = 3;
	int ProdTime = 1000;
	int ConsTime = 1000;
	bool done;
	int mavg = 15;

	semget(notFull);
	semget(notEmpty);
	semget(mutex);

	int returnSem;
	returnSem = semctl(notFull,"SETVAL",BufSz);
	semctl(notEmpty,"SETVAL",0);
	semctl(mutex,"SETVAL",1);


	ProdConsBuffer *buffer = malloc(sizeof(ProdConsBuffer));
	buffer->size = BufSz;
	Producer prod [nbP]; // initialiser chaque case ???
	/*###########################################
############# Probleme de type de pointeurs  ###############
###########################################*/
	initAllProd(&prod,mavg,nbP);
	Consumer cons[nbC];
	initAllCons(&cons,mavg, nbC);

	Randomize(&prod,&cons, nbC, nbP, buffer, ConsTime, ProdTime, mavg );

	int i = 0;
	done = false;
	while (!done) {
		done = true;
		i = 0;
		while (i < nbP) {
			done = done && prod[i].done;
			i++;
		}
		// Si tout les producteurs ont finit de produire
		// et que le buffer est vide (tout a ete consommé), on passe a la suite

	}
	
	while (buffer->nmsg != 0) {
		// on attend que tout les messages aient été consommé
	}

	// Arriv� ici, tous les producteurs ont tout produit, et tous les consomamteurs
	// ont tout consomm� <=> il ne reste rien a faire
	// on verifie si on a bien produit le nombre de message attendu
	assert (buffer->nbMessProd == (nbP * mavg));
	// assert(buffer.getNbMessCons() == (nbC*Mavg) );
	// on verifie que on a consomm� autant que ce qu'on a produit
	assert (buffer->nbMessProd == buffer->nbMessCons);

	printf("%d messages ont ete produits, %d messages ont ete consomme\n",buffer->nbMessProd,buffer->nbMessCons);
	exit(0);
}