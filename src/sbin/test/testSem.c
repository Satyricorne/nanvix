#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
Pour les sources, se referer à 
https://guyteub.xyz/lucas/SE-Projet_PC/src/branch/v4/Projet%20Eclipse/src/prodcons/v2
Aller sur la branche v4, puis dans le dossier v2 (c'est bizarre mais c'est comme ça mdr)
*/

#define notFull 0
#define notEmpty 1
#define mutex 2

typedef struct {
	int N;
	char msg[];
}Message;


/*###########################################
################  PRODUCER  #################
###########################################*/
typedef struct {
	// ProdConsBuffer buffer;
	int tpsProd;
	int mavg;
	bool done;
	Message msg[];
}Producer;

void initProducer(Producer &producer,int mavg,int id){
	*producer->done = false;
	*producer->msg = malloc(sizeof(Message)*mavg);
	*producer->id = id;
}

void initAllProd(Producer &prod[],int mavg,int nbP){
	for (int i = 0; i < nbP; ++i)
	{
		initProducer(&prod[i],mavg,i);
	}
}

void runProducer(Producer &producer,int mavg,ProdConsBuffer &buffer,int tpsProd) {

	for (int i = 0; i < mavg; i++) {
		Thread.sleep(tpsProd);
		int x = rand()%3+1;

		char str[] = "Producer %d, msg N°%d", *producer->id, i+1);
Message mymsg = malloc(sizeof(Message));
mymsg.N = x;
mymsg.msg = str;
*producer->msg[i] =mymsg;
printf(*producer->msg[i]+"*"+x);
put(buffer,*producer->msg[i]);

}
*producer->done = true;
return;
}

/*###########################################
################  CONSUMER  #################
###########################################*/
typedef struct {
	// ProdConsBuffer buffer;
	int tpsCons;
	int mavg;
	Message msg[];
}Consumer;

void initConsumer(Consumer &consumer,int Mavg, int id){
	*consumer->msg = malloc(sizeof(Message)*Mavg);
	*consumer->id = id;
}

void initAllCons(Consumer &cons[],int mavg, int nbC){
	for (int i = 0; i < nbC; ++i)
	{
		initConsumer(&cons[i],mavg, i);
	}
}
void runConsumer(Consumer &consumer,ProdConsBuffer &buffer,int tpsCons, int mavg) {
	for (int i = 0; i < mavg; i++) { // on va consommer mavg messages

		Thread.sleep(tpsCons); // on attend entre chaque get
		*consumer->msg[i] = get(buffer);
		printf("Consumer N°%d -> msg recu : \"%s\"",*consumer->id, *consumer->msg[i]);
	}

}

/*###########################################
############# ProdConsBuffer  ###############
###########################################*/

typedef struct {
	int in;
	int out;
	int size;
	int nbmsgTotal;
	int nmsg;
	int nbMessProd;
	int nbMessCons;
	bool consNExemplaire;
	Message tab[];
}ProdConsBuffer;


void initProdConsBuffer(ProdConsBuffer buffer,int size) {
	Message msg[size];
	buffer.tab = malloc(sizeof(Message)*size);
	buffer.size = size;
	buffer.in = 0;
	buffer.out = 0;
		// Nombre exact de message dans le buffer (prend en compte le N)
	buffer.nbmsgTotal = 0;
		// Contient le nombre d'exemplaire diff�rents de messages
	buffer.nmsg = 0;
	buffer.nbMessCons = 0;
	buffer.nbMessProd = 0;
	buffer.consNExemplaire = true;
}

void put(ProdConsBuffer &buffer, Message m){
		// On ne peut pas put si le buffer est plein
	semop(notFull,-1);

		// le mutex n'a qu'une seule ressource. Si on arrive a la prendre, on est le
		// seul à executer le code suivant
	semop(mutex,-1);

	*bufer->tab[*buffer->in] = m;
	*bufer->in = (*buffer->in + 1) % *buffer->size;
	*bufer->nbmsg++;
	*bufer->nbMessProd++;

		// On relache la ressource
	semop(mutex,1);
		// On relache aussi
	semopt(notEmpty,1);
}

Message get(ProdConsBuffer &buffer){
		// On prent la ressource si le semaphore n'est pas vide, car on ne peut pas get
		// si le buffer est vide
	semop(notEmpty,-1);
	semop(mutex,-1);

	Message tempMsg = *bufer->tab[*bufer->out];
	*bufer->out = (*bufer->out + 1) % *bufer->size;
	*bufer->nbmsg--;
	*bufer->nbMessCons++;

	semop(mutex,1);
	semop(notFull,1);
	return tempMsg;
}


void Randomize(Producer &prod[],Consumer &cons[], int nbC, int nbP,ProdConsBuffer &buffer, int tpsCons, int tpsProd, int mavg ) {

	ArrayList l;
	init(&l);
	l = new ArrayList<>();

	for (int i = 0; i < nbP; i++) {
		add(&l,"prod");
	}
	for (int j = 0; j < nbC; j++) {
		add(&l,"cons");
	}

	int min = 0;
	int max = l.size;
	int x;
	int iProd = 0;
	int iCons = 0;
	bool prodDone = false;
	bool consDone = false;
	while (!l.isEmpty()) {
		x = (rand()%(max - min+1)) + min;
		char[] s = l.get(x);
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
		remove(&l,s);
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


	ProdConsBuffer buffer = malloc(sizeof(ProdConsBuffer));
	buffer.size = BufSz;
	Producer prod [nbP]; // initialiser chaque case ???
	initAllProd(&prod,nbP);
	Consumer cons[nbC];
	initAllCons(&cons,nbC);

	Randomize();

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
		// et que le buffer est vide (tout a ete consommé)

	}
	
	while (buffer.nmsg != 0) {
		// on attend de recevoir tout les messages
	}

	// Arriv� ici, tous les producteurs ont tout produit, et tous les consomamteurs
	// ont tout consomm� <=> il ne reste rien a faire
	// on verifie si on a bien produit le nombre de message attendu
	assert (buffer.NbMessProd == (nbP * mavg));
	// assert(buffer.getNbMessCons() == (nbC*Mavg) );
	// on verifie que on a consomm� autant que ce qu'on a produit
	assert (buffer.NbMessProd == buffer.NbMessCons);

	printf("%d messages ont ete produits, %d messages ont ete consomme\n",buffer.nbMessProd,buffer.nbMessCons);
	exit(0);
}