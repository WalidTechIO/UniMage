#include <main.h>

#define MAX_WIDTH_IMG 650 //Largeur maximale d'une image sur l'UI
#define MAX_HEIGHT_IMG 900 //Hauteur maximale d'une image sur l'UI

int main(int argc, char *argv[])
{
    //Définition de l'application selon l'OS
    char launchFile[500] = "";
    #ifdef _WIN32
        FreeConsole();
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
    g_clear_object(&app);
    
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
    GtkWidget *okAbout = GTK_WIDGET(gtk_builder_get_object(builder, "okApropos"));

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
    g_clear_object(&builder);

    //Construction d'une instance d'etat application
    appdata dt = {GTK_LABEL(labelOriginal), GTK_LABEL(labelTraitee), GTK_IMAGE(imgOriginal), GTK_IMAGE(imgTraitee), GTK_RADIO_BUTTON(moyenne), GTK_RADIO_BUTTON(delta), GTK_RADIO_BUTTON(seuil), adjustment, NULL, NULL, NULL, NULL, NULL, 1};

    //Définition mode d'entrée
    deltaPressed(NULL, &dt);

    //Si un fichier est spécifié definition en tant qu'original puis appel mode d'entrée
    if(strlen(user_data) > 0) {
        loadFile(&dt, user_data);
    }

    //Connexion des signaux
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);                   // Close request (OS)
    g_signal_connect(quit, "activate", G_CALLBACK(gtk_main_quit), NULL);                    // Bouton Quitter (Filebar)
    g_signal_connect(okAbout, "clicked", G_CALLBACK(okAboutPressed), aboutWindow);                 // Bouton OK (Fenetre a propos)
    g_signal_connect(aboutButton, "activate", G_CALLBACK(aboutButtonPressed), aboutWindow); // Bouton A propos (Filebar)
    g_signal_connect(openButton, "activate", G_CALLBACK(openFilePressed), &dt);                    // Bouton Ouvrir (Filebar)
    g_signal_connect(saveButton, "activate", G_CALLBACK(saveAsPressed), &dt);                      // Bouton Enregistrer sous (Filebar)
    g_signal_connect(seuil, "pressed", G_CALLBACK(seuilPressed), &dt);                      // Bouton seuil (Opération)
    g_signal_connect(delta, "pressed", G_CALLBACK(deltaPressed), &dt);                      // Bouton delta (Opération)
    g_signal_connect(moyenne, "pressed", G_CALLBACK(moyennePressed), &dt);                  // Bouton moyenne (Opération)
    g_signal_connect(adjustment, "value_changed", G_CALLBACK(adjustmentChanged), &dt);      // Ajustement curseur (Opération)

    //Affichage fenetre principale et lancement mainloop
    gtk_widget_show_all(window);
    gtk_main();

    if(dt.pixbufOriginalScaled != NULL) g_clear_object(&dt.pixbufOriginalScaled); //Destruction du pixbuf si existant
    if(dt.pixbufCalcul != NULL) g_clear_object(&dt.pixbufCalcul);
    if(dt.pixbufOriginal != NULL) g_clear_object(&dt.pixbufOriginal);

    detruire_arbre(&dt.node); //Destruction de l'arbre si existant
}

void loadFile(appdata * dt, char* filename)
{
    //On nettoie tout nos pixbuf et notre arbre
    g_clear_object(&dt->pixbufOriginal);
    g_clear_object(&dt->pixbufOriginalScaled);
    g_clear_object(&dt->pixbufCalcul);
    detruire_arbre(&dt->node);
    //On remet l'UI en etat "sans fichier"
    gtk_label_set_text(dt->labelOriginal, "Image original");
    gtk_label_set_text(dt->labelTraitee, "Image uniformisee");
    gtk_image_clear(dt->imgOriginal);
    gtk_image_clear(dt->imgTraitee);
    //On cree un pixbuf depuis le fichier charger et reinitialise le ratio
    dt->pixbufOriginal= gdk_pixbuf_new_from_file(filename,NULL);
    dt->ratio = 1;
    //Si l'image a correctement été chargé
    if(dt->pixbufOriginal != NULL){
        // On recupere sa hauteur et largeur puis on calcul le ratio de retrecissement
        // selon les constantes MAX_HEIGHT_IMG et MAX_WIDTH_IMG
        double height = (double) gdk_pixbuf_get_height(dt->pixbufOriginal);
        double width = (double) gdk_pixbuf_get_width(dt->pixbufOriginal);
        if(height > MAX_HEIGHT_IMG){
            dt->ratio = MAX_HEIGHT_IMG / height;
        }
        if(dt->ratio*width > MAX_WIDTH_IMG){
            dt->ratio = MAX_WIDTH_IMG / width;
        }
        //Si le ratio n'a pas changé on referencie pibxufOriginal en tant qu'originalScaled sinon on scale selon le ratio
        if(dt->ratio != 1) {
            dt->pixbufOriginalScaled = gdk_pixbuf_scale_simple(dt->pixbufOriginal, dt->ratio * width, dt->ratio * height, GDK_INTERP_HYPER);
        } else {
            dt->pixbufOriginalScaled = dt->pixbufOriginal;
        }
    }
    //Si le retrecissement/referencement a reussi on affiche le pixbufOriginalScaled puis on effectue la construction et le rendu
    if(dt->pixbufOriginalScaled != NULL) {
        gtk_image_set_from_pixbuf(dt->imgOriginal, dt->pixbufOriginalScaled);
        build(dt), rendu(dt);
    } else { //Sinon on affiche une erreur (MsgBox pour windows car on libere la console)
        #ifdef _WIN32
            MessageBox(NULL, "UniMage ERREUR: Format non pris en charge.", "Erreur", MB_OK);
        #endif
        fprintf(stderr, "UniMage ERREUR: Format non pris en charge.\n");
    }
}

void rota_buffer(GdkPixbuf **pixbuf, int rota_multiple)
{
    //Si le pixbuf n'est pas NULL et que rota_multiple est 1,2 ou 3
    if(pixbuf != NULL && *pixbuf != NULL && rota_multiple > 0 && rota_multiple < 4){
        GdkPixbuf *tmp = gdk_pixbuf_rotate_simple(*pixbuf, 90*rota_multiple); //On cree un nouveau pixbuf avec la rotation effectuée
        g_clear_object(pixbuf); //On libere l'ancien pixbuf
        *pixbuf = tmp; //On defini le pixbuf reçu comme etant le nouveau pixbuf
    }
}

void saveFile(appdata * dt, char* filename, int mod)
{
    char * tmp = NULL;
    if(strcmp(&filename[strlen(filename)-4], ".bmp") != 0){
        tmp = calloc(strlen(filename)+5, sizeof(char));
        if(tmp != NULL){
            sprintf(tmp, "%s.bmp", filename);
            filename = tmp;
        }
    }
    if(dt->pixbufOriginal != NULL){
        GdkPixbuf * sortie = gdk_pixbuf_copy(dt->pixbufOriginal); //On copie l'original
        rota_buffer(&sortie, mod); //On lui applique une rotation selon mod (les entrees sont defini dans l'evenement)
        if(sortie != NULL){  //Si la copie (et la rotation) a réussi 
            //On effectue un rendu une constructuion et un rendu selon le mode courant sur la copie de l'original pour ne pas perdre en qualité
            image *img = creerImagePixbuf(sortie);
            GNode * node = construire_arbre_zpixel(0, 0, MAX(img->largeur, img->hauteur), img, dt->f);
            affiche_arbre(node, gtk_adjustment_get_value(dt->adjustment), img);
            createBitmapFile(filename, img); //On cree le fichier
            //On libere l'arbre construit, le pixbuf sur lequel on a travaillé et l'interface image crée pour le lien
            detruire_arbre(&node);
            g_clear_object(&sortie);
            free(img);
        }
    } 
    if(tmp != NULL) free(tmp);
}

void change_mode(appdata * dt, int max)
{
    gdouble ratio = gtk_adjustment_get_value(dt->adjustment) / gtk_adjustment_get_upper(dt->adjustment); //Calcul du pourcentage actuel de l'ajustement
    gtk_adjustment_set_upper(dt->adjustment, max); //Definition du max demandé
    gtk_adjustment_set_value(dt->adjustment, ratio*max); //Définition de la valeur a partir du ratio de l'ancienne valeur sur l'ancien max facteur du nouveau max
    build(dt), rendu(dt); //On construit et on rend
}

