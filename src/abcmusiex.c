#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "abcmusiex.h"
#include "functions.h"
#include "time.h"

#define  NOTNORMPROB 12
#define  NOTDOTPROB 8
#define  NOTTRPPROB 8
#define  NOTRSTPROB 6
#define  SFUNIVERSE 6
/* Sharps and flats universe 0=b,1=#,2,3=2,4=2,5=2*/
#define  LEUNIVERSE 6
/* Legato universe one of eight is legato */

char *notes[]={"C,","D,","E,","F,","G,","A,","B,","C","D","E","F","G","A","B","c","d","e","f","g","a","b","c'","d'","e'","f'","g'","a'","b'", "c''"};
char *notesS[]={"^C,","^D,","E,","^F,","^G,","^A,","B,","^C","^D","E","^F","^G","^A","B","^c","^d","^e","f","^g","^a","b","^c'","^d'","e'","^f'","^g'","^a'","b'", "^c''"};
char *notesF[]={"C,","_D,","_E,","F,","_G,","_A,","_B,","C","_D","_E","F","_G","_A","_B","c","_d","_e","f","_g","_a","_b","c'","_d'","_e'","f'","_g'","_a'","_b'", "c''"};
//    notes array goes from 0 to 28

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Process starts with this function: generates a bar based on metrics:  
  * 4/4 --> [4][4][4][4].
  *                                                                                                                                                       */
void setbarbeats(int beatsperbar, int beatlongbar, char* barbuffer){
	int count;
	char unit[]="[--]";
	strcpy(barbuffer, "");
	if(beatlongbar==8) strcpy(unit, "[8]");
	if(beatlongbar==4) strcpy(unit, "[4]");
	if(beatlongbar==2) strcpy(unit, "[2]");
	for(count=0;count<beatsperbar;count++)
		strcat(barbuffer, unit);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Then the metrics-generated bar is compressed: [4][4][4][4] --> [1] to be splitted on several and pseudo-random notes.
  * 
  *                                                                                                                                                       */
void compressbar(char* barbuffer){
	while(strlen(barbuffer)>6){
		replace(barbuffer, "[8][8]", "[4]");
		replace(barbuffer, "[4][4]", "[2]");
		replace(barbuffer, "[2][2]", "[1]");
	}
	replace(barbuffer, "[8][8]", "[4]");
	replace(barbuffer, "[4][4]", "[2]");
	replace(barbuffer, "[2][2]", "[1]");
	replace(barbuffer, "[2][4]", "[4] [4] [4]");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Then, the starting bar is marked as divisible: [1] --> [1d]
  *  
  *                                                                                                                                                       */
void genrtbar(char* barbuffer, char* barmodel){
	strcpy(barbuffer, barmodel);
	replace(barbuffer, "]", "d]");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Now the main routine: splitting barbuffer randomicly until split max level is reached: 
  * INIT --> [1d] 
  * [1d] --> [2d] [2d]
  * [1d] --> [4d] [4d]     [4d] [4d]
  * [1d] --> [4]  [8d][8d] [4]  [8d][8d]
  * [1d] --> [4]  [8][8]   [4]  [16][16][8]
  *                                                                                                                                                       */
void splitbar(char* barbuffer, int splitlevel, int splitmlevel){
	if(splitlevel<=splitmlevel){
		if(splitlevel==1) {
			replace_prob(barbuffer, "[1d]", "[1]", NOTNORMPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[1d]", " [1z] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[1d]", " [2z][2d] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[1d]", " [2d][2z] ", NOTRSTPROB*splitlevel);
			if(BROKENTS==1)replace_prob(barbuffer, "[1d]", " [2]>[2] ", NOTDOTPROB*(splitlevel-1));
			if(BROKENTS==1)replace_prob(barbuffer, "[1d]", " [2]<[2] ", NOTDOTPROB*(splitlevel-1));
			if(TRIPLETS==1)replace_prob(barbuffer, "[1d]", " (3[2][2][2] ", NOTTRPPROB*(splitlevel-1));
			if(splitlevel==splitmlevel) {
				replace (barbuffer, "[1d]", "[2] [2]");
				replace (barbuffer, "[2d]", "[2]");
			}
			else
				replace (barbuffer, "[1d]", "[2d] [2d]");
		}
		if(splitlevel==2) {
			replace_prob(barbuffer, "[2d]", "[2]", NOTNORMPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[2d]", " [2z] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[2d]", " [4z][4d] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[2d]", " [4d][4z] ", NOTRSTPROB*splitlevel);
			if(BROKENTS==1)replace_prob(barbuffer, "[2d]", " [4]>[4]", NOTDOTPROB*(splitlevel-1));
			if(BROKENTS==1)replace_prob(barbuffer, "[2d]", " [4]<[4]", NOTDOTPROB*(splitlevel-1));
			if(TRIPLETS==1)replace_prob(barbuffer, "[2d]", " (3[4][4][4] ", NOTTRPPROB*(splitlevel-1));
			if(splitlevel==splitmlevel) {
				replace (barbuffer, "[2d]", "[4] [4]");
				replace (barbuffer, "[4d]", "[4]");
			}
			else
				replace (barbuffer, "[2d]", "[4d] [4d]");
		}
		if(splitlevel==3) {
			replace_prob(barbuffer, "[4d]", "[4]", NOTNORMPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[4d]", " [4z] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[4d]", " [8z][8d] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[4d]", " [8d][8z] ", NOTRSTPROB*splitlevel);
			if(BROKENTS==1)replace_prob(barbuffer, "[4d]", " [8]>[8] ", NOTDOTPROB*(splitlevel-1));
			if(BROKENTS==1)replace_prob(barbuffer, "[4d]", " [8]<[8] ", NOTDOTPROB*(splitlevel-1));
			if(TRIPLETS==1)replace_prob(barbuffer, "[4d]", " (3[8][8][8] ", NOTTRPPROB*(splitlevel-1));
			if(splitlevel==splitmlevel) {
				replace (barbuffer, "[4d]", "[8][8]");
				replace (barbuffer, "[8d]", "[8]");
			}
			else
				replace (barbuffer, "[4d]", "[8d][8d]");
		}
		if(splitlevel==4) {
			replace_prob(barbuffer, "[8d]", "[8]", NOTNORMPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[8d]", " [8z] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[8d]", " [16z][16d] ", NOTRSTPROB*splitlevel);
			if(RESTNTES==1)replace_prob(barbuffer, "[8d]", " [16d][16z] ", NOTRSTPROB*splitlevel);
			replace     (barbuffer, "[8d]", "[16][16]");
			replace     (barbuffer, "[16d]", "[16]");
		}
		replace (barbuffer, "  ", " ");
		replace (barbuffer, "  ", " ");
		replace (barbuffer, "  ", " ");
		replace (barbuffer, "  ", " ");
		splitbar(barbuffer, splitlevel+1, splitmlevel);
	}
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Now, some optimizations. This code is not completed
  *                                                                                                                                                       */
void checkbar(char* barbuffer){
	//replace(barbuffer, "[8][8][8] [8][8] [8][8]", " [8][8][8] [8][8][8][8]");
	//replace(barbuffer, "[8][8] [8][8] ", "[8][8][8][8] ");
	//replace(barbuffer, " [8][8] [8][8]", " [8][8][8][8]");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Generating notes based on a dispersion from an original note
  *                                                                                                                                                       */
int disperse_note(int note, int dispersion, int minnote, int maxnote){
	if(ONLEGATO==0)	{
		note+=random_num(dispersion*2+1) - dispersion;
        if(note<minnote)note=minnote+dispersion;
        if(note>maxnote)note=maxnote-dispersion;
	}
	
    return note;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** The vertical approach just replaces generated bar with notes within a range and a dispersion factor:
  * [4] [8][8] [4] [16][16][8] --> c d/2b/2 d a/4f/4b/2
  * The generated bar is ready to be appended at the end of the sngbuffer
  *                                                                                                                                                       */
int  putnotes(char* barbuffer, int note){
	int status;

	replace(barbuffer, "[16]", "[n]/4");
	replace(barbuffer, "[8]",  "[n]/2");
	replace(barbuffer, "[4]",  "[n]");
	replace(barbuffer, "[2]",  "[n]2");
	replace(barbuffer, "[1]",  "[n]4");

	replace(barbuffer, "[16z]", "z/4");
	replace(barbuffer, "[8z]",  "z/2");
	replace(barbuffer, "[4z]",  "z");
	replace(barbuffer, "[2z]",  "z2");
	replace(barbuffer, "[1z]",  "z4");

	while((status=replacenotes(barbuffer, note))) 
		note=disperse_note(note, DISPERSION, MINNOTE, MAXNOTE);
	
	return note;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Here, each note is replaced: [n] --> ^c) (c sharp closing legato)
  *  
  *                                                                                                                                                       */
int replacenotes(char* barbuffer, int note){
	int notype, status;
	char notstr[9]; /* notstr = notesF[note] = "_C," or whatever structure is*/
	int  ONLEGATO_TEMP;
	
	ONLEGATO_TEMP=ONLEGATO;

/* Sharps and flats generation */
	status=0;
	notype=random_num(SFUNIVERSE); 
	/* notype=0-b,1-#,2,3,4,5 */
 	if(notype>2) notype=2;
	/* notype=0-b,1-#,2,2,2,2 */
	if(GENFLATS==0 && notype==0) notype=2;
	/* notype=  2,1-#,2,2,2,2 */
	if(GENSHRPS==0 && notype==1) notype=2;
	/* notype=0-b,2  ,2,2,2,2 */
	
	if(ONLEGATO==1) notype=2;
	
	/* Put sharps or/and flats*/
	if(notype==2) strcpy(notstr, notes[note]);
	if(notype==1) strcpy(notstr, notesS[note]);
	if(notype==0) strcpy(notstr, notesF[note]);

/* Ties/legato generation */
	if(GENLEGAT==1) {
		if(ONLEGATO==1){
			ONLEGATO=0;
		}
		else {
			if(random_num(LEUNIVERSE)==0) {
				strcat(notstr, "-");
				ONLEGATO=1;
			}	
		}
	}
	
	status=replace_once(barbuffer, "[n]", notstr);	
	//replace(barbuffer, "- ", "-");	
	replace(barbuffer, "-/", "/-");	
	replace(barbuffer, "-2", "2-");	
	replace(barbuffer, "-4", "4-");	
	replace(barbuffer, "-8", "8-");	
	
	if(!status)	ONLEGATO=ONLEGATO_TEMP;

	return status;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Then, the starting bar is marked as divisible: [1] --> [1d]
  *  
  *                                                                                                                                                       */
void genrtclk(char* barbuffer, char* barmodel){
	strcpy(barbuffer, barmodel);
	replace(barbuffer, "[4]", "A");
	replace(barbuffer, "[2]", "A A");
	replace(barbuffer, "[1]", "A A A A");
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Main process. It is not called main cause it might be included in a glade project.
  * 
  *                                                                                                                                                       */
void generate(void) {
	/* Main definitions */
	char *barmodel, *barbuffer, *rstbuffer, *hdrbuffer, *sngbuffer,  *filbuffer;
	barmodel =(char *)malloc(BARSIZEMAX);
	barbuffer=(char *)malloc(BARSIZEMAX);
	rstbuffer=(char *)malloc(BARSIZEMAX);
	hdrbuffer=(char *)malloc(HDRSIZEMAX);
	sngbuffer=(char *)malloc(SNGSIZEMAX);
	filbuffer=(char *)malloc(FILSIZEMAX);

	int count=0, note=INITIALNOTE;

	srand((unsigned)time(NULL));

	/* Get the beats quantity, so we can generate a bar with this size */
	setbarbeats(BEATSPERBAR, BEATLONGBAR, barbuffer);
	compressbar(barbuffer);

	strcpy(barmodel, barbuffer);

	/* Generate header and initialize song buffer with it */
	genrthdr(hdrbuffer);
	strcpy(sngbuffer, hdrbuffer);

	genrtrst(rstbuffer);
	strcat(sngbuffer, rstbuffer);
	strcat(sngbuffer, BARSEPRTR);

	/* Then we start the count */
	for(count=0; count<BARSPERSONG-1; count++){
		genrtbar(barbuffer, barmodel);
		splitbar(barbuffer, 1, SPLITMLEVEL);
		checkbar(barbuffer);
		
		note=putnotes(barbuffer, note);

		strcat(sngbuffer, barbuffer);
		strcat(sngbuffer, BARSEPRTR);
	}
	strcat(sngbuffer, BARENDING);
	strcat(sngbuffer, "\n");

	/* Generate metro */
	if(GENMETRO==1){
		genrtmtr(hdrbuffer);
		strcat(sngbuffer, hdrbuffer);
		for(count=0; count<BARSPERSONG; count++){
			genrtclk(barbuffer, barmodel);
			strcat(sngbuffer, barbuffer);
			strcat(sngbuffer, BARSEPRTR);
		}
		strcat(sngbuffer, BARENDING);
	}

	strcpy(filbuffer, sngbuffer);
	wrtfil(filbuffer);
	free(barmodel);
	free(barbuffer);
	free(rstbuffer);
	free(hdrbuffer);
	free(sngbuffer);
	free(filbuffer);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
/** Main process, oriented mainly to arguments processing
  * 
  *                                                                                                                                                       */
int  main (int argc, char *argv[ ]){
	int argn;

	strcpy(ARGTEMPO, "60");
	strcpy(ARGTITLE, "Music Score Reading Exercise");
	strcpy(ARGBEATS, "4");
	strcpy(ARGNOTEWID, "4");
	strcpy(ARGSONGKEY, "C");
	strcpy(ARGCOMPOSER, "Generated with ABCMusiEx");
	strcpy(ARGWORDS, "This printable and audio parts are generated under the terms of the GNU license. ");
	strcpy(ARGCLEF, "treble");

	BARSPERSONG=162;
	strcpy(ALLFILE, "exercise");

	SPLITMLEVEL=2;
	DISPERSION=4;
	MINNOTE=2;
	MAXNOTE=21;

	TRIPLETS=0;
	BROKENTS=0;
	RESTNTES=0;
	SEEDSTRT=0;

	GENSTAGE=4;
	GENMETRO=1;

	GENSHRPS=0;
	GENFLATS=0;
	GENLEGAT=0;
	ONLEGATO=0;

	for(argn=0; argn<argc; argn++)
	{
		if(strcmp(argv[argn], "--h")==0 || strcmp(argv[argn], "-h")==0 || strcmp(argv[argn], "--help")==0 || argc<=1) {
			printf("GNU 'abcmusiex' generates music score and audio exercises for improving musical score reading.\n\n");
			printf("Usage: abcmusiex [OPTION]...\n\nOptions:\n");
			printf("\t-t TEMPO\ttempo (20-300, default TEMPO=60)\n");
			printf("\t-b BEATS\tbeats (2-4, default BEATS=4), metric numerator\n");
			printf("\t-w WIDTH\twidth (2, 4 or 8, default WIDTH=4), 2vs, 4ths or 8ths, metric denominator\n");
			printf("\t-k KEY\t\tkey, default KEY=C\n");
			printf("\t-c CLEF\t\tclef: treble(default), alto, tenor, baritone, bass, mezzo, soprano, perc\n");
			printf("\t-n QUANTITY\tnumber of bars to generate, default QUANTITY=162\n");
			printf("\t-o FILENAME\toutput filenames, default FILENAME=exercise.*\n");
			printf("\t-l LEVEL\tlevel, only use 1 to 4: default LEVEL=2; 1 is 1/2s, 2 is 4ts, 3 is 8ts, 4 is 16ths\n");
			printf("\t-d DISPERSION\tdispersion (distance from last generated tone, in notes), default DISPERSION=3 notes\n");
			printf("\t-T TITLE\ttitle\n");
			printf("\t-C AUTHOR\tauthor\n");
			printf("\t-W COMMENT\tcomment words\n");
			printf("\t--triplets\tinclude triplets in exercise\n");
			printf("\t--dots\t\tinclude dotted couples\n");
			printf("\t--rests\t\tinclude rests\n");
			printf("\t--sharps\tgenerate random sharps\n");
			printf("\t--flats\t\tgenerate random flats\n");
			printf("\t--legato\tgenerate random legatos\n");
			printf("\t--min I\t\tminnote, default I=2; low-low-C=0; low-C=7; C=14; high-C=21; high-high-C=28\n");
			printf("\t--max A\t\tmaxnote, default A=21; low-low-C=0; low-C=7; C=14; high-C=21; high-high-C=28\n");
			printf("\t--seed S\tseed start, 0...10000, default S=0\n\n");
			printf("\t--stage STAGE\tgeneration stage: 0(abc), 1(+mid), 2(+ps), 3(shows .ps), 4(plays .mid, default), 5(+wav, neither show nor play), 6(+mp3), 7(+pdf)\n");
			printf("\t--nometro\tdoes not generate metronom click\n\n");
			printf("\t-h, --help\tshows this help\n");
			printf("\t--version \tshows version\n\n");
			printf("Examples:\n\n");
			printf("\tabcmusiex --help\t\t\tshows this help\n");
			printf("\tabcmusiex -l 3\t\t\t\tgenerates an 8ths exercise\n");
			printf("\tabcmusiex -t 100\t\t\tgenerates a tempo 100 exercise\n");
			printf("\tabcmusiex -n 10 -c F\t\t\tgenerates 10 measures on F key\n");
			printf("\tabcmusiex -T \"Exercise 1\" -o exercise1\tentitles exercise\n\n");
			printf("Report bugs to <rodolfoap@gmail.com>\n");
			return 0;
		}

		if(strcmp(argv[argn], "--version")==0) {
			printf("ABCMusiEX %s\n\n", VERSOFT);
			printf("Copyright (C) 1999 Free Software Foundation, Inc.\n");
			printf("This is free software; see the source for copying conditions.  There is NO\n");
			printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
			printf("Written by Rodolfo Alcazar <rodolfoap@gmail.com>\n");
			return 0;
		};
	
		// Is getopt neccesary?? This seems simpler!

		if(strcmp(argv[argn], "-t")==0 && (argn+1) < argc) strcpy(ARGTEMPO, argv[++argn]);
		if(strcmp(argv[argn], "-T")==0 && (argn+1) < argc) strcpy(ARGTITLE, argv[++argn]);
		if(strcmp(argv[argn], "-b")==0 && (argn+1) < argc) strcpy(ARGBEATS, argv[++argn]);
		if(strcmp(argv[argn], "-w")==0 && (argn+1) < argc) strcpy(ARGNOTEWID, argv[++argn]);
		if(strcmp(argv[argn], "-k")==0 && (argn+1) < argc) strcpy(ARGSONGKEY, argv[++argn]);
		if(strcmp(argv[argn], "-C")==0 && (argn+1) < argc) strcpy(ARGCOMPOSER, argv[++argn]);
		if(strcmp(argv[argn], "-W")==0 && (argn+1) < argc) strcpy(ARGWORDS, argv[++argn]);
		if(strcmp(argv[argn], "-c")==0 && (argn+1) < argc) strcpy(ARGCLEF, argv[++argn]);

		if(strcmp(argv[argn], "-n")==0 && (argn+1) < argc) BARSPERSONG=atoi(argv[++argn]);
		if(strcmp(argv[argn], "-o")==0 && (argn+1) < argc) strcpy(ALLFILE, argv[++argn]);

		if(strcmp(argv[argn], "-l")==0 && (argn+1) < argc) SPLITMLEVEL=atoi(argv[++argn]);
		if(strcmp(argv[argn], "-d")==0 && (argn+1) < argc) DISPERSION=atoi(argv[++argn]);
		if(strcmp(argv[argn], "--min")==0 && (argn+1) < argc) MINNOTE=atoi(argv[++argn]);
		if(strcmp(argv[argn], "--max")==0 && (argn+1) < argc) MAXNOTE=atoi(argv[++argn]);
		if(strcmp(argv[argn], "--seed")==0 && (argn+1) < argc) SEEDSTRT=atoi(argv[++argn]);
		if(strcmp(argv[argn], "--stage")==0 && (argn+1) < argc) GENSTAGE=atoi(argv[++argn]);

		if(strcmp(argv[argn], "--triplets")==0) TRIPLETS=1;
		if(strcmp(argv[argn], "--dots")==0) BROKENTS=1;
		if(strcmp(argv[argn], "--rests")==0) RESTNTES=1;
		if(strcmp(argv[argn], "--sharps")==0) GENSHRPS=1;
		if(strcmp(argv[argn], "--flats")==0) GENFLATS=1;
		if(strcmp(argv[argn], "--legato")==0) GENLEGAT=1;
		if(strcmp(argv[argn], "--nometro")==0) GENMETRO=0;
	}
	BEATSPERBAR=atoi(ARGBEATS);
	BEATLONGBAR=atoi(ARGNOTEWID);
	INITIALNOTE=MINNOTE + (MAXNOTE-MINNOTE)/2;

	srand(SEEDSTRT*3+1);

	strcpy(ABCFILE, ALLFILE); strcat(ABCFILE, ".abc");
	strcpy(PSFILE,  ALLFILE); strcat(PSFILE,  ".ps" );
	strcpy(PDFFILE, ALLFILE); strcat(PDFFILE, ".pdf");
	strcpy(MIDFILE, ALLFILE); strcat(MIDFILE, ".mid");
	strcpy(WAVFILE, ALLFILE); strcat(WAVFILE, ".wav");
	strcpy(MP3FILE, ALLFILE); strcat(MP3FILE, ".mp3");

	generate();
	return 0;
	free(ARGTEMPO);
}
