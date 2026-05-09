#ifndef STREAMFINDER_H
#define STREAMFINDER_H
#include "tdas/list.h"
#include "tdas/map.h"

typedef struct {
  char id[100];
  char title[100];
  List *directors;
  List *genres;
  int year;
  float rating;
  Map *user_ratings;
} Film;

int is_equal_str(void *key1, void *key2);

int is_equal_int(void *key1, void *key2);

void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirectors, const char *archivo_csv);

// Busqueda
void buscar_por_genero(Map *pelis_bygenres);
void buscar_por_director(Map *pelis_bydirectors);
void buscar_por_decada(Map *pelis_byid);
void busqueda_avanzada(Map *pelis_bygenres);

// Gestionar Watchlist
void agregar_a_watchlist(Map *pelis_byid, List *watchlist);
void eliminar_de_watchlist(List *watchlist);
void mostrar_watchlist(List *watchlist);

void calificar_pelicula(Map *pelis_byid);

void mostrar_pelicula(Film *peli);

#endif