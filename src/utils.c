#include <utils.h>

int afficher_image(image *img)
{
    int index = 0;
    if (img == NULL || img->contenu == NULL)
        return -1;
    int gap = img->rowstride - (3 * img->largeur);
    printf("\n");
    for (int i = 0; i < img->largeur; i++)
    {
        printf("%17d", i);
    }
    if (gap)
        printf("        GAP\n");
    else
        printf("\n");
    for (int i = 0; i < img->hauteur; i++)
    {
        printf("%7d ", i);
        for (int j = 0; j < img->largeur; j++)
        {
            index = j * 3 + i * img->rowstride;
            printf("RGB(%3d,%3d,%3d) ", img->contenu[index], img->contenu[index + 1], img->contenu[index + 2]);
        }
        if (gap)
        {
            index = img->largeur * 3 + i * img->rowstride;
            int stop = (i + 1) * img->rowstride;
            for (int i = index; i < stop; i++)
            {
                printf("%02X", img->contenu[i]);
                if (i != stop - 1)
                    printf(":");
            }
        }
        printf("\n");
    }
    printf("\n       Infos image: largeur: %dpx, hauteur: %dpx, gap: %d octets\n\n", img->largeur, img->hauteur, gap);
    return 0;
}

int createBitmapFile(const char *filename, image *img)
{
    FILE *file = fopen(filename, "wb+");
    if (file == NULL || img == NULL || img->contenu == NULL){
        fprintf(stderr, "Erreur d'ouverture du fichier ou image NULL.\n");
        return -1;
    }
    int gap = img->rowstride - (3 * img->largeur);

    // En-tête de fichier (14 octets)
    unsigned char file_header[14] = {
        'B', 'M',   // Signature BMP
        0, 0, 0, 0, // Taille du fichier
        0, 0, 0, 0, // Réservé
        54, 0, 0, 0 // Offset de données de l'image
    };

    // En-tête d'image (40 octets)
    unsigned char image_header[40] = {
        40, 0, 0, 0, // Taille de l'en-tête d'image
        0, 0, 0, 0,  // Largeur
        0, 0, 0, 0,  // Hauteur
        1, 0,        // Planes (toujours 1)
        24, 0,       // Profondeur des couleurs (24 bits)
        0, 0, 0, 0,  // Compression (0 signifie aucune compression)
        0, 0, 0, 0,  // Taille de l'image en octets
        0, 0, 0, 0,  // Résolution horizontale en pixels par mètre (Non obligatoire)
        0, 0, 0, 0,  // Résolution verticale en pixels par mètre (Non obligatoire)
        0, 0, 0, 0,  // Couleurs de la palette (0 signifie aucune palette)
        0, 0, 0, 0   // Couleurs importantes (0 signifie toutes les couleurs sont importantes)
    };

    // Calcul de la taille de l'image en octets
    int image_size = img->rowstride * img->hauteur;
    int width = img->largeur;
    int height = img->hauteur;

    // Mise à jour des en-têtes avec les informations correctes
    file_header[2] = (image_size + 54) & 0xFF;
    file_header[3] = ((image_size + 54) >> 8) & 0xFF;
    file_header[4] = ((image_size + 54) >> 16) & 0xFF;
    file_header[5] = ((image_size + 54) >> 24) & 0xFF; //Taille image + en tete image + en tete fichier

    image_header[4] = width & 0xFF;
    image_header[5] = (width >> 8) & 0xFF;
    image_header[6] = (width >> 16) & 0xFF;
    image_header[7] = (width >> 24) & 0xFF; //Hauteur et largeur
    image_header[8] = height & 0xFF;
    image_header[9] = (height >> 8) & 0xFF;
    image_header[10] = (height >> 16) & 0xFF;
    image_header[11] = (height >> 24) & 0xFF;

    image_header[20] = image_size & 0xFF;
    image_header[21] = (image_size >> 8) & 0xFF; //Taille de l'image seulement
    image_header[22] = (image_size >> 16) & 0xFF;
    image_header[23] = (image_size >> 24) & 0xFF;

    // Écriture des en-têtes dans le fichier
    fwrite(file_header, sizeof(unsigned char), 14, file);
    fwrite(image_header, sizeof(unsigned char), 40, file);

    // Écriture des données de l'image dans le fichier
    //Parcours des lignes a l'envers et ecriture des composantes en ordre Little Endian
    for (int y = height - 1; y >= 0; y--){
        for (int x = 0; x < width; x++){
            int idx = (y * img->rowstride) + (x * 3);
            fwrite(img->contenu + idx + 2, sizeof(unsigned char), 1, file);
            fwrite(img->contenu + idx + 1, sizeof(unsigned char), 1, file);
            fwrite(img->contenu + idx, sizeof(unsigned char), 1, file);
        }
        fwrite(img->contenu + y * img->rowstride + width * 3, sizeof(unsigned char), gap, file); //Ecriture du gap a chaque fin de ligne
    }

    fclose(file); //Fermeture du fichier
    return 0;
}

image * creerImage(unsigned int width, unsigned int height){
    if(width<=0 || height <=0){
        return NULL;
    }
    unsigned int gap = 4 - (width * 3) % 4; if(gap == 4) gap = 0;
    unsigned int rowstride = width * 3 + gap;
    unsigned char *contenuImg = (unsigned char *) calloc(height * rowstride, sizeof(unsigned char));
    if(contenuImg == NULL){
        return NULL;
    }
    for(int i = 0; i < height; i++){
        int idx = i * rowstride + 3 * width;
        for(int y = idx; y<idx+gap; y++){
            contenuImg[y] = 0x00;
            if(y == idx+gap-1){
                contenuImg[y] = 0xFF;
            }
        }
    }
    image * img = (image * ) malloc(sizeof(image));
    if(img == NULL){
        free(contenuImg);
        return NULL;
    }
    img->contenu = contenuImg;
    img->rowstride = rowstride;
    img->largeur = width;
    img->hauteur = height;
    return img;
}

image *creerImagePixbuf(GdkPixbuf *pixbuf){
    if(pixbuf==NULL){
        return NULL;
    }
    image *img = (image *)malloc(sizeof(image));
    img->hauteur = gdk_pixbuf_get_height(pixbuf);
    img->largeur = gdk_pixbuf_get_width(pixbuf);
    img->rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    img->contenu = (unsigned char *)gdk_pixbuf_get_pixels(pixbuf);
    return img;
}

void destroyNodeTree(GNode *node, gpointer data)
{
    if (node == NULL)
    {
        return;
    }
    g_node_children_foreach(node, G_TRAVERSE_ALL, destroyNodeTree, NULL);
    free(node->data);
    g_node_destroy(node);
}