# StreamFinder - Base de Datos de Películas 

**Tarea 2 - Estructuras de Datos** 
**Pontificia Universidad Católica de Valparaíso**

##  Contribuciones del Equipo

* **Cris:** Desarrollo de la lógica del menú principal (`main.c`), implementación de las funciones de carga de datos, estructura base de `streamfinder.c`, implementación de las búsquedas por género, década, búsqueda avanzada, y manejo del control de versiones (resolución de conflictos en Git), implementación del sistema de la Watchlist (agregar, mostrar, eliminar).
* **Seba:** Implementación y ajuste de los Tipos de Datos Abstractos (TDAs List y Map), desarrollo de la lógica para la búsqueda parcial por director (`strstr`) y la función para calificar películas. 

##  Estado de la Aplicación y Limitaciones (Qué funciona y qué no)

Actualmente, **todas las opciones del menú (1 al 7) funcionan correctamente** y no presentan caídas (*Segmentation Fault*) bajo un uso normal. Sin embargo, existen las siguientes consideraciones/limitaciones conocidas:

* **Sensibilidad a Mayúsculas:** La "Búsqueda por Género" y la "Búsqueda Avanzada" requieren que la primera letra del género sea mayúscula (ej. debe ingresarse `Action`, no `action`). Si se ingresa en minúscula, indicará que no hay resultados.
* **Dependencia del Archivo:** El programa asume que el archivo `Top1500.csv` tiene un formato perfecto y está en la raíz. Si el archivo es modificado manualmente y pierde comas, la lectura podría desfasarse.
* **Persistencia de Datos:** La Watchlist y las calificaciones se guardan solo en la memoria (RAM) durante la ejecución. Al cerrar el programa (Opción 8), esos datos no se guardan permanentemente en el archivo CSV.

##  Cómo Ejecutar el Programa

Para compilar y ejecutar este proyecto, es obligatorio enlazar los archivos fuente de la raíz junto con las implementaciones de la carpeta `tdas`.

1. Abre la terminal y posiciónate en la carpeta raíz del proyecto (`streamfinder-tarea2`).
2. **Para compilar**, usa el siguiente comando:
   ```bash
   gcc *.c tdas/*.c -o tarea2
3. **Para ejecutar el programa**, usa el siguiente comando:
    ```bash
   ./streamfinder