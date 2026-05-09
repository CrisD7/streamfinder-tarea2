#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *directors;
  List *genres;
  int year;
  float rating;
  Map *user_ratings;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por Género");
  puts("3) Buscar por Director");
  puts("4) Buscar por Década");
  puts("5) Búsqueda Avanzada");
  puts("6) Gestionar Mi Watchlist");
  puts("7) Calificar Película");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
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

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
        cargar_peliculas(pelis_byid, pelis_bygenres);
        break;
    case '2':
        buscar_por_id(pelis_byid);
        break;
    case '3':
        break;
    case '4':
        break;
    case '5':
        break;
    case '6':
        limpiarPantalla();
        puts("========================================");
        puts("     Gestionar Mi Watchlist");
        puts("========================================");
        puts("1) Agregar");
        puts("2) Eliminar");
        puts("3) Mostrar");
        puts("4) Volver atrás");
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion2);
        switch (opcion2) {
            case '1':
                agregar_a_watchlist();
                break;
            case '2':
                eliminar_de_watchlist();
                break;
            case '3':
                mostrar_watchlist();
                break;
            case '4':
                continue;
            default:
                puts("Opcion no valida. Por favor, intente de nuevo.");
        }
        break;
    case '7':

      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}