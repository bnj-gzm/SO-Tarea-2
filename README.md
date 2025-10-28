# Chat por Pipes (C++/UNIX)

Este proyecto, correspondiente a la Tarea N° 2 de Sistemas Operativos de la Universidad Diego Portales , implementa una simulación sincronizada basada en el clásico videojuego Doom.

El desarrollo se centra en la simulación de entidades concurrentes (Héroes y Monstruos) que interactúan en un grid 2D. Cada entidad es gestionada por un hilo (thread) independiente, y sus interacciones (movimiento, detección, alerta y combate) están controladas mediante herramientas de sincronización para evitar condiciones de carrera y deadlocks.

La simulación se inicializa a partir de un archivo de configuración que define el tamaño del mapa, las estadísticas y rutas del héroe , y las propiedades y posiciones de los monstruos.

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

El sistema implementa una simulación multi-hilo en un entorno UNIX. Está conformado por los siguientes componentes principales:


Héroe(s): Uno o más hilos (N threads)  que representan a los personajes principales. Cada héroe sigue una ruta predefinida en el grid y ataca a los monstruos que entran en su rango de ataque.


Monstruos: M hilos (M threads)  que representan a los enemigos. Inicialmente están en estado pasivo.

Lógica de Simulación:

Cuando un héroe entra en el VISION_RANGE de un monstruo, este se activa.

El monstruo activado alerta a todos los otros monstruos dentro de su propio rango de visión.

Todos los monstruos alertados comienzan a moverse hacia el héroe, siguiendo la ruta más corta (Distancia de Manhattan).

El héroe deja de moverse y entra en combate cuando hay monstruos en su rango de ataque. No reanuda su ruta hasta matar a todos los monstruos cercanos o morir.

La simulación finaliza cuando el héroe muere, llega al final de su ruta, o todos los monstruos son eliminados.
---

## Tecnologías utilizadas

- Entorno UNIX (Requisito obligatorio) 
- C++ (g++)
- make (Para la compilación) 
- pthreads (Para la gestión de hilos de héroes y monstruos) 
- Herramientas de Sincronización (Mutexes, Semáforos, etc., para proteger el acceso a variables globales como el grid, HP de las entidades, etc.)

---

##  Características

- Simulación Concurrente: Soporta N héroes y M monstruos, cada uno ejecutándose en su propio hilo.
- Parseo de Configuración: Lee y configura la simulación completa desde un archivo de texto de entrada.
- Movimiento por Ruta: El héroe sigue una secuencia de coordenadas (x,y) predefinida.
- Sistema de Combate: Héroes y monstruos tienen HP, ATTACK_DAMAGE y ATTACK_RANGE para gestionar el combate.
- IA de Monstruos (Visión y Alerta): Los monstruos tienen un VISION_RANGE que, al activarse, provoca una alerta en cadena a otros monstruos cercanos.
- Pathfinding de Monstruos: Los monstruos usan la Distancia de Manhattan para calcular la ruta más corta hacia el héroe una vez alertados.
- Gestión de Sincronización: Implementa mecanismos de exclusión mutua para evitar deadlocks, livelocks y condiciones de carrera en variables compartidas.
- Extensible: La simulación base de 1 héroe y M monstruos se extiende para soportar N héroes con rutas independientes.
---


## Configuración

### 1) Requisitos

Se requiere un entorno UNIX (como Ubuntu) con las herramientas de compilación básicas.
```
### 2) Crear la carpeta del proyector
```bash
mkdir -p ~/chat_udp
cd ~/chat_udp
```
### 2) Crear los archivos del proyecto
Cree una carpeta para el proyecto y coloque allí sus archivos fuente.
Todos los códigos de este proyecto se pueden encontrar al final en [Códigos](#códigos).


### 3) Crear archivo de configuración
Cree un archivo de configuración de prueba (ej. config.txt) basado en el ejemplo .

Pega el siguiente contenido (o el tuyo):

### 4) Compilar

Si su Makefile está configurado correctamente, la compilación es simple.

Se creará el binario:

- doom_sim (o el nombre que definas en tu Makefile)




### 5) Ejecutar
El programa debe recibir el archivo de configuración como argumento.


##  Pruebas Realizadas
Se deben realizar pruebas que validen:
- La correcta lectura de distintas configuraciones.
- El movimiento del héroe por la ruta especificada.
- La activación y persecución de los monstruos cuando el héroe entra en rango.
- La correcta ejecución del combate (reducción de HP, muerte de entidades).
- El funcionamiento de la alerta en cadena.
- La ejecución correcta con N héroes y M monstruos sin deadlocks.

Mensaje cliente 1: 
![Texto alternativo](cliente1enviandomensaje.png)



## Resultados

- El sistema funciona en Linux/WSL sin errores de ENXIO.

- Se logró comunicación estable, duplicación de procesos y expulsión por reportes.


## Contacto

Creado por:
- Benjamín Guzmán
- Martin Huiriqueo
