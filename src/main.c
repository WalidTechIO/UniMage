#include <utils.h>
#include <zpixel.h>
#include <tree.h>
#include <degradation.h>
#include <string.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#ifdef linux
    #include <unistd.h>
#endif

#define MAX_WIDTH_IMG 400
#define MAX_HEIGHT_IMG 650

//Données d'etat d'application
typedef struct appdata {
    GtkLabel *labelOriginal;    //Label sous l'image original
    GtkLabel *labelTraitee;     //Label sous l'image traitée
    GtkImage *imgOriginal;      //Image original
    GtkImage *imgTraitee;       //Image traitée
    GtkRadioButton *moyenne;    //Bouton moyenne
    GtkRadioButton *delta;      //Bouton delta
    GtkRadioButton *seuil;      //Bouton seuil
    GtkAdjustment *adjustment;  //Ajustement curseur
    GdkPixbuf *pixbuf;          //Pixbuf pour traitement
    GdkPixbuf *scaledO;         //Pixbuf original scalé
    GdkPixbuf *traitement;      //Pixbuf de travail
    GdkPixbuf *scaledT;         //Pixbuf de travail scalé
    GNode *node;                //Arbre determine selon f
    DegradationFunction f;      //Methode de degradation courante
    double ratio;
} appdata;


/**
 * Construit l'UI, inisialise l'etat et demarre le mainloop
*/
void create_window(GtkApplication *application, gpointer user_data);
/**
 * Met a jour l'ajustement selon une valeur max et assure la stabilité de celui-ci au changement de mode
*/
void change_mode(appdata * dt, int max);
/**
 * Defini le pixbuf de l'etat courant comme copie de l'image original
*/
void get_original_pixbuf(appdata *dt);
/**
 * Evalue l'arbre node selon la methode de degradation courante
*/
void build(appdata *dt);
/**
 * Effectue le rendu de l'arbre node selon le seuil courant
*/
void rendu(appdata * dt);

void loadFile(appdata * dt, char* filename);

//Signaux de l'application
void aboutButtonClicked(GtkWidget * widget, gpointer data);
void finApropos(GtkWidget * widget, gpointer data);
void openFile(GtkWidget * widget, gpointer data);
void saveAs(GtkWidget * widget, gpointer data);
void seuilPressed(GtkWidget * widget, gpointer data);
void moyennePressed(GtkWidget *widget, gpointer data);
void deltaPressed(GtkWidget *widget, gpointer data);
void adjustmentChanged(GtkWidget * widget, gpointer data);

