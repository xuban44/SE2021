#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "definitions.h"
#include "list.h"
#include "clocka.h"
#include "timera.h"
#include "loader.h"
#include "core.h"


void sortu_hariak(){//clock, timer, process generator
	int i, err, hari_kop;
    pthread_t *hariak;
    struct Informazioa *h_p;
    hari_kop= 4 + corekop;//dauden hari kopurua. betiko 4 ak + hari kopurua

    hariak = malloc(hari_kop * sizeof(pthread_t));
    h_p = malloc(hari_kop * sizeof(struct Informazioa));
    //coreentzat informazioa
    struct Informazioa *c_p;
    c_p = malloc(corekop * sizeof(struct Informazioa));
    //core bakoitzari bere id-a sartu
    for(i = 0; i < corekop; i++){//pasatzeko informazioa sortu
        c_p[i].id = i;
    }

    i=0;//hasieratu

    for(int j = 0; j < hari_kop; j++){//pasatzeko informazioa sortu
        h_p[j].maiztasuna = maiztasuna;
    }

    //1.HARIA
    //clocka haria sortu eta clocka prozesua eman
    err = pthread_create(&hariak[i], NULL, clocka, (void *)&h_p[i]);

    if(err > 0){
        fprintf(stderr, "Errore bat gertatu da clock haria sortzean.\n");
        exit(1);
    }
    i++;//hurrengo harira pasa

    //2.HARIA
    //timera haria sortu eta timera prozesua eman
    err = pthread_create(&hariak[i], NULL, timera, (void *)&h_p[i]);

    if(err > 0){
        fprintf(stderr, "Errore bat gertatu da timer haria sortzean.\n");
        exit(1);
    }
    i++;//hurrengo harira pasa

    //3.HARIA
    //process_generator haria sortu eta process_generator prozesua eman
    err = pthread_create(&hariak[i], NULL, loader, (void *)&h_p[i]);

    if(err > 0){
        fprintf(stderr, "Errore bat gertatu da timer haria sortzean.\n");
        exit(1);
    }
    i++;//hurrengo harira pasa

    //4.HARIA
    err = pthread_create(&hariak[i], NULL, scheduler_dispatcher, (void *)&h_p[i]);

    if(err > 0){
        fprintf(stderr, "Errore bat gertatu da timer haria sortzean.\n");
        exit(1);
    }
    i++;//hurrengo harira pasa

    //coreak harira sartu
    for(int j=0;j<corekop;j++){
        err = pthread_create(&hariak[i], NULL, core, (void *)&c_p[i]);

        if(err > 0){
            fprintf(stderr, "Errore bat gertatu da core hariak sortzean.\n");
            exit(1);
        }
    }

    //HARIAK HASIERATU
    for(i = 0;i < hari_kop;i++){
        pthread_join(hariak[i], NULL);
    }
}

int main(int argc, char *argv[]){
  
	char *c;
	char *d;
  	int maiztasuna;
  	int tamPageTable;
  	if(argc!=3){
    	printf("maiztasuna eta hari kopurua sartu\n");
    	exit(1);
  	}

  	//quantum-a. Core-a zenbat denbora egongo den maximoa PCB-bakoitzarekin
  	quantum=200;
  	//memoria fisikoaren luzera. Suposatzen da ez dela inoiz betetzen
  	mem_luz=5000;
 
    //aldagaia hartu. maiztasuna eta core kopurua
    maiztasuna = strtol(argv[1], &c, 10);
    corekop = strtol(argv[2], &d, 10);

    //COREAK SORTU
    coreak=malloc(corekop*sizeof(process_queue));
    for(int i=0;i<corekop;i++){//core bakoitzeko lista bat
        //listaHasieratu(&coreak[i]);
    }

    //MEMORIA SORTU
    //HORRI-TAULA
    horri_taula = malloc(50000*sizeof(int));
    //MEMORIA FISIKOA
    memoria_fisikoa = malloc(50000*sizeof(char));

    //mutex-ak sortu
    pthread_mutex_init(&kont_tik, NULL);
    pthread_mutex_init(&proc_list,NULL);
    pthread_mutex_init(&kont_core, NULL);
    //listak hasieratu
    prozesuen_lista=malloc(sizeof(process_queue));
    //Process_Queue hasieratu
    listaHasieratu(prozesuen_lista);
    //hariask sortu
    sortu_hariak();
}