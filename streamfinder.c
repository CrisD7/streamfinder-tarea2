#include "streamfinder.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirectors, const char *archivo_csv) {
    FILE *archivo = fopen(archivo_csv, "r");
    if (archivo == NULL) {
        return;
    }

    char **campos;
    campos = leer_linea_csv(archivo, ',');

    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Film *peli = (Film *)malloc(sizeof(Film));
        strcpy(peli->id, campos[1]);
        strcpy(peli->title, campos[5]);
        peli->rating = atof(campos[8]);
        peli->year = atoi(campos[10]);
        peli->genres = split_string(campos[11], ", ");
        peli->directors = split_string(campos[14], ", ");
        peli->user_ratings = map_create(is_equal_str);

        map_insert(pelis_byid, peli->id, peli);

        char *genre = list_first(peli->genres);
        while (genre != NULL) {
            MapPair *genre_pair = map_search(pelis_bygenres, genre);
            if (genre_pair == NULL) {
                List *new_list = list_create();
                list_pushBack(new_list, peli);
                map_insert(pelis_bygenres, genre, new_list);
            } else {
                List *genre_list = (List *)genre_pair->value;
                list_pushBack(genre_list, peli);
            }
            genre = list_next(peli->genres);
        }

        char *director = list_first(peli->directors);
        while (director != NULL) {
            MapPair *director_pair = map_search(pelis_bydirectors, director);
            if (director_pair == NULL) {
                List *new_list = list_create();
                list_pushBack(new_list, peli);
                map_insert(pelis_bydirectors, director, new_list);
            } else {
                List *director_list = (List *)director_pair->value;
                list_pushBack(director_list, peli);
            }
            director = list_next(peli->directors);
        }
    }
    fclose(archivo);
}

void mostrar_pelicula(Film *peli) {
    printf("Título: %s\n", peli->title);
    printf("Año: %d | Rating: %.1f\n", peli->year, peli->rating);

    printf("Directores: ");
    for (char *dir = list_first(peli->directors); dir != NULL; dir = list_next(peli->directors)) {
        printf("%s ", dir);
    }
    printf("\n");

    printf("Géneros: ");
    for (char *gen = list_first(peli->genres); gen != NULL; gen = list_next(peli->genres)) {
        printf("%s ", gen);
    }
    printf("\n");
    printf("Calificaciones de usuarios -> ");
    MapPair *rating_pair = map_first(peli->user_ratings);
    if (rating_pair == NULL) {
        printf("Nadie ha calificado esta película aún.");
    } else {
        while (rating_pair != NULL) {
            printf("%s: %d | ", (char *)rating_pair->key, *(int *)rating_pair->value);
            rating_pair = map_next(peli->user_ratings);
        }
    }
    printf("\n");
    puts("----------------------------------------");
}

void buscar_por_genero(Map *pelis_bygenres) {
    char genero[100];
    printf("Ingrese el género a buscar (ej. Drama, Action): ");
    scanf(" %[^\n]", genero);

    MapPair *pair = map_search(pelis_bygenres, genero);

    if (pair != NULL) {
        List *lista_peliculas = (List *)pair->value;
        printf("\n=== Películas del género: %s ===\n", genero);
        
        for (Film *peli = list_first(lista_peliculas); peli != NULL; peli = list_next(lista_peliculas)) {
            mostrar_pelicula(peli);
        }
    } else {
        printf("No se encontraron películas para el género '%s'.\n", genero);
    }
}

void buscar_por_director(Map *pelis_bydirectors) {
    char director[100];
    printf("Ingrese el nombre del director a buscar: ");
    scanf(" %[^\n]", director);

    MapPair *pair = map_search(pelis_bydirectors, director);

    if (pair != NULL) {
        List *lista_peliculas = (List *)pair->value;
        printf("\n=== Películas del director: %s ===\n", director);
        
        for (Film *peli = list_first(lista_peliculas); peli != NULL; peli = list_next(lista_peliculas)) {
            mostrar_pelicula(peli);
        }
    } else {
        printf("No se encontraron películas para el director '%s'.\n", director);
    }
}