int main(int argc, char *argv[])
{
    //Définition de l'application selon l'OS
    char launchFile[500] = "";
    #ifdef _WIN32
        //FreeConsole();
        GtkApplication *app = gtk_application_new("un.image", G_APPLICATION_DEFAULT_FLAGS);
    #endif
    #ifdef linux
        GtkApplication *app = gtk_application_new("un.image", G_APPLICATION_FLAGS_NONE);
    #endif
    if (app == NULL)
        exit(1);
    
    //Détermine si l'application doit demarrer avec un fichier selon l'OS
    if(argc > 1) {
        #ifdef _WIN32
            if(GetFileAttributes(argv[1]) != INVALID_FILE_ATTRIBUTES){
        #endif
        #ifdef linux
            if(access(argv[1], F_OK)==0){
        #endif
                if(strlen(argv[1]) > 490){
                    exit(1);
                }
                sprintf(launchFile, "%s", argv[1]);
            }
        argc--;
    }
    
    //Lancement de l'application et du mainloop
    g_signal_connect(app, "activate", G_CALLBACK(create_window), launchFile);
    g_application_run(G_APPLICATION(app), argc, argv);

    //Destruction de l'application et sortie du programme
    g_object_unref(app);
    
    return 0;
}

void create_window(GtkApplication *application, gpointer user_data){

    //Récupération du chemin du fichier Glade selon l'OS
    char uipath[500] = "";
    #ifdef _WIN32
        _pgmptr[strlen(_pgmptr)-14] = '\0';
        if(strlen(_pgmptr) > 490){
            MessageBox(NULL, "Impossible de charger l'interface utilisateur.", "Erreur", MB_OK);
            fprintf(stderr, "Impossible de charger l'interface utilisateur.\n");
            exit(1);
        }
        sprintf(uipath, "%sui.glade", _pgmptr);
    #endif
    #ifdef linux
        readlink("/proc/self/exe", uipath, sizeof(uipath)-1);
        uipath[strlen(uipath)-7] = '\0';
        if(strlen(uipath) > 490){
            fprintf(stderr, "Impossible de charger l'interface utilisateur.\n");
            exit(1);
        }
        sprintf(uipath, "%sui.glade", uipath);
    #endif

    //Construction du builder
    GtkBuilder *builder = gtk_builder_new_from_file(uipath);

    //Récupération des fenetres
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *aboutWindow = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));

    //Récupération des actions menu et bouton OK de la fenetre A propos
    GtkWidget *quit = GTK_WIDGET(gtk_builder_get_object(builder, "menuQuit"));
    GtkWidget *aboutButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuApropos"));
    GtkWidget *openButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuOuvrir"));
    GtkWidget *saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuSave"));
    GtkWidget *finAbout = GTK_WIDGET(gtk_builder_get_object(builder, "finapropos"));

    //Récupération des données utiles a l'etat de l'application
    GtkWidget *imgOriginal = GTK_WIDGET(gtk_builder_get_object(builder, "original"));
    GtkWidget *imgTraitee = GTK_WIDGET(gtk_builder_get_object(builder, "uniformisee"));
    GtkWidget *labelOriginal = GTK_WIDGET(gtk_builder_get_object(builder, "tailleOriginal"));
    GtkWidget *labelTraitee = GTK_WIDGET(gtk_builder_get_object(builder, "tailleUniformisee"));
    GtkWidget *moyenne = GTK_WIDGET(gtk_builder_get_object(builder, "moyenne"));
    GtkWidget *delta = GTK_WIDGET(gtk_builder_get_object(builder, "delta"));
    GtkWidget *seuil = GTK_WIDGET(gtk_builder_get_object(builder, "seuil"));
    GtkAdjustment *adjustment = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));

    //Liberation du builder
    g_object_unref(builder);

    //Construction d'une instance d'etat application
    appdata dt = {GTK_LABEL(labelOriginal), GTK_LABEL(labelTraitee), GTK_IMAGE(imgOriginal), GTK_IMAGE(imgTraitee), GTK_RADIO_BUTTON(moyenne), GTK_RADIO_BUTTON(delta), GTK_RADIO_BUTTON(seuil), adjustment, NULL, NULL, NULL, NULL, NULL, NULL, 1};

    //Définition mode d'entrée
    deltaPressed(NULL, &dt);

    //Si un fichier est spécifié definition en tant qu'original puis appel mode d'entrée
    if(strlen(user_data) > 0) {
        loadFile(&dt, user_data);
    }

    //Connexion des signaux
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);                   // Close request (OS)
    g_signal_connect(quit, "activate", G_CALLBACK(gtk_main_quit), NULL);                    // Bouton Quitter (Filebar)
    g_signal_connect(finAbout, "clicked", G_CALLBACK(finApropos), aboutWindow);             // Bouton OK (Fenetre a propos)
    g_signal_connect(aboutButton, "activate", G_CALLBACK(aboutButtonClicked), aboutWindow); // Bouton A propos (Filebar)
    g_signal_connect(openButton, "activate", G_CALLBACK(openFile), &dt);                    // Bouton Ouvrir (Filebar)
    g_signal_connect(saveButton, "activate", G_CALLBACK(saveAs), &dt);               // Bouton Enregistrer sous (Filebar)
    g_signal_connect(seuil, "pressed", G_CALLBACK(seuilPressed), &dt);                      // Bouton seuil (Opération)
    g_signal_connect(moyenne, "pressed", G_CALLBACK(moyennePressed), &dt);                  // Bouton moyenne (Opération)
    g_signal_connect(delta, "pressed", G_CALLBACK(deltaPressed), &dt);                      // Bouton delta (Opération)
    g_signal_connect(adjustment, "value_changed", G_CALLBACK(adjustmentChanged), &dt);      // Ajustement curseur (Opération)

    //Affichage fenetre principale et lancement mainloop
    gtk_widget_show_all(window);
    gtk_main();

    if(dt.pixbuf != NULL) g_clear_object(&dt.pixbuf); //Destruction du pixbuf si existant
    if(dt.traitement != NULL) g_clear_object(&dt.traitement);
    if(dt.scaledT != NULL) g_clear_object(&dt.scaledT);
    if(dt.scaledO != NULL) g_clear_object(&dt.scaledO);

    destroyNodeTree(dt.node, NULL); //Destruction de l'arbre si existant
}

