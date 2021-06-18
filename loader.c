#include <stdlib.h>
#include "definitions.h"
#include "list.h"

void *loader(void *hari_param){
    //batzuetan prozesu bat sortzen du
    int zenbakia;
    struct PCB P;//prozesua sortzeko
    struct mm M; //PCB-ren mm-a
    M.pgb=&horri_taula;//PCB bakoitzak horri taularen helbidea behar du.
    int i=0;//zenbatgarren prozesua den jakiteko
    int aurkitua;//aldagai laguntzaile bat
    int nonF=0;    //memoria fisikoan non sartu behar den erabakitzen da. Denak ordenean sartuko dira.
    int nonHT;      //horri taulean non sartu behar den erabakitzeko erabiltzen da.
    int j;          //for ean erabiltzeko
    while(1){
        if(prozesuen_lista->zenbat<800){//aldioro 1000 prozesu baino gehiago ez eduki. Memoria kontrolatzeko eta ez betetzeko.
          //%0,001 eko probabilitatearekin sortuko du PCB bat
          zenbakia = rand() % 100000;
          if(zenbakia==0){
            P.pid=i;
            P.x_denb=rand() % 400 + 1;//exekuzoa denbora bat eman auzaz.
            P.ego='I';
            P.lehentasuna=rand() % 201;//lehentasuna jarri 0tik 200 era.
            //MEMORIA
            //supodatzen da irakurri egiten dituela eta memorian sartu

            //aAURRENA KODEA gorde.
            //horri taulan non dagoen benetako helbidea.
            M.h_code=nonHT;
            //horri taulan helbide fisikoa jarri.
            horri_taula[nonHT]=nonF;
            //offset bat aukeratu kode zatiarentzat. random bat 1 eta 5 en artean.
            M.offsetC=rand() % 4 + 1;
            //memoria fisikoa bete. Betetzeko 'c' char bat jarriko da toki bakoitzean
            for(j=0;j<M.offsetC;j++){
                memoria_fisikoa[nonF]='c';
            }
            //aktualizatu nonHT eta nonF;
            nonHT++;//horri taulan lerro bakarra okupatzen du
            nonF=nonF+M.offsetC;


            //ORAIN DATUAK gorde
            //horri taulan non dagoen benetako helbidea.
            M.h_data=nonHT;
            //horri taulan helbide fisikoa jarri.
            horri_taula[nonHT]=nonF;
            //offset bat aukeratu kode zatiarentzat. random bat 1 eta 5 en artean.
            M.offsetD=rand() % 4 + 1;
            //memoria fisikoa bete. Betetzeko 'd' char bat jarriko da toki bakoitzean
            for(j=0;j<M.offsetD;j++){
                memoria_fisikoa[nonF]='d';
            }
            //aktualizatu nonHT eta nonF;
            nonHT++;//horri taulan lerro bakarra okupatzen du
            nonF=nonF+M.offsetC;

            //mm-a PCB-an sartu
            P.memoria=M;

            //listan sartu
            //mutex
            pthread_mutex_lock(&proc_list);
            listaSartu(prozesuen_lista,P);
            pthread_mutex_unlock(&proc_list);
            //zenbat dauden aktualizatu
            i++;
            //lista ikusteko ondo egiten duela
            //listaInprimatu(prozesuen_lista);
          }
       }
    }
}