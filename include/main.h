#include <gtk/gtk.h>
#include <utils.h>
#include <tree.h>
#include <degradation.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef linux
#include <unistd.h>
#endif

// Données d'etat d'application
typedef struct appdata
{
    GtkLabel *labelOriginal;         // Label sous l'image original
    GtkLabel *labelTraitee;          // Label sous l'image traitée
    GtkImage *imgOriginal;           // Image original
    GtkImage *imgTraitee;            // Image traitée
    GtkRadioButton *moyenne;         // Bouton moyenne
    GtkRadioButton *delta;           // Bouton delta
    GtkRadioButton *seuil;           // Bouton seuil
    GtkAdjustment *adjustment;       // Ajustement curseur
    GdkPixbuf *pixbufOriginalScaled; // Pixbuf original afficher sur l'UI (pe ref pixbufOriginal)
    GdkPixbuf *pixbufCalcul;         // Pixbuf de calcul de la preview
    GdkPixbuf *pixbufOriginal;       // Pixbuf original de l'image courante
    GNode *node;                     // Arbre determine selon f
    DegradationFunction f;           // Methode de degradation courante
    double ratio;                    // Ratio de retrecissement appliqué a la preview
} appdata;

/**
 * Construit l'UI, inisialise l'etat et demarre le mainloop
 */
void create_window(GtkApplication *application, gpointer user_data);
/**
 * Charge un fichier
 */
void loadFile(appdata *dt, char *filename);
/**
 * Applique une rotation de rota_multiple * 90° vers la gauche au pixbuf
 */
void rota_buffer(GdkPixbuf **pixbuf, int rota_multiple);
/**
 * Sauvegarde un fichier
 */
void saveFile(appdata *dt, char *filename, int mod);
/**
 * Met a jour l'ajustement selon une valeur max et assure la stabilité de celui-ci au changement de mode
 */
void change_mode(appdata *dt, int max);
/**
 * Evalue l'arbre node selon la methode de degradation courante
 */
void build(appdata *dt);
/**
 * Effectue le rendu de l'arbre node selon le seuil courant
 */
void rendu(appdata *dt);

// Signaux de l'application
void aboutButtonPressed(GtkWidget *widget, gpointer data);
void okAboutPressed(GtkWidget *widget, gpointer data);
void openFilePressed(GtkWidget *widget, gpointer data);
void saveAsPressed(GtkWidget *widget, gpointer data);
void seuilPressed(GtkWidget *widget, gpointer data);
void moyennePressed(GtkWidget *widget, gpointer data);
void deltaPressed(GtkWidget *widget, gpointer data);
void adjustmentChanged(GtkWidget *widget, gpointer data);