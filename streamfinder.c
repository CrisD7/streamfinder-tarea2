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
    limpiarPantalla();
    FILE *archivo = fopen(archivo_csv, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
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
    puts("Archivo cargado con exito...");
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
        printf("N/A");
    } else {
        while (rating_pair != NULL) {
            printf("%s: %ld | ", (char *)rating_pair->key, (long)rating_pair->value);
            rating_pair = map_next(peli->user_ratings);
        }
    }
    printf("\n");
    puts("----------------------------------------");
}

void buscar_por_genero(Map *pelis_bygenres) {
    limpiarPantalla();
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
    limpiarPantalla();
    char director[100];
    printf("Ingrese el nombre del director a buscar (ej. Tarantino, Nolan): ");
    scanf(" %[^\n]", director);

    printf("\n=== Resultados para '%s' ===\n", director);
    int encontradas = 0;

    MapPair *pair = map_first(pelis_bydirectors);
    while (pair != NULL) {
        char *director_key = (char *)pair->key;
        
        if (strstr(director_key, director) != NULL) {
            List *lista_peliculas = (List *)pair->value;
            
            for (Film *peli = list_first(lista_peliculas); peli != NULL; peli = list_next(lista_peliculas)) {
                mostrar_pelicula(peli);
                encontradas++;
            }
        }
        pair = map_next(pelis_bydirectors);
    }
    if (encontradas == 0) {
        printf("No se encontraron películas para el director '%s'.\n", director);
        printf("(Nota: Recuerda respetar las mayúsculas iniciales, ej. 'Christopher' o 'Nolan').\n");
    }
}

void buscar_por_decada(Map *pelis_byid) {
    limpiarPantalla();
    int decada;
    printf("Ingrese la década a buscar (ej. 1990): ");
    scanf("%d", &decada);

    printf("\n=== Películas de la década de %d ===\n", decada);
    int encontradas = 0;

    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
        Film *peli = (Film *)pair->value;
        if (peli->year >= decada && peli->year < decada + 10) {
            mostrar_pelicula(peli);
            encontradas++;
        }
        
        pair = map_next(pelis_byid);
    }
    if (encontradas == 0) {
        printf("No se encontraron películas de esa década.\n");
    }
}

void busqueda_avanzada(Map *pelis_bygenres) {
    limpiarPantalla();
    char genero[100];
    int decada, encontradas = 0;

    printf("Ingrese el género a buscar (ej. Drama, Action): ");
    scanf(" %[^\n]", genero);
    
    MapPair *pair = map_search(pelis_bygenres, genero);
    if (pair == NULL) {
        printf("No se encontraron películas para el género '%s'.\n", genero);
        return;
    }

    printf("Ingrese la década a buscar (ej. 1990): ");
    scanf("%d", &decada);

    printf("\n=== Películas de %s de la década de %d ===\n", genero, decada);
    
    List *lista_peliculas = (List *)pair->value;
    for (Film *peli = list_first(lista_peliculas); peli != NULL; peli = list_next(lista_peliculas)) {
        if (peli->year >= decada && peli->year < decada + 10) {
            mostrar_pelicula(peli);
            encontradas++;
        }
    }
    if (encontradas == 0) {
        printf("No se encontraron películas de esa década.\n");
    }
}

void agregar_a_watchlist(Map *pelis_byid, List *watchlist) {
    limpiarPantalla();
    char id[100];
    printf("Ingrese el ID de la película a agregar (ej. tt0068646): ");
    scanf(" %99[^\n]", id);

    MapPair *pair = map_search(pelis_byid, id);
    if (pair != NULL) {
        Film *peli = (Film *)pair->value;
        list_pushBack(watchlist, peli);
        printf("¡La película '%s' se ha agregado a tu Watchlist!\n", peli->title);
    } else {
        printf("Error: No se encontró ninguna película con el ID '%s' en el catálogo.\n", id);
    }
}

void mostrar_watchlist(List *watchlist) {
    limpiarPantalla();
    if (list_first(watchlist) == NULL) {
        printf("Tu Watchlist está vacía. ¡Agrega algunas películas primero!\n");
        return;
    }

    printf("\n=== MI WATCHLIST ===\n");
    for (Film *peli = list_first(watchlist); peli != NULL; peli = list_next(watchlist)) {
        mostrar_pelicula(peli);
    }
}

void eliminar_de_watchlist(List *watchlist) {
    limpiarPantalla();
    if (list_first(watchlist) == NULL) {
        printf("Tu Watchlist ya está vacía.\n");
        return;
    }

    char id[100];
    printf("Ingrese el ID de la película a eliminar: ");
    scanf(" %99[^\n]", id);

    int encontrada = 0;
    for (Film *peli = list_first(watchlist); peli != NULL; peli = list_next(watchlist)) {
        if (strcmp(peli->id, id) == 0) {
            list_popCurrent(watchlist);
            printf("¡La película '%s' ha sido eliminada de tu Watchlist!\n", peli->title);
            encontrada = 1;
            break;
        }
    }

    if (!encontrada) {
        printf("La película con ID '%s' no estaba en tu Watchlist.\n", id);
    }
}