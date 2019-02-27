#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
Pour les sources, se referer à 
https://guyteub.xyz/lucas/SE-Projet_PC/src/branch/v4/Projet%20Eclipse/src/prodcons/v2
Aller sur la branche v4, puis dans le dossier v2 (c'est bizarre mais c'est comme ça mdr)
*/


typedef struct {
	int N;
	char msg[];
}Message;



typedef struct {
	int in;
	int out;
	int size;
	int nbmsgTotal;
	int nmsg;
	int nbMessProd;
	int nbMessCons;
	bool consNExemplaire;
	Semaphore notFull;
	Semaphore notEmpty;
	Semaphore mutex;
	Message tab[];
}ProdConsBuffer;

typedef struct {
	// ProdConsBuffer buffer;
	int tpsCons;
	int mavg;
	Message msg[];
}Consumer;

typedef struct {
	// ProdConsBuffer buffer;
	int tpsProd;
	int mavg;
	bool done;
	Message msg[];
}Producer;

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

void put(Message m){
		// On ne peut pas put si le buffer est plein
		notFull.acquire();
		// le mutex n'a qu'une seule ressource. Si on arrive a la prendre, on est le
		// seul à executer le code suivant
		mutex.acquire();

		this.tab[this.in] = m;
		this.in = (this.in + 1) % this.size;
		this.nbmsg++;
		this.nbMessProd++;

		// On relache la ressource
		mutex.release();
		// On relache aussi
		notEmpty.release();
	}

Message get(){
		// On prent la ressource si le semaphore n'est pas vide, car on ne peut pas get
		// si le buffer est vide
		notEmpty.acquire();
		mutex.acquire();

		Message tempMsg = tab[out];
		this.out = (this.out + 1) % this.size;
		this.nbmsg--;
		this.nbMessCons++;

		mutex.release();
		notFull.release();
		return tempMsg;
	}

void runConsumer() {
		int i = 0;
		while (i < mavg) { // on va consommer mavg messages
				bool done = false;
				printf("\nConsumer N°" + (this.getId() - 9) + " consomme " + x + " messages");
				
					Thread.sleep(tpsCons); // on attend entre chaque get
					msg[i] = buffer.get();
					printf("Consumer N°" + (this.getId() - 9) + " -> msg reçu : \"" + msg[i] + "\"");
				
				i ++;
		}
		consumer.done = true;
	}

	void runProducer() {

		for (int i = 0; i < mavg; i++) {
				Thread.sleep(tpsProd);
				int x = rand((3 - 1)) + 1;
				msg[i] = new Message("Producer " + (this.getId() - 9) + ", msg N°" + (i + 1), x);
				printf(msg[i]+"*"+x);
				buffer.put(msg[i]);

		}
		done = true;
		return;
	}

void Randomize() {

		l = new ArrayList<>();

		for (int i = 0; i < nbP; i++) {
			l.add("prod");
		}
		for (int j = 0; j < nbC; j++) {
			l.add("cons");
		}

		int min = 0;
		int max = l.size();
		int x;
		int iProd = 0;
		int iCons = 0;
		boolean prodDone = false;
		boolean consDone = false;
		while (!l.isEmpty()) {
			Random rand = new Random();
			x = rand.nextInt((max - min)) + min;
			String s = l.get(x);
			if (s == "prod") {
				if (!prodDone && iProd < prod.length) {
					prod[iProd++] = new Producer(buffer, ProdTime, Mavg);
					if (iProd == prod.length)
						prodDone = true;
				}
			} else if (s == "cons") {
				if (!consDone && iCons < cons.length) {
					cons[iCons++] = new Consumer(buffer, ConsTime, Mavg, lock);
					if (iCons == cons.length)
						consDone = true;
				}

			}
			l.remove(s);
			max = l.size();
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
		int Mavg = 15;
		bool done;
		int mavg = 10;
		Message msg[mavg];

		ProdConsBuffer buffer = malloc(sizeof(ProdConsBuffer));
		buffer.size = BufSz;
		Producer prod [nbP]; // initialiser chaque case ???
		Consumer cons[nbC];

		Randomize();

		int i = 0;
		done = false;
		while (!done) {
			done = true;
			i = 0;
			while (i < prod.length) {
				done = done && prod[i].isDone();
				i++;
			}
			// Si tout les producteurs ont finit de produire
			// et que le buffer est vide (tout a ete consommé)

		}
		
		while (buffer.nmsg() != 0) {
			System.gc();
		}

		// Arriv� ici, tous les producteurs ont tout produit, et tous les consomamteurs
		// ont tout consomm� <=> il ne reste rien a faire
		// on verifie si on a bien produit le nombre de message attendu
		assert (buffer.getNbMessProd() == (nbP * Mavg));
		// assert(buffer.getNbMessCons() == (nbC*Mavg) );
		// on verifie que on a consomm� autant que ce qu'on a produit
		assert (buffer.getNbMessProd() == buffer.getNbMessCons());

		System.out.println(buffer.getNbMessProd() + " messages ont été produit, " + buffer.getNbMessCons()
				+ " messages ont été consommé");

		System.exit(1);
	}