void build(appdata * dt)
{
    //Destruction de l'arbre précédent et creation d'une interface image sur le pixbufOriginalScaled
    detruire_arbre(&dt->node);
    image *img = creerImagePixbuf(dt->pixbufOriginalScaled);
    if(img != NULL){
        dt->node = construire_arbre_zpixel(0, 0, MAX(img->largeur, img->hauteur), img, dt->f); //Construction de l'arbre selon fonction courante
        char labelOriginalS[100];
        char labelTraiteeS[100];
        sprintf(labelOriginalS, "%d pixels", img->largeur * img->hauteur); //Définition du nb de pixel de la version reduite
        sprintf(labelTraiteeS, "%d zone de pixels", g_node_n_nodes(dt->node, G_TRAVERSE_ALL)); //Definition du nombre de noeud de l'arbre de la version reduite
        gtk_label_set_text(dt->labelOriginal, labelOriginalS); //Affichage nb pixels original
        gtk_label_set_text(dt->labelTraitee, labelTraiteeS); //Affichage nb noeud arbre de rendu
    }
    free(img);
}

void rendu(appdata * dt)
{
    //Recuperation dans le pixbuf de calcul de preview du pixbufOriginalScaled
    if(dt->pixbufCalcul!=NULL) g_clear_object(&dt->pixbufCalcul);
    if(dt->pixbufOriginalScaled!=NULL) dt->pixbufCalcul = gdk_pixbuf_copy(dt->pixbufOriginalScaled);
    image *img = creerImagePixbuf(dt->pixbufCalcul); //Creation de notre interface image
    //Si interface OK et arbre courant OK
    if (img != NULL && dt->node != NULL)
    {
        affiche_arbre(dt->node, (unsigned int)gtk_adjustment_get_value(dt->adjustment), img); //Projection zpixel degradation inferieure au seuil
        gtk_image_set_from_pixbuf(dt->imgTraitee, dt->pixbufCalcul); //Definition du pixbufCalcul en tant que preview image traitee
    }
    free(img);
}


//Signaux

void okAboutPressed(GtkWidget * widget, gpointer data)
{
    gtk_widget_hide(GTK_WIDGET(data)); //Cacher fenetre a propos
}

void aboutButtonPressed(GtkWidget * widget, gpointer data)
{
    gtk_widget_show(GTK_WIDGET(data)); //Afficher fenetre a propos
}

//Bouton Ouvrir pressé
void openFilePressed(GtkWidget * widget, gpointer data)
{
    appdata *dt = (appdata *)data; //Recuperation etat de l'app
    //Lancement dialogue d'ouverture et appel a la procedure loadFile
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Sélectionner un fichier", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Choisir", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    switch(res){
        case GTK_RESPONSE_ACCEPT:
            loadFile(dt, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
            break;
        case GTK_RESPONSE_CANCEL:
            break;
    }
    gtk_widget_destroy(dialog);
}

//Bouton Enregistrer sous pressé
void saveAsPressed(GtkWidget * widget, gpointer data)
{
    appdata *dt = (appdata *) data; //Recuperation de l'etat de l'app
    //Si le pixbufOriginal existe
    if(dt->pixbufOriginal != NULL){
        //Lancement dialogue d'enregistrement et appel procédure saveFile
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Enregistrer sous", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "Sauvegarder", 0, "Appliquer rotation 90° droite", 3, "Appliquer rotation 90° gauche", 1, "Appliquer rotation 180°", 2 , "Annuler", GTK_RESPONSE_CANCEL, NULL);
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        switch (res){
            case 0:
            case 1:
            case 2:
            case 3:
                saveFile(dt, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)), res);
                break;
            case GTK_RESPONSE_CANCEL:
                break;
        }
        gtk_widget_destroy(dialog);
    }
}

void seuilPressed(GtkWidget *widget, gpointer data)
{
    appdata *dt = (appdata *)data; // Recuperation de l'etat de l'app
    dt->f = degradationLuminosite; // Definition de la methode de degradation
    change_mode(dt, 255); // Change le max sans changer le remplissage de l'ajustement, construit et rend
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
    appdata *dt = (appdata *)data; // Recuperation de l'etat de l'app
    rendu(dt); // Effectue un rendu
}