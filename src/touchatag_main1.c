#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "touchatag_reader.h"
#include "touchatag_tag.h"
#include "touchatag_taglist.h"
#include "touchatag_time.h"

#define MAX_ROW 30

libusb_t libusb;
tag_t tag[2];
reader_t reader;
notes_t notes;
char idmio[]={'0','0','0','0','5','4'};
long UID[7];
long DATA[7]; 
char buf[30];



void sigfun (int sig){
	printf("\nQuit\n");
	touchatag_poweroff_one(&libusb, &reader, idmio);
	exit(-1);
}
	
	 

int main(){
	
	int numtags = 0;
	int i,z;
	int recvconf;
	char *act="firefox";
	int j;
	char w[14];
	char q[48];
	list_t lists[MAX_ROW];

		
	char bah[]={0x01,0x02,0x03,0x04};                     //array che viene scritto in notes.note e poi scritto sul tag
	
	touchatag_sqlite3_init();                              //inizializza il db, è praticamente quella che avevi fatto tu
	touchatag_set_notes(4, 0, bah, &notes);					//scrive l'array bah in notes.note, sapendo che è lungo 4 e lo si vuole iniziare a scrivere dal primo byte libero in scrittura
	
	touchatag_scan_bus(&libusb);							//contiene le libusb init, search e open
	
	(void) signal(SIGINT, sigfun);
	
	if (libusb.dev_number != 0){ 
			//setting time reader
			touchatag_set_time(5000,0);
			//init reader
			touchatag_init_all(&libusb, &reader);
		/*	touchatag_blink_set('r', 0x02, 0x05, 0x01);
			touchatag_blink_all(&libusb, &reader);
			touchatag_blink_set('o', 0x02, 0x05, 0x01);
			touchatag_blink_all(&libusb, &reader);
			touchatag_blink_set('g', 0x02, 0x05, 0x01);			
			touchatag_blink_all(&libusb, &reader);             */
			touchatag_get_IDreader_all(&libusb, &reader,1);     //prende tutti gli id dei reader così da collegare id e handler.
			touchatag_get_firm_one(&libusb, &reader, idmio);	//prende il firmware del reader con l'id dato.
			touchatag_stamp_idreader(&reader);					
			
						
			while (1){
				
				numtags = touchatag_polling_one(&libusb, &reader, tag, idmio);
				
				switch(numtags){
					
					case 0:
							printf("timeout: no tags found\n");
							break;
							
					case 1:	
						//	printf("\n** 1 TAG TROVATO **\n");
						//	printf("ID:\n");
							touchatag_get_sam(&libusb, &reader);
							touchatag_stamp_IDsam(&reader);
						//	touchatag_stamp_IDtag(tag);	
						//	touchatag_hexstamp_data(tag);
						//	touchatag_stamp_otps(tag);
						//	touchatag_stamp_lbits(tag);
						//	j=touchatag_execute0(tag);
						//	if ((j=touchatag_sqlite3_show(tag, buf))!=1) printf("\neRrOr!!");    //aggiunge il tag al db, con nella colonna azione quello scritto in act
						//	j=touchatag_sqlite3_cont(tag);                                     //ritorna il cont del tag
						//	printf("\ncont1 %d\n",j);                                           
						//	j=touchatag_sqlite3_upcont(tag);                                   //aggiunge uno al cont del tag
						//	printf("\nuc %d\n",j);
						//	j=touchatag_sqlite3_cont(tag);              //ritorna il cont del tag                       
						//	printf("\ncont2 %d\n",j);
						//	touchatag_time_startp(1);                                           //bandierina d'inizio
						//	j=touchatag_sqlite3_reset(tag);
						//	touchatag_hexstamp_data(tag);
						//	printf("row: %d\n",touchatag_sqlite3_nrow());
						//	touchatag_stamp_firm(&reader);
							                                      //cancella il tag dal db
						//	printf("\n\n%ld\n\n",touchatag_time_now());                            //da il tempo a partire dal libusb_openall
						//	touchatag_write_ultralight_oviesse(&libusb, &reader, tag, &notes, 0);
						//	touchatag_write_ultralight(&libusb, &reader, tag, &notes, idmio);
						//	printf("DATA Mem:\n");							
						//	touchatag_stamp_datamem(tag);

							break;
							
					case 2:
							printf("\n** 2 TAGS TROVATI **\n");
							//touchatag_hexstamp_datamem(tag);
							printf("\nid1: ");
							touchatag_stamp_IDtag(tag);
							printf("\nid2: ");
							touchatag_stamp_IDtag(&tag[1]);
							printf("\n");
							break;
							
					default:
							printf("ERRORE: numero di tags trovati: %d\n", numtags);
				}
			}
	}


return 0;	
}
