#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "definitions.h"

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** With a parameter range=100 this function generates numbers in a range 
  * of 0 to 99, meaning 0 < RANDOM_NUMBER < range, being range an integer.
  *                                                                                                                                                       */
int  random_num(int range){
	return (float)rand()/RAND_MAX*range;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** ABC metronome is generated with this routine
  * 
  *                                                                                                                                                       */
void genrtmtr(char* hdrbuffer){
	int count=0;
	char *metro[]={
	        /* 01 */ "V: 2\n",
	        /* 02 */ "%%MIDI channel 10\n",
	        /* 03 */ "%%MIDI program 10\n",
	        /* 04 */ "%%MIDI transpose -25\n",
	        /* 05 */ "K: C",
	};
	strcpy(hdrbuffer, "");
	for(count=0;count<5;count++) {
		strcat(hdrbuffer, metro[count]);
	}
	strcat(hdrbuffer, "\n");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** ABC headers are generated with this function into a header buffer: hdrbuffer = "% ABC notation ....."
  * 
  *                                                                                                                                                       */
void genrthdr(char* hdrbuffer){
	int count=0;
	char *header[]={
	        /* 01 */ "% ABC notation for midi generation file",
	        /* 02 */ "\n%%topmargin 80\n% X = Song Index\nX: ",
	        /* 03 */ "1",
	        /* 04 */ "\n% L = Default Lenght\nL: ",
	        /* 05 */ "1/4",
	        /* 06 */ "\n% T = Title\nT: ",
	        /* 07 */ ARGTITLE,
	        /* 08 */ "\n% Q = Tempo\nQ: ",
	        /* 09 */ ARGTEMPO,
	        /* 10 */ "\n% C = Composer\nC: ",
	        /* 11 */ ARGCOMPOSER,
	        /* 12 */ "\n% S = Source\nS: ",
	        /* 13 */ "ABCMusiEx V2",
	        /* 14 */ "\n% Z = Transcriber\nZ: ",
	        /* 15 */ "ABCMusiEx V2",
	        /* 16 */ "\n% M = Meter\nM: ",
	        /* 17 */ ARGBEATS,
	        /* 18 */ "/",
	        /* 18 */ ARGNOTEWID,
		/* 19 */ "\n% K = Key \nK: ",
	        /* 19 */ ARGSONGKEY,
		/* 19 */ " clef=",
	        /* 19 */ ARGCLEF,
	        /* 20 */ "\n% P = Parts, A, A, B, etc.\nP: ",
	        /* 21 */ "A",
	        /* 22 */ "\n% W = Words\nW: ",
	        /* 23 */ ARGWORDS,
	        /* 24 */ "\n% V = Clef\nV: ",
	        /* 25 */ "1"
	};
	strcpy(hdrbuffer, "");
	for(count=0;count<29;count++) {
		strcat(hdrbuffer, header[count]);
	}
	strcat(hdrbuffer, "\n");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** A routine for replacing all characters, pasted from somewhere at the Internet
  * 
  *                                                                                                                                                       */
int replace(char *string, char *oldpiece, char *newpiece){
   int str_index, newstr_index, oldpiece_index, end, new_len, old_len, cpy_len;
   char *c;
   static char *newstring;

   newstring=(char*)malloc(sizeof(string)*40);

   if ((c = (char *) strstr(string, oldpiece)) == NULL) return 0;

   new_len        = strlen(newpiece);
   old_len        = strlen(oldpiece);
   end            = strlen(string) - old_len;
   oldpiece_index = c - string;

   newstr_index = 0;
   str_index = 0;
   while(str_index <= end && c != NULL)
   {
        cpy_len = oldpiece_index-str_index;

        strncpy(newstring+newstr_index, string+str_index, cpy_len);
        newstr_index += cpy_len;
        str_index    += cpy_len;

        strcpy(newstring+newstr_index, newpiece);
        newstr_index += new_len;
        str_index    += old_len;

        if((c = (char *) strstr(string+str_index, oldpiece)) != NULL) oldpiece_index = c - string;
   }
   strcpy(newstring+newstr_index, string+str_index);
   strcpy(string, newstring);
   free(newstring);
   return 1;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** A routine for replacing based on probabilities, based on last one
  * 
  *                                                                                                                                                       */
int replace_prob(char *string, char *oldpiece, char *newpiece, int probability) {

   int str_index, newstr_index, oldpiece_index, end, new_len, old_len, cpy_len, r;
   char *c;
   static char *newstring;

   newstring=(char*)malloc(sizeof(string)*40);

   if ((c = (char *) strstr(string, oldpiece)) == NULL) return 0;

   new_len        = strlen(newpiece);
   old_len        = strlen(oldpiece);
   end            = strlen(string) - old_len;
   oldpiece_index = c - string;

   newstr_index = 0;
   str_index = 0;
   while(str_index <= end && c != NULL)
   {
        r=random_num(100);
        cpy_len = oldpiece_index-str_index;

        strncpy(newstring+newstr_index, string+str_index, cpy_len);
        newstr_index += cpy_len;
        str_index    += cpy_len;

        if(r<probability){
                strcpy(newstring+newstr_index, newpiece);
                newstr_index += new_len;
                str_index    += old_len;
        }
        else {
                strcpy(newstring+newstr_index, oldpiece);
                newstr_index += old_len;
                str_index    += old_len;
        }


        if((c = (char *) strstr(string+str_index, oldpiece)) != NULL) oldpiece_index = c - string;
   }
   strcpy(newstring+newstr_index, string+str_index);
   strcpy(string, newstring);
   free(newstring);
   return 1;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** A routine for replacing once, based on first one
  * 
  *                                                                                                                                                       */
int replace_once(char *string, char *oldpiece, char *newpiece) {

   int str_index, newstr_index, oldpiece_index, end, new_len, old_len, cpy_len;
   char *c;
   static char *newstring;

   newstring=(char*)malloc(sizeof(string)*40);

   if ((c = (char *) strstr(string, oldpiece)) == NULL) return 0;

   new_len        = strlen(newpiece);
   old_len        = strlen(oldpiece);
   end            = strlen(string) - old_len;
   oldpiece_index = c - string;

   newstr_index = 0;
   str_index = 0;
   cpy_len = oldpiece_index-str_index;

   strncpy(newstring+newstr_index, string+str_index, cpy_len);
   newstr_index += cpy_len;
   str_index    += cpy_len;

   strcpy(newstring+newstr_index, newpiece);
   newstr_index += new_len;
   str_index    += old_len;

   if((c = (char *) strstr(string+str_index, oldpiece)) != NULL) oldpiece_index = c - string;
   strcpy(newstring+newstr_index, string+str_index);
   strcpy(string, newstring);
   free(newstring);
   return 1;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Write the file buffer to a file
  * 
  *                                                                                                                                                       */
void wrtfil(char* filbuffer){
	char syscommand[100];
	int count=0;
	FILE *fileout;
	char *syscommands[] = {
		"abc2midi %s -o %s -RS", ABCFILE, MIDFILE,
		"abc2abc %s -V 1 -n 6 -e | yaps - -M 50x100 -s 0.8 -o %s", ABCFILE, PSFILE,
		"evince %s %s &", PSFILE, "",
		"(timidity %s; pkill evince) &", MIDFILE, "",
		//"timidity -ig %s &", MIDFILE, "",	// graphical interface
		"timidity %s -Ow -o %s", MIDFILE, WAVFILE,
		"lame %s %s", WAVFILE, MP3FILE,
		"gs -dNOPAUSE -sDEVICE=pdfwrite -dBATCH -sOutputFile=%s %s", PDFFILE, PSFILE
	};
	fileout=fopen(ABCFILE, "w");
	fprintf(fileout, "%s\n", filbuffer);
	fclose(fileout);
	printf("ABC file generated.\n");
	for(count=0; count<GENSTAGE*3; count+=3){
		sprintf(syscommand, syscommands[count], syscommands[count+1], syscommands[count+2]);
		if(GENSTAGE>4 && (count==6 || count==9)) {
			printf("\nSkipping shell command:\n\n$ %s\n\n", syscommand);
		}
		else {
			printf("\nExecuting shell command:\n\n$ %s\n\n", syscommand);
			system(syscommand);
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Initial song rests are generated with this routine. Very simple: barbuffer = "Z"
  *
  *                                                                                                                                                       */
void genrtrst(char* barbuffer){
        strcpy(barbuffer, "Z");
}
