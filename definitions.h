#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <pthread.h>

//ESTRUKTURAK

//PCB-ko memory management
struct mm{
	int **pgb;
	int h_code;  	//horri-zenbakia kodearena
	int h_data;	 	//horri zenbakia datuarena
	int offsetC; 	//luzera jakiteko kodearen zatiarena
	int offsetD; 	//luzera jakiteko datuen zatiarena
};
typedef struct mm mm;
//prozesuaren informazioa
struct PCB{
    int pid;//id-a
    int lehentasuna;//0tik-200era. 200 lehentasun gehien eta 0 gutxien
    int x_denb;//exekuzio denbora
    char ego;//egoera. X->exekutatzen. I->itxaroten.
    mm memoria;
};

//hariak hartuko duten informazioa
struct Informazioa{
    int maiztasuna;//pasa behar den maiztasuna
    int id;//zein den jakiteko
};

//nodoak sortu eta bakoitzean PCB izan
struct node{
    struct PCB balioa;
    struct node *next;
};
typedef struct node node;//izena aldatu

//prozesuen lista dinamikoa sortu. Prozesuak lehentasunaren arabera ordenatuak egongo dira
struct process_queue{
    int zenbat;
    node *aurrena;
};
typedef struct process_queue process_queue;//izena aldatu


//ALDAGAI GLOBALAK
int tik;                            //denbora kontrolatzeko. Erlojuaren tik-ak kontatzen ditu
int quantum;                        //prozezu bakoitzeko denbora
int corekop;						//zenbat core dauden.
int maiztasuna;                     //makinaren maiztasuna
int mug;							//0 eta 1 balioak hartuko ditu. Zerbait aldatu den ala ez jakiteko
int quantum;						//prozesatzen egongo den denbora maximoa aldi oro
process_queue *prozesuen_lista;     //prozesuen-lista
process_queue *coreak; 				//coreak
int *horri_taula;					//horri taula.Suposatzen da kernelean dagoela
char *memoria_fisikoa;				//memoria fisikoa
int mem_luz;						//memoria fisikoaren luzera. Suposatuko dugu ez dela beteko, horregaitik zenbaki altuak jarri behar dira

//mutex-ak
pthread_mutex_t kont_tik;
pthread_mutex_t proc_list;
pthread_mutex_t kont_core;

//FUNTZIOAK
void sortu_hariak();
void *clocka (void *hari_param);
void *timera (void *hari_param);
void *loader (void *hari_param);
void *scheduler_dispatcher(void *hari_param);
void *core(void *hari_param);

#endif // DEFINITIONS_H