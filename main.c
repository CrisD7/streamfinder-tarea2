#include "streamfinder.h"
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
  char opcion, opcion2;

  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirectors = map_create(is_equal_str);
  List *watchlist = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
        cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirectors, "Top1500.csv");
        break;
    case '2':
        buscar_por_genero(pelis_bygenres);
        break;
    case '3':
        buscar_por_director(pelis_bydirectors);
        break;
    case '4':
        buscar_por_decada(pelis_byid);
        break;
    case '5':
        busqueda_avanzada(pelis_bygenres);
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
                agregar_a_watchlist(pelis_byid, watchlist);
                break;
            case '2':
                eliminar_de_watchlist(watchlist);
                break;
            case '3':
                mostrar_watchlist(watchlist);
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