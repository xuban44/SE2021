#include "definitions.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//MMU. Honek helbide birtual batetik helbide fisiko bat lortzen du. offset berdina da bietan.
int MMU(int hel_birtuala, int *taula){
    int helbide_fisikoa;
    helbide_fisikoa = taula[hel_birtuala];
    return helbide_fisikoa;
}


void *core(void *hari_param){

    //MEMORIA
    int **PTBR;       //Horri taularen helbidea
    int IR;
    int PC;


    int semaforoa=0;//0 eta 1 balioak hartuko ditu. Kontatzeko erabiltzen da
    int kont=0;//zenbat denbora doan kontatuko du.
    struct Informazioa *param;
    param = (struct Informazioa *)hari_param;
    int id = param->id;
    struct PCB pro;//prozesatzen ari den balioa
    int prozesatzen=0;//lanean ari den ala ez jakiteko
    int luzera;//jakiteko kodea eta datuen luzera memorian
    int i;//aldagai laguntzaile bat
    int bukatua=0;//bukatu duen prozesua ala ez jakiteko
    int helb_fisikoa;   //hemen gordeko da lortutako helbide fisikoa

    while(1){
        //listaInprimatu(&coreak[id]);
        if(semaforoa!=mug){//maiztasuna betetzean begiratu
            if(prozesatzen){//lanean ari bada corea.
                semaforoa=mug;
                kont++;
                //begiratu prozesua bukatu den ala ez
                if(kont>=pro.x_denb){
                    //prozesua bukatu da eta ez da listan sartuko
                    prozesatzen=0;//ez dago inor prozesatzen, hurrengo itzulian hartuko du bat.
                }else{
                    if(kont>=quantum){
                	   kont=0;
                        if(pro.x_denb>quantum){//kenketa egin denbora gehiago behar badu
                            pro.x_denb=-quantum;
                            pro.ego='I';
                            if(pro.lehentasuna>5){//begiratu lehentasuna ez dela 5 baino txikiagoa kenketa egiterakoan
                                pro.lehentasuna=-5;//5 kendu
                            }
                            //korean sartu prozesua.
                            pthread_mutex_lock(&kont_core);
                            listaSartu(&coreak[id],pro);
                            pthread_mutex_unlock(&kont_core);
                            bukatua=1;
                        }
                        //free(pro);
                        //prozesatzen=0;//ez dago inor prozesatzen, hurrengo itzulian hartuko du bat.
                    }
                }
            }
            //ez dago lanean corea
            else{//saiatu prozesu bat hartzean. Prozesua hartzean irakurri memoriatik.
                if(!listaHutsa(&coreak[id])){//begiratu prozesurik badagoen hartzeko
                    pthread_mutex_lock(&kont_core);
                    if(!listaHutsa(&coreak[id])){
                        pro=listaHartu(&coreak[id]);
                        pro.ego='X';//egoera aldatu
                        prozesatzen=1;//suposatzen da lehenengoa hartu duela. Lehentasunaren arabera ordenatuak daudelako

                        //zati honetan irakurketa simulatzen da
                        //MEMORIATIK IRAKURRI BEHAR DA
                        //aurrena horri taularen helbidea PTBR-n gorde
                        PTBR=pro.memoria.pgb;
                        //kodea irakurriko da aurrena PC erregistroarekin
                        //helbide fisikoa kalkulatu
                        helb_fisikoa=MMU(pro.memoria.h_code,*PTBR);
                        for(PC=helb_fisikoa;PC<pro.memoria.offsetC;PC++){
                            //hemen irakurri egiten du baina ez dut inon gordeko
                            memoria_fisikoa[PC];
                        }
                        //orain datuak irakurriko dira IR erregistroarekin
                        //aurrena helbide_fisikoa kalkulatu
                        helb_fisikoa=MMU(pro.memoria.h_data,*PTBR);
                        for(IR=helb_fisikoa;IR<pro.memoria.offsetD;PC++){
                            //hemen irakurri egiten du baina ez dut inon gordeko
                            memoria_fisikoa[IR];
                        }


                    }
                    pthread_mutex_unlock(&kont_core);

                }
            }
        }
    }    
} 