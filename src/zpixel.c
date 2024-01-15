#include <zpixel.h>

void minmaxrgb(rgbcolor color, unsigned char *min, unsigned char *max){
    *max = MAX(MAX(color.blue, color.red), MAX(color.red, color.green));
    *min = MIN(MIN(color.blue, color.red), MIN(color.blue, color.green));
}

zpixel * creerZpixel(unsigned int size, point pos, rgbcolor color){
    zpixel *ptr = malloc(sizeof(zpixel));
    if(ptr == NULL || pos.x < 0 || pos.y < 0){
        return NULL;
    }
    ptr->size = size;
    ptr->pos = pos;
    ptr->color = color;
    ptr->degradation = 0;
    return ptr;
}

zpixel * creerZpixelNoir(int size, point pos){
    return creerZpixel(size, pos, (rgbcolor) {0x00, 0x00, 0x00});
}

zpixel * creerZpixelBlanc(int size, point pos){
    return creerZpixel(size, pos, (rgbcolor) {0xFF, 0xFF, 0xFF});
}

unsigned char luminosite(zpixel * ptr){
    unsigned char min, max;
    minmaxrgb(ptr->color, &min, &max);
    return (min+max)/2;
}

unsigned char saturation(zpixel * ptr){
    unsigned char min, max;
    minmaxrgb(ptr->color, &min ,&max);
    if(luminosite(ptr) >= 128){
        return (unsigned char) (255 * ((double) (max-min) / (511 - (max+min))));
    } else {
        return (unsigned char) (255 * ((double) (max-min) / (max+min)));
    }
}

unsigned int distance(zpixel * ptr1, zpixel * ptr2){
    unsigned int distance = 0;
    distance = (unsigned int) sqrt(CARRE(ptr1->color.red - ptr2->color.red) + CARRE(ptr1->color.green - ptr2->color.green) + CARRE(ptr1->color.blue - ptr2->color.blue));
    return distance;
}

int projeter(zpixel * ptr, image * image){
    int index = 0;

    if(ptr == NULL || image == NULL || ptr->pos.x < 0 || ptr->pos.y < 0){
        return -1;
    }

    point pos = ptr->pos;

    //On redefinis size dans le cas d'un debordement
    int size_h = ptr->size, size_l = size_h;
    if(size_h + pos.y > image->hauteur){
        size_h -= size_h - (image->hauteur - pos.y);
    }
    if(size_l + pos.x > image->largeur){
        size_l -= size_l - (image->largeur - pos.x);
    }

    for(int i = 0; i<size_h; i++){
        for(int j = 0; j<size_l; j++){
            index = (j + pos.x) * 3 + (i + pos.y) * image->rowstride;
            image->contenu[index] = ptr->color.red;
            image->contenu[index+1] = ptr->color.green;
            image->contenu[index+2] = ptr->color.blue;
        }
    }

    return 0;
}

int afficher_zpixel(zpixel *ptr){
    if(ptr == NULL){
        return -1;
    }
    printf("       Infos zone de pixel: taille: %3d, Pos x: %3d, Pos y: %3d, RGB: (%3d,%3d,%3d)\n", ptr->size, ptr->pos.x, ptr->pos.y, ptr->color.red, ptr->color.green, ptr->color.blue);
    return 0;
}