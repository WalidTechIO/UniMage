#include <utils.h>
#include <zpixel.h>
#include <tree.h>
#include <degradation.h>

typedef struct appdata {
    GtkLabel *labelOriginal;
    GtkLabel *labelTraitee;
    GtkImage *imgOriginal;
    GtkImage *imgTraitee;
    GtkRadioButton *moyenne;
    GtkRadioButton *delta;
    GtkRadioButton *seuil;
    GtkAdjustment *adjustment;
} appdata;

void create_window(GtkApplication *application, gpointer user_data);
void finApropos(GtkWidget * widget, gpointer data);
void aboutButtonClicked(GtkWidget * widget, gpointer data);
void openFile(GtkWidget * widget, gpointer data);
void saveAs(GtkWidget * widget, gpointer data);
void seuilPressed(GtkWidget * widget, gpointer data);
void moyennePressed(GtkWidget *widget, gpointer data);
void deltaPressed(GtkWidget *widget, gpointer data);
void adjustmentChanged(GtkWidget * widget, gpointer data);
void rendu(appdata * dt);

int main(int argc, char *argv[]){

    GtkApplication *app = gtk_application_new("un.image", G_APPLICATION_FLAGS_NONE);
    if (app == NULL)
        exit(1);

    g_signal_connect(app, "activate", G_CALLBACK(create_window), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return 0;
}

void create_window(GtkApplication *application, gpointer user_data){
    GtkBuilder *builder = gtk_builder_new_from_file("ui.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *quit = GTK_WIDGET(gtk_builder_get_object(builder, "menuQuit"));
    GtkWidget *aboutButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuApropos"));
    GtkWidget *openButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuOuvrir"));
    GtkWidget *saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "menuSave"));
    GtkWidget *aboutWindow = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));
    GtkWidget *finAbout = GTK_WIDGET(gtk_builder_get_object(builder, "finapropos"));


    GtkWidget *imgOriginal = GTK_WIDGET(gtk_builder_get_object(builder, "original"));
    GtkWidget *imgTraitee = GTK_WIDGET(gtk_builder_get_object(builder, "uniformisee"));
    GtkWidget *labelOriginal = GTK_WIDGET(gtk_builder_get_object(builder, "tailleOriginal"));
    GtkWidget *labelTraitee = GTK_WIDGET(gtk_builder_get_object(builder, "tailleUniformisee"));
    GtkWidget *moyenne = GTK_WIDGET(gtk_builder_get_object(builder, "moyenne"));
    GtkWidget *delta = GTK_WIDGET(gtk_builder_get_object(builder, "delta"));
    GtkWidget *seuil = GTK_WIDGET(gtk_builder_get_object(builder, "seuil"));
    GtkAdjustment *adjustment = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));

    appdata dt = { GTK_LABEL(labelOriginal), GTK_LABEL(labelTraitee), GTK_IMAGE(imgOriginal), GTK_IMAGE(imgTraitee), GTK_RADIO_BUTTON(moyenne), GTK_RADIO_BUTTON(delta), GTK_RADIO_BUTTON(seuil), adjustment};

    g_signal_connect(finAbout, "clicked", G_CALLBACK(finApropos), aboutWindow);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(aboutButton, "activate", G_CALLBACK(aboutButtonClicked), aboutWindow);
    g_signal_connect(openButton, "activate", G_CALLBACK(openFile), imgOriginal);
    g_signal_connect(saveButton, "activate", G_CALLBACK(saveAs), imgTraitee);
    g_signal_connect(seuil, "pressed", G_CALLBACK(seuilPressed), &dt);
    g_signal_connect(moyenne, "pressed", G_CALLBACK(moyennePressed), &dt);
    g_signal_connect(delta, "pressed", G_CALLBACK(deltaPressed), &dt);
    g_signal_connect(adjustment, "value_changed", G_CALLBACK(adjustmentChanged), &dt);
    gtk_widget_show_all(window);
    gtk_main();
}

void aboutButtonClicked(GtkWidget * widget, gpointer data){
    gtk_widget_show(GTK_WIDGET(data));
}

void finApropos(GtkWidget * widget, gpointer data){
    gtk_widget_hide(GTK_WIDGET(data));
}

void openFile(GtkWidget *widget, gpointer data){
    GtkWidget * originalImg = GTK_WIDGET(data);
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Sélectionner un fichier", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Choisir", NULL);
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_FILE_CHOOSER_ACTION_OPEN){
        gtk_image_set_from_file(GTK_IMAGE(originalImg), gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    }
    gtk_widget_destroy(dialog);
}

void saveAs(GtkWidget * widget, gpointer data){
    GtkWidget * traiteeImg = GTK_WIDGET(data);
    GdkPixbuf * pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(traiteeImg));
    image * img = NULL;
    if(pixbuf != NULL){
        img = creerImagePixbuf(pixbuf);
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Enregistrer sous", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "Sauvegarder", NULL);
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_FILE_CHOOSER_ACTION_OPEN){
            createBitmapFile(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)), img);
        }
        gtk_widget_destroy(dialog);
    }
}

void seuilPressed(GtkWidget *widget, gpointer data){
    appdata *dt = (appdata *)data;
    gtk_adjustment_set_value(dt->adjustment, 0);
    gtk_adjustment_set_upper(dt->adjustment, 255);
    rendu(dt);
}

void deltaPressed(GtkWidget *widget, gpointer data){
    appdata *dt = (appdata *)data;
    gtk_adjustment_set_value(dt->adjustment, 0);
    gtk_adjustment_set_upper(dt->adjustment, 1200);
    rendu(dt);
}

void moyennePressed(GtkWidget *widget, gpointer data){
    appdata *dt = (appdata *)data;
    gtk_adjustment_set_value(dt->adjustment, 0);
    gtk_adjustment_set_upper(dt->adjustment, 200);
    rendu(dt);
}

void adjustmentChanged(GtkWidget * widget, gpointer data){
    appdata *dt = (appdata *)data;
    rendu(dt);
}

void rendu(appdata * dt){
    GNode * node;
    GdkPixbuf * pixbuf = gdk_pixbuf_copy(gtk_image_get_pixbuf(dt->imgOriginal));
    image * img = creerImagePixbuf(pixbuf);
    int (*degradation)(GNode *, my_stack_t *);
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dt->seuil)) == TRUE){
        degradation = degradationLuminosite;
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dt->moyenne)) == TRUE)
    {
        degradation = degradationTaille;
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dt->delta)) == TRUE)
    {
        degradation = degradationDistance;
    }
    node = construire_arbre_zpixel(0, 0, MAX(img->largeur, img->hauteur), img, degradation);
    if(node != NULL){
        affiche_arbre(node, (unsigned int) gtk_adjustment_get_value(dt->adjustment), img);
        pixbuf = gdk_pixbuf_new_from_data(img->contenu, GDK_COLORSPACE_RGB, FALSE, 8, img->largeur, img->hauteur, img->rowstride, NULL, NULL);
        gtk_image_set_from_pixbuf(dt->imgTraitee, pixbuf);
        char* labelOriginalS = (char *) malloc(100*sizeof(char));
        char *labelTraiteeS = (char *)malloc(100 * sizeof(char));
        sprintf(labelOriginalS, "%d pixels", img->largeur * img->hauteur);
        sprintf(labelTraiteeS, "%d zone de pixels", g_node_n_nodes(node, G_TRAVERSE_ALL));
        gtk_label_set_text(dt->labelOriginal, labelOriginalS);
        gtk_label_set_text(dt->labelTraitee, labelTraiteeS);
    }
}