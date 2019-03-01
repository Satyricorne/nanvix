// structure du message que l'on envoit
typedef struct {
	int N;
	char msg[];
}Message;

// buffer partagé
typedef struct {
	int in;
	int out;
	int size;
	int nbmsgTotal;
	int nmsg;
	int nbMessProd;
	int nbMessCons;
	_Bool consNExemplaire; // _Boolean
	Message tab[];
}ProdConsBuffer;

/*###########################################
################  PRODUCER  #################
###########################################*/

// structure d'un producteur
typedef struct {
	// ProdConsBuffer buffer;
	int tpsProd;
	int mavg;
	_Bool done; // _Boolean
	int id;
	Message msg[];
}Producer;

// on initialise un producteur
void initProducer(Producer *producer,int mavg,int id);

// on initialise tous les producteurs
void initAllProd(Producer *prod,int mavg,int nbP);

// on commence la production d'un producteur
void runProducer(Producer *producer,int mavg,ProdConsBuffer *buffer,int tpsProd) ;

/*###########################################
################  CONSUMER  #################
###########################################*/
typedef struct {
	// ProdConsBuffer buffer;
	int tpsCons;
	int mavg;
	int id;
	Message msg[];
}Consumer;

// on initialise un consumer
void initConsumer(Consumer *consumer,int Mavg, int id);

// on initialise tout les consumer
void initAllCons(Consumer *cons,int mavg, int nbC);
// on lance la consommation d'un consumer
void runConsumer(Consumer *consumer,ProdConsBuffer *buffer,int tpsCons, int mavg);

/*###########################################
############# ProdConsBuffer  ###############
###########################################*/



// on initialise le buffer
void initProdConsBuffer(ProdConsBuffer *buffer,int size) ;

// mettre un message dans le buffer
void put(ProdConsBuffer *buffer, Message m);
// on prend un message dans le buffer
Message get(ProdConsBuffer *buffer);

// on randomise l'execution des consumers et producer
void Randomize(Producer *prod,Consumer *cons, int nbC, int nbP,ProdConsBuffer *buffer, int tpsCons, int tpsProd, int mavg );
