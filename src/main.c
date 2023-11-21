#include <utils.h>
#include <zpixel.h>
#include <tree.h>
#include <degradation.h>

void finApropos(GtkWidget * widget, gpointer data);
void aboutButtonClicked(GtkWidget * widget, gpointer data);
void create_window(GtkApplication *application, gpointer user_data);

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
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(aboutButton, "activate", G_CALLBACK(aboutButtonClicked), builder);
    gtk_widget_show_all(window);
    gtk_main();
}

void aboutButtonClicked(GtkWidget * widget, gpointer data){
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkWidget *aboutWindow = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));
    GtkWidget *finAbout = GTK_WIDGET(gtk_builder_get_object(builder, "finapropos"));
    g_signal_connect(finAbout, "clicked", G_CALLBACK(finApropos), aboutWindow);
    gtk_widget_show_all(aboutWindow);
}

void finApropos(GtkWidget * widget, gpointer data){
    gtk_widget_hide(widget);
}