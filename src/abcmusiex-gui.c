#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libconfig.h>

FILE *configfile;

GtkBuilder	*builder;
GtkWidget	*eTitle, *eAuthor, *eComments, *eOutput, 
		*sTempo, *sQuantity, *sDispersion, *sMin, *sMax,
		*xTriplets, *xDots, *xMetro, *xRests, *xLegato, *xSharps, *xFlats,
		*cLevel, *cKey, *cClef, *cBeats, *cWidth,
		*window;
char 		filename[256], dirname[240];
//const gchar 	*text;
int	Tempo=0, Quantity=0, Dispersion=0, Min=0, Max=0;
int	Triplets=0, Dots=0, Metro=0, Rests=0, Legato=0, Sharps=0, Flats=0;
int	Level=0, Key=0, Clef=0, Beats=0, Width=0;
const char 	*Title=NULL;
const char 	*Author=NULL;
const char 	*Comments=NULL;
const char 	*Output=NULL;
 
int readConfig(){
	config_t cfg, *cf;
	const config_setting_t *retries;
	const char *base = NULL;
	int count, n, enabled;
	cf = &cfg;
	config_init(cf);
	config_read_file(cf, filename);
	if (config_lookup_int(cf, "Tempo", &Tempo)) 		gtk_spin_button_set_value(GTK_SPIN_BUTTON(sTempo), 		Tempo);
	if (config_lookup_int(cf, "Quantity", &Quantity)) 	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sQuantity), 		Quantity);
	if (config_lookup_int(cf, "Dispersion", &Dispersion)) 	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sDispersion), 	Dispersion);
	if (config_lookup_int(cf, "Min", &Min)) 		gtk_spin_button_set_value(GTK_SPIN_BUTTON(sMin), 		Min);
	if (config_lookup_int(cf, "Max", &Max)) 		gtk_spin_button_set_value(GTK_SPIN_BUTTON(sMax), 		Max);

	if (config_lookup_int(cf, "Triplets", &Triplets)) 	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xTriplets),	Triplets);
	if (config_lookup_int(cf, "Dots", &Dots)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xDots), 		Dots);
	if (config_lookup_int(cf, "Metro", &Metro)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xMetro), 	Metro);
	if (config_lookup_int(cf, "Rests", &Rests)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xRests), 	Rests);
	if (config_lookup_int(cf, "Legato", &Legato)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xLegato), 	Legato);
	if (config_lookup_int(cf, "Sharps", &Sharps)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xSharps), 	Sharps);
	if (config_lookup_int(cf, "Flats", &Flats)) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(xFlats), 	Flats);

	if (config_lookup_int(cf, "Level", &Level)) 		gtk_combo_box_set_active(GTK_COMBO_BOX(cLevel),			Level);
	if (config_lookup_int(cf, "Key", &Key)) 		gtk_combo_box_set_active(GTK_COMBO_BOX(cKey), 			Key);
	if (config_lookup_int(cf, "Clef", &Clef)) 		gtk_combo_box_set_active(GTK_COMBO_BOX(cClef), 			Clef);
	if (config_lookup_int(cf, "Beats", &Beats)) 		gtk_combo_box_set_active(GTK_COMBO_BOX(cBeats), 		Beats);
	if (config_lookup_int(cf, "Width", &Width)) 		gtk_combo_box_set_active(GTK_COMBO_BOX(cWidth), 		Width);

	if (config_lookup_string(cf, "Title", &Title)) 		gtk_entry_set_text(GTK_ENTRY(eTitle), 				Title);
	if (config_lookup_string(cf, "Author", &Author)) 	gtk_entry_set_text(GTK_ENTRY(eAuthor), 				Author);
	if (config_lookup_string(cf, "Comments", &Comments)) 	gtk_entry_set_text(GTK_ENTRY(eComments), 			Comments);
	if (config_lookup_string(cf, "Output", &Output)) 	gtk_entry_set_text(GTK_ENTRY(eOutput), 				Output);
	config_destroy(cf);
	return 0;
}
void saveConfig(){
	mkdir(dirname, 0777);
	configfile=fopen(filename,  "w");
	fprintf(configfile, "# authenticator\n");

	fprintf(configfile, "Tempo = %i;\n",		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sTempo)));
	fprintf(configfile, "Quantity = %i;\n",		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sQuantity)));
	fprintf(configfile, "Dispersion = %i;\n",	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sDispersion)));
	fprintf(configfile, "Min = %i;\n",		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sMin)));
	fprintf(configfile, "Max = %i;\n",		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sMax)));

	fprintf(configfile, "Triplets = %i;\n",		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xTriplets)));
	fprintf(configfile, "Dots = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xDots)));
	fprintf(configfile, "Metro = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xMetro)));
	fprintf(configfile, "Rests = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xRests)));
	fprintf(configfile, "Legato = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xLegato)));
	fprintf(configfile, "Sharps = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xSharps)));
	fprintf(configfile, "Flats = %i;\n", 		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xFlats)));

	fprintf(configfile, "Level = %i;\n",		gtk_combo_box_get_active(GTK_COMBO_BOX(cLevel))); 
	fprintf(configfile, "Key = %i;\n", 		gtk_combo_box_get_active(GTK_COMBO_BOX(cKey)));
	fprintf(configfile, "Clef = %i;\n", 		gtk_combo_box_get_active(GTK_COMBO_BOX(cClef)));
	fprintf(configfile, "Beats = %i;\n", 		gtk_combo_box_get_active(GTK_COMBO_BOX(cBeats)));
	fprintf(configfile, "Width = %i;\n", 		gtk_combo_box_get_active(GTK_COMBO_BOX(cWidth)));

	fprintf(configfile, "Title = \"%s\";\n",	gtk_entry_get_text(GTK_ENTRY(eTitle )));
	fprintf(configfile, "Author = \"%s\";\n",	gtk_entry_get_text(GTK_ENTRY(eAuthor)));
	fprintf(configfile, "Comments = \"%s\";\n",	gtk_entry_get_text(GTK_ENTRY(eComments)));
	fprintf(configfile, "Output = \"%s\";\n",	gtk_entry_get_text(GTK_ENTRY(eOutput)));
	fclose(configfile);
}
char* fixValue(char *string){
	return string;
}
void on_window1_destroy (GtkObject *object, gpointer user_data) {
	gtk_main_quit();
}
void on_bClose_clicked (GtkButton *button, gpointer user_data) {
	gtk_main_quit();
}
void on_bOk_clicked (GtkButton *button, gpointer user_data) {
	saveConfig();
	char cmd[256], syscommand[1024];
	strcpy(syscommand, "abcmusiex ");

	sprintf(cmd, "-t %i ", 		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sTempo	))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-n %i ", 		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sQuantity))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-d %i ", 		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sDispersion)));strcat(syscommand, cmd);
	sprintf(cmd, "--min %i ", 	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sMin	))); 	strcat(syscommand, cmd);
	sprintf(cmd, "--max %i ",	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sMax	))); 	strcat(syscommand, cmd);

	sprintf(cmd, "-l %.*s ", 1,	gtk_combo_box_get_active_text(GTK_COMBO_BOX(cLevel	))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-k \"%s\" ", 	gtk_combo_box_get_active_text(GTK_COMBO_BOX(cKey	))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-c %s ", 		gtk_combo_box_get_active_text(GTK_COMBO_BOX(cClef	)));	strcat(syscommand, cmd);
	sprintf(cmd, "-b %s ", 		gtk_combo_box_get_active_text(GTK_COMBO_BOX(cBeats	))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-w %s ", 		gtk_combo_box_get_active_text(GTK_COMBO_BOX(cWidth	))); 	strcat(syscommand, cmd);

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xTriplets))==1) 				strcat(syscommand, "--triplets ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xDots))==1) 					strcat(syscommand, "--dots ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xMetro))==0) 					strcat(syscommand, "--nometro ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xRests))==1) 					strcat(syscommand, "--rests ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xLegato))==1) 				strcat(syscommand, "--legato ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xSharps))==1) 				strcat(syscommand, "--sharps ");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(xFlats))==1) 					strcat(syscommand, "--flats ");

	sprintf(cmd, "-T \"%s\" ", gtk_entry_get_text(GTK_ENTRY(eTitle   			))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-C \"%s\" ", gtk_entry_get_text(GTK_ENTRY(eAuthor  			))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-W \"%s\" ", gtk_entry_get_text(GTK_ENTRY(eComments			))); 	strcat(syscommand, cmd);
	sprintf(cmd, "-o \"%s\" ", gtk_entry_get_text(GTK_ENTRY(eOutput  			))); 	strcat(syscommand, cmd);

	printf("%s\n", syscommand);
	system(syscommand);
}
void on_bSave_clicked (GtkButton *button, gpointer user_data) {
	saveConfig();
}
void on_bSsh_clicked (GtkButton *button, gpointer user_data) {
	system("pkill timidity");
}
int main (int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	builder = gtk_builder_new ();
	if(!gtk_builder_add_from_file (builder, "abcmusiex-gui.gtk", NULL))
		gtk_builder_add_from_file (builder, "/usr/share/abcmusiex/abcmusiex-gui.gtk", NULL);

	sprintf(filename, "%s/.abcmusiex/abcmusiex.conf", getenv("HOME"));
	sprintf( dirname, "%s/.abcmusiex/", getenv("HOME"));
 
	window=GTK_WIDGET (gtk_builder_get_object (builder, "window1"));

	sTempo		=GTK_WIDGET (gtk_builder_get_object (builder, "sTempo"));
	sQuantity	=GTK_WIDGET (gtk_builder_get_object (builder, "sQuantity"));
	sDispersion	=GTK_WIDGET (gtk_builder_get_object (builder, "sDispersion"));
	sMin		=GTK_WIDGET (gtk_builder_get_object (builder, "sMin"));
	sMax		=GTK_WIDGET (gtk_builder_get_object (builder, "sMax"));

	xTriplets	=GTK_WIDGET (gtk_builder_get_object (builder, "xTriplets"));
	xDots 		=GTK_WIDGET (gtk_builder_get_object (builder, "xDots"));
	xMetro 		=GTK_WIDGET (gtk_builder_get_object (builder, "xMetro"));
	xRests 		=GTK_WIDGET (gtk_builder_get_object (builder, "xRests"));
	xLegato 	=GTK_WIDGET (gtk_builder_get_object (builder, "xLegato"));
	xSharps 	=GTK_WIDGET (gtk_builder_get_object (builder, "xSharps"));
	xFlats 		=GTK_WIDGET (gtk_builder_get_object (builder, "xFlats"));

	cLevel		=GTK_WIDGET (gtk_builder_get_object (builder, "cLevel"));
	cKey 		=GTK_WIDGET (gtk_builder_get_object (builder, "cKey"));
	cClef 		=GTK_WIDGET (gtk_builder_get_object (builder, "cClef"));
	cBeats 		=GTK_WIDGET (gtk_builder_get_object (builder, "cBeats"));
	cWidth 		=GTK_WIDGET (gtk_builder_get_object (builder, "cWidth"));

	eTitle		=GTK_WIDGET (gtk_builder_get_object (builder, "eTitle"));
	eAuthor		=GTK_WIDGET (gtk_builder_get_object (builder, "eAuthor"));
	eComments	=GTK_WIDGET (gtk_builder_get_object (builder, "eComments"));
	eOutput		=GTK_WIDGET (gtk_builder_get_object (builder, "eOutput"));

	readConfig();

	gtk_builder_connect_signals (builder, NULL);
	g_object_unref (G_OBJECT (builder));
	gtk_widget_show (window);
	gtk_main ();
	return 0;
}