void loadFile(appdata * dt, char* filename)
{
    g_clear_object(&dt->scaledO);
    g_clear_object(&dt->pixbuf);
    dt->pixbuf = gdk_pixbuf_new_from_file(filename,NULL);
    dt->ratio = 1;
    if(dt->pixbuf != NULL){
        double height = (double) gdk_pixbuf_get_height(dt->pixbuf);
        double width = (double) gdk_pixbuf_get_width(dt->pixbuf);
        if(height > MAX_HEIGHT_IMG){
            dt->ratio = MAX_HEIGHT_IMG / height;
        }
        if(dt->ratio*width > MAX_WIDTH_IMG){
            dt->ratio = MAX_WIDTH_IMG / width;
        }
        dt->scaledO = gdk_pixbuf_scale_simple(dt->pixbuf,dt->ratio*width,dt->ratio*height,GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(dt->imgOriginal, dt->scaledO);
    }
}

void change_mode(appdata * dt, int max)
{
    gdouble ratio = gtk_adjustment_get_value(dt->adjustment) / gtk_adjustment_get_upper(dt->adjustment);
    gtk_adjustment_set_upper(dt->adjustment, max);
    gtk_adjustment_set_value(dt->adjustment, ratio*max);
    build(dt);
    rendu(dt);
}

void get_original_pixbuf(appdata * dt)
{
    if(dt->traitement!=NULL) g_clear_object(&dt->traitement);
    if(dt->pixbuf!=NULL) dt->traitement = gdk_pixbuf_copy(dt->pixbuf);
}

void build(appdata * dt)
{
    destroyNodeTree(dt->node, NULL);
    image *img = creerImagePixbuf(dt->pixbuf);
    if(img != NULL){
        dt->node = construire_arbre_zpixel(0, 0, MAX(img->largeur, img->hauteur), img, dt->f);
        char labelOriginalS[100];
        char labelTraiteeS[100];
        sprintf(labelOriginalS, "%d pixels", img->largeur * img->hauteur);
        sprintf(labelTraiteeS, "%d zone de pixels", g_node_n_nodes(dt->node, G_TRAVERSE_ALL));
        gtk_label_set_text(dt->labelOriginal, labelOriginalS);
        gtk_label_set_text(dt->labelTraitee, labelTraiteeS);
    }
    free(img);
}

void rendu(appdata * dt)
{
    get_original_pixbuf(dt);
    image *img = creerImagePixbuf(dt->traitement);
    if (img != NULL && img->contenu != NULL && dt->node != NULL)
    {
        affiche_arbre(dt->node, (unsigned int)gtk_adjustment_get_value(dt->adjustment), img);
        g_clear_object(&dt->scaledT);
        dt->scaledT = gdk_pixbuf_scale_simple(dt->traitement, img->largeur*dt->ratio, img->hauteur*dt->ratio, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(dt->imgTraitee, dt->scaledT);
    }
    free(img);
}

void aboutButtonClicked(GtkWidget * widget, gpointer data)
{
    gtk_widget_show(GTK_WIDGET(data));
}

void finApropos(GtkWidget * widget, gpointer data)
{
    gtk_widget_hide(GTK_WIDGET(data));
}

void openFile(GtkWidget *widget, gpointer data)
{
    appdata *dt = (appdata *)data;
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Sélectionner un fichier", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Choisir", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
    gtk_image_clear(dt->imgTraitee);
    gtk_label_set_text(dt->labelOriginal, "Image original");
    gtk_label_set_text(dt->labelTraitee, "Image uniformisee");
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    switch(res){
        case GTK_RESPONSE_ACCEPT:
            loadFile(dt, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
            if(gtk_image_get_pixbuf(dt->imgOriginal)!=NULL){
                build(dt);
                rendu(dt);
            } else {
                #ifdef _WIN32
                    gtk_widget_destroy(dialog);
                    dialog = NULL;
                    MessageBox(NULL, "Format non pris en charge.", "Erreur", MB_OK);
                #endif
                fprintf(stderr, "Format non pris en charge.\n");
            }
            break;
        case GTK_RESPONSE_CANCEL:
            break;
    }
    gtk_widget_destroy(dialog);
}

void saveAs(GtkWidget * widget, gpointer data)
{
    appdata *dt = (appdata *) data;
    if(dt->traitement != NULL){
        image * img = creerImagePixbuf(dt->traitement);
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Enregistrer sous", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "Sauvegarder", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        switch (res){
            case GTK_RESPONSE_ACCEPT:
                createBitmapFile(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)), img);
                break;
            case GTK_RESPONSE_CANCEL:
                break;
        }
        gtk_widget_destroy(dialog);
        free(img);
    }
}

void seuilPressed(GtkWidget *widget, gpointer data)
{
    appdata *dt = (appdata *)data;
    dt->f = degradationLuminosite;
    change_mode(dt, 255);
}

void deltaPressed(GtkWidget *widget, gpointer data)
{
    appdata *dt = (appdata *)data;
    dt->f = degradationDistance;
    change_mode(dt, 1200);
}

void moyennePressed(GtkWidget *widget, gpointer data)
{
    appdata *dt = (appdata *)data;
    dt->f = degradationTaille;
    change_mode(dt, 200);
}

void adjustmentChanged(GtkWidget * widget, gpointer data)
{
    appdata *dt = (appdata *)data;
    rendu(dt);
}