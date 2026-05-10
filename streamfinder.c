#include "streamfinder.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función para comparar las claves en tipo string en el mapa.
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

// Función para comparar las claves en tipo entero en el mapa.
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

// Función para cargar películas desde un archivo CSV
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirectors, const char *archivo_csv) {
    limpiarPantalla();
    // Se abre el archivo CSV en modo lectura
    FILE *archivo = fopen(archivo_csv, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    // Se declara un arreglo dinámico de cadenas para almaenar columnas, se lee una línea del archivo
    // usando la coma como delimitador, y se retorna un puntero a punteros.
    char **campos;
    campos = leer_linea_csv(archivo, ',');

    // Iteramos mientras haya líneas para leer en el archivo.
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        
        // Se reserva memoria para una nueva película y se copian los datos del archivo CSV.
        Film *peli = (Film *)malloc(sizeof(Film)); 
        strcpy(peli->id, campos[1]); 
        strcpy(peli->title, campos[5]); 
        peli->rating = atof(campos[8]); 
        peli->year = atoi(campos[10]); 
        peli->genres = split_string(campos[11], ", "); 
        peli->directors = split_string(campos[14], ", "); 
        peli->user_ratings = map_create(is_equal_str); 

        // Insertamos la película en el mapa por ID.
        map_insert(pelis_byid, peli->id, peli);

        // Recorremos la lista de películas, para cada género insertamos la película en el mapa por género.
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

        // Recorremos la lista de directores, para cada director insertamos la película en el mapa por director.
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
    fclose(archivo); // Cerramos el archivo CSV
    puts("Archivo cargado con exito...");
}

// Función para mostrar la información de una película.
void mostrar_pelicula(Film *peli) {
    printf("Título: %s\n", peli->title);
    printf("Año: %d | Rating: %.1f\n", peli->year, peli->rating);

    // Recorremos la lista de directores de la película y los mostramos.
    printf("Directores: ");
    for (char *dir = list_first(peli->directors); dir != NULL; dir = list_next(peli->directors)) {
        printf("%s ", dir);
    }
    printf("\n");

    // Recorremos la lista de géneros de la película y los mostramos.
    printf("Géneros: ");
    for (char *gen = list_first(peli->genres); gen != NULL; gen = list_next(peli->genres)) {
        printf("%s ", gen);
    }
    printf("\n");

    // Se busca la primeracalificación de usuario en el mapa de calificaciones, si no hay ninguna,
    // se muestra "N/A", si hay calificaciones, se recorren y se muestra cada una de ellas junto al
    // nombre del usuario correspondiente.
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

// Función para buscar películas según su género.
void buscar_por_genero(Map *pelis_bygenres) {
    limpiarPantalla();
    char genero[100];
    printf("Ingrese el género a buscar (ej. Drama, Action): ");
    scanf(" %[^\n]", genero);

    // Obtenemos la lista de películas para el género ingresado y la mostramos.
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

// Función para buscar películas según su director.
void buscar_por_director(Map *pelis_bydirectors) {
    limpiarPantalla();
    char director[100];
    printf("Ingrese el nombre del director a buscar (ej. Tarantino, Nolan): ");
    scanf(" %[^\n]", director);

    printf("\n=== Resultados para '%s' ===\n", director);
    int encontradas = 0;

    // Obtenemos la lista de películas para el director ingresado y la mostramos. 
    // Se recorre el mapa de directores, si el nombre del director coincide con el ingresado, 
    // se muestra la lista de películas asociada a ese director.
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
    // Si no se encontró ninguna película, se imprime un mensaje indicándolo.
    if (encontradas == 0) {
        printf("No se encontraron películas para el director '%s'.\n", director);
        printf("(Nota: Recuerda respetar las mayúsculas iniciales, ej. 'Christopher' o 'Nolan').\n");
    }
}

// Función para buscar películas según la década en la que fueron lanzadas.
void buscar_por_decada(Map *pelis_byid) {
    limpiarPantalla();
    int decada;
    printf("Ingrese la década a buscar (ej. 1990): ");
    scanf("%d", &decada);

    printf("\n=== Películas de la década de %d ===\n", decada);
    int encontradas = 0;

    // Iteramos desde el primer par del mapa de película por ID, si el año de lanzamiento de la
    // película pertenece a la década ingresada, mostramos la película.
    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
        Film *peli = (Film *)pair->value; //peli vaa ser el valor del par
        if (peli->year >= decada && peli->year < decada + 10) { // si el año de la peli pertenece a la década
            mostrar_pelicula(peli);
            encontradas++;
        }
        
        pair = map_next(pelis_byid);
    }
    // Si no se encontró ninguna película, se imprime un mensaje indicándolo.
    if (encontradas == 0) {
        printf("No se encontraron películas de esa década.\n");
    }
}

// Función para realizar una búsqueda avanzada combinando género y década.
void busqueda_avanzada(Map *pelis_bygenres) {
    limpiarPantalla();
    char genero[100];
    int decada, encontradas = 0;

    printf("Ingrese el género a buscar (ej. Drama, Action): ");
    scanf(" %[^\n]", genero); 
    
    MapPair *pair = map_search(pelis_bygenres, genero);
    // Si no hay películas en este género, salimos de la función.
    if (pair == NULL) {
        printf("No se encontraron películas para el género '%s'.\n", genero);
        return;
    }

    printf("Ingrese la década a buscar (ej. 1990): ");
    scanf("%d", &decada); 

    printf("\n=== Películas de %s de la década de %d ===\n", genero, decada);
    
    // Se recorre la lista de películas para el género ingresado, si el año de lanzamiento
    // pertenece a la década ingresada, se muestra la película.
    List *lista_peliculas = (List *)pair->value; 
    for (Film *peli = list_first(lista_peliculas); peli != NULL; peli = list_next(lista_peliculas)) {
        if (peli->year >= decada && peli->year < decada + 10) { // si pertenece a la década
            mostrar_pelicula(peli);
            encontradas++;
        }
    }
    // Si no se encontró ninguna película, se imprime un mensaje indicándolo.
    if (encontradas == 0) {
        printf("No se encontraron películas de esa década.\n");
    }
}

// Función para agregar una película a la Watchlist del usuario.
void agregar_a_watchlist(Map *pelis_byid, List *watchlist) {
    limpiarPantalla();
    char id[100];
    printf("Ingrese el ID de la película a agregar (ej. tt0068646): ");
    scanf(" %99[^\n]", id);

    // Buscamos la película por su ID en el mapa de películas por ID, si se encuentra,
    // se agrega a la Watchlist, si no, se imprime un mensaje indicando que no se encontró.
    MapPair *pair = map_search(pelis_byid, id);
    if (pair != NULL) {
        Film *peli = (Film *)pair->value;
        list_pushBack(watchlist, peli);
        printf("¡La película '%s' se ha agregado a tu Watchlist!\n", peli->title);
    } else {
        printf("Error: No se encontró ninguna película con el ID '%s' en el catálogo.\n", id);
    }
}

// Función para eliminar una película de la Watchlist del usuario.
void eliminar_de_watchlist(List *watchlist) {
    limpiarPantalla();
    
    // Comprobamos que la Watchlist no esté vacía, si lo está, se imprime un mensaje indicándolo 
    // y salimos de la función.
    if (list_first(watchlist) == NULL) {
        printf("Tu Watchlist ya está vacía.\n");
        return;
    }

    char id[100];
    printf("Ingrese el ID de la película a eliminar: ");
    scanf(" %99[^\n]", id);

    // Recorremos la Watchlist, si encontramos una película que su ID coincida con el ingresado, 
    // la eliminamos de la Watchlist.
    int encontrada = 0;
    for (Film *peli = list_first(watchlist); peli != NULL; peli = list_next(watchlist)) {
        if (strcmp(peli->id, id) == 0) {
            list_popCurrent(watchlist);
            printf("¡La película '%s' ha sido eliminada de tu Watchlist!\n", peli->title);
            encontrada = 1;
            break;
        }
    }

    // Si no se encontró ninguna película que coincida con el ID ingresado, se imprime un mensaje indicándolo.
    if (!encontrada) {
        printf("La película con ID '%s' no estaba en tu Watchlist.\n", id);
    }
}

// Función para mostrar las películas que se encuentran en la Watchlist del usuario.
void mostrar_watchlist(List *watchlist) {
    limpiarPantalla();
    
    // Comprobamos que la Watchlist no esté vacia, si lo está, se imprime un mensaje indicándolo
    // y salimos de la función.
    if (list_first(watchlist) == NULL) {
        printf("Tu Watchlist está vacía. ¡Agrega algunas películas primero!\n");
        return;
    }

    // Recorremos la Watchlist y mostramos cada película que se encuentra en ella.
    printf("\n=== MI WATCHLIST ===\n");
    for (Film *peli = list_first(watchlist); peli != NULL; peli = list_next(watchlist)) {
        mostrar_pelicula(peli);
    }
}

// Función para calificar una película.
void calificar_pelicula(Map *pelis_byid) {
    limpiarPantalla();
    char id[100]; 

    printf("Ingrese el ID de la película que desea calificar (ej. tt0068646): ");
    scanf(" %99[^\n]", id);

    // Buscamos la película por su ID en el mapa de películas por ID, si se encuentra, 
    // se solicita el nombre de usuario y la calificación, si no, se imprime indicando que no 
    // se encontró la película.
    MapPair *pair = map_search(pelis_byid, id);
    if (pair != NULL) {
        Film *peli = (Film *)pair->value;
        char usuario[100];
        
        printf("Ingrese su nombre de usuario: ");
        scanf(" %99[^\n]", usuario);

        unsigned short calificacion;
        printf("Ingrese su calificación para '%s' (1 al 10): ", peli->title);
        scanf("%hu", &calificacion);

        // Se solicita que la calificación esté entre 1 y 10.
        if (calificacion < 1 || calificacion > 10) {
            printf("Error: La calificación debe estar entre 1 y 10");
            return;
        }

        // Buscamos si el usuario ya ha calificado la película, si es así, actualizamos su calificación,
        // si no, agregamos la nueva calificación al mapa de calificaciones de la película.
        MapPair *rating_pair = map_search(peli->user_ratings, usuario);
        if (rating_pair != NULL) {
            rating_pair->value = (void *)(unsigned long)calificacion;
            printf("¡Tu calificación para '%s' fue actualizada a %hu!\n", peli->title, calificacion);
        }
        else {
            char *usuario_key = strdup(usuario);
            map_insert(peli->user_ratings, usuario_key, (void *)(unsigned long)calificacion);
            printf("¡Tu calificación de %hu para '%s' fue registrada!\n", calificacion, peli->title);
        }
    }
    else {
        printf("Error: No se encontró ninguna película con el ID %s en el catálogo.\n", id);
    }
    return;
}