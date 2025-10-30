# Simulación Concurrente (Versión N-Héroes y Visual)

Este proyecto, correspondiente a la Tarea N° 2 de Sistemas Operativos, implementa una simulación sincronizada basada en el clásico videojuego Doom.

El desarrollo se centra en la simulación de entidades concurrentes (Héroes y Monstruos) que interactúan en un grid 2D. Cada entidad es gestionada por un hilo (thread) independiente, y sus interacciones (movimiento, detección, alerta y combate) están controladas mediante std::mutex y std::condition_variable para evitar condiciones de carrera.

Esta versión implementa la extensión de N Héroes y renderiza la simulación en tiempo real en la terminal utilizando la biblioteca ncurses.

## Tabla de contenido

- [Información general](#información-general)
- [Tecnologías utilizadas](#tecnologías-utilizadas)
- [Características](#características)
- [Configuración](#configuración)
- [Pruebas realizadas](#pruebas-realizadas)
- [Resultados](#resultados)
- [Contacto](#contacto)

---

## Información general

- El sistema implementa una simulación multi-hilo en un entorno UNIX. Está conformado por los siguientes componentes principales:

- Héroe(s): N hilos (std::thread) que representan a los personajes principales. Cada héroe sigue una ruta predefinida en el grid (config.txt) y ataca a los monstruos que entran en su rango de ataque.

- Monstruos: M hilos (std::thread) que representan a los enemigos. Inicialmente están en estado pasivo ("Durmiendo").

- Hilo de Renderizado: 1 hilo (std::thread) dedicado a dibujar el estado de la simulación en la terminal usando ncurses, actualizando la UI sin bloquear la lógica de simulación.

Lógica de Simulación:

- Cuando un héroe entra en el VISION_RANGE de un monstruo, este se activa.
- El monstruo activado alerta a todos los otros monstruos dentro de su propio rango de visión (VISION_RANGE).
- Todos los monstruos alertados comienzan a moverse hacia el héroe más cercano, siguiendo la ruta más corta (Distancia de Manhattan).
- El héroe deja de moverse y entra en combate si hay monstruos en su ATTACK_RANGE. No reanuda su ruta hasta matar a todos los monstruos cercanos o morir.
- La simulación finaliza cuando todos los héroes mueren o todos los monstruos son eliminados.
---

## Tecnologías utilizadas

- Entorno UNIX (Requisito obligatorio)
- C++11 (g++)
- make (Para la compilación)
- pthreads (std::thread) (Para la gestión de N+M+1 hilos)
- Herramientas de Sincronización (std::mutex, std::condition_variable)
- ncurses (libncurses5-dev) (Para la interfaz gráfica en terminal)
---

##  Características

- Simulación Concurrente: Soporta N héroes y M monstruos, cada uno ejecutándose en su propio hilo.
- Interfaz Gráfica en Terminal: Renderizado en tiempo real de la simulación usando la biblioteca ncurses, con ventanas separadas para el mapa y la UI de estado.
- Parseo de Configuración: Lee y configura la simulación completa (mapa, N héroes, M monstruos) desde un archivo config.txt.
- Movimiento por Ruta: Los héroes siguen una secuencia de coordenadas (x,y) predefinida.
- Sistema de Combate: Héroes y monstruos tienen HP, ATTACK_DAMAGE y ATTACK_RANGE.
- IA de Monstruos (Visión y Alerta): Los monstruos tienen un VISION_RANGE que, al activarse, provoca una alerta en cadena a otros monstruos cercanos.
- Pathfinding de Monstruos: Los monstruos usan la Distancia de Manhattan para calcular la ruta más corta hacia el héroe más cercano una vez alertados.
- Gestión de Sincronización: Implementa un std::mutex global y una std::condition_variable para coordinar los turnos de los monstruos y el dibujado, evitando condiciones de carrera.
---


## Configuración

### 1) Requisitos

Se requiere un entorno UNIX (como Ubuntu/Debian) con las herramientas de compilación g++, make y la biblioteca ncurses.

```bash
# Actualizar repositorios
sudo apt update

# Instalar ncurses (requisito para la UI visual)
sudo apt install libncurses5-dev

```
### 2) Crear los archivos del proyecto
Cree una carpeta para el proyecto y coloque los 3 archivos (Makefile, config.txt, main.cpp) en el mismo directorio.


### 3) Compilar
Con los 3 archivos en la misma carpeta, ejecute make para compilar.
```bash
# Compilar el proyecto
make
Esto utilizará el Makefile para compilar main.cpp y creará el ejecutable simulador_visual_N, enlazando las librerías pthread y ncurses.
```

### 4) Ejecutar

El programa recibe el archivo de configuración como argumento.
```bash
# Ejecutar la simulación
./simulador_visual_N config.txt
```
La terminal se limpiará y mostrará la simulación gráfica. Presione 'q' en cualquier momento para salir.




##  Pruebas Realizadas
- La correcta lectura de distintas configuraciones (N héroes, M monstruos).
- El movimiento de los héroes por sus rutas especificadas.
- La activación y persecución de los monstruos cuando un héroe entra en rango.
- La correcta ejecución del combate (reducción de HP, muerte de entidades).
- El funcionamiento de la alerta en cadena entre monstruos.
- La ejecución correcta con N héroes y M monstruos sin deadlocks.
- La correcta visualización y actualización de la UI de ncurses.

Mensaje cliente 1: 
![Texto alternativo](cliente1enviandomensaje.png)



## Resultados

El programa compila y ejecuta una simulación visual en la terminal. Se renderiza el mapa, los héroes (mostrados como H1, H2, etc.) y los monstruos (M) en tiempo real.

Un panel de UI adjunto muestra el estado (HP actual/máximo y última acción) de todos los héroes y de los monstruos que han sido activados. La simulación finaliza correctamente mostrando un mensaje de "¡VICTORIA!" (si mueren todos los monstruos) o "GAME OVER" (si mueren todos los héroes).



## Contacto

Creado por:
- Benjamín Guzmán
- Martin Huiriqueo
