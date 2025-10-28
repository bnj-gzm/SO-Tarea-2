# [cite\_start]Tarea N° 2: Simulación Concurrente (Sistemas Operativos) [cite: 2]

[cite\_start]Este proyecto es una simulación sincronizada a menor escala del videojuego clásico Doom, desarrollada para el curso de Sistemas Operativos[cite: 2, 6]. [cite\_start]La simulación maneja 1 Héroe y M Monstruos, donde cada entidad activa se ejecuta en su propio hilo (thread), gestionando sus acciones de movimiento y combate de forma concurrente[cite: 26, 33].

## Requisitos

[cite\_start]El programa está desarrollado en C++11 y debe ser compilado y ejecutado en un *ambiente UNIX* (como Ubuntu), lo cual es un requisito indispensable de la tarea[cite: 70].

  * g++ (con soporte para C++11)
  * make
  * pthread (para la compilación de hilos)

## Compilación

[cite\_start]El proyecto incluye un Makefile que facilita la compilación[cite: 68]. Para compilar el programa, simplemente ejecuta el siguiente comando en la terminal, dentro de la carpeta del proyecto:

bash
make


Esto generará un archivo ejecutable llamado simulador.

Para limpiar los archivos generados (borrar el ejecutable), puedes usar:

bash
make clean


## Ejecución

[cite\_start]Para ejecutar la simulación, debes pasarle como argumento la ruta al archivo de configuración[cite: 28, 67], como se muestra a continuación:

bash
./simulador config.txt


Si deseas compilar y ejecutar en un solo paso (como se definió en el Makefile), puedes usar:

bash
make run


## Explicación de Funcionamiento

[cite\_start]Esta sección explica brevemente cómo funciona internamente el código, cumpliendo con el requisito de la tarea[cite: 67].

### Concurrencia (Threads)

  * [cite\_start]Cada entidad activa (1 Héroe y M Monstruos) se ejecuta en su propio hilo (std::thread)[cite: 26, 33].
  * Esto permite que todas las entidades "piensen" y actúen de forma concurrente, en lugar de esperar a que termine el turno del anterior.
  * El hilo principal (main) se encarga de lanzar todos los hilos y luego esperar a que terminen usando thread::join().

### Sincronización (Mutex y Condition Variable)

Para evitar condiciones de carrera (race conditions) —por ejemplo, que dos monstruos ataquen al héroe exactamente al mismo tiempo y corrompan su HP— se utiliza un sistema de sincronización:

1.  **std::mutex (Candado):** Se usa un único candado global (sim_mutex). Cualquier hilo (Héroe o Monstruo) debe tomar este candado antes de leer o modificar cualquier dato compartido (el HP del héroe, la posición de un monstruo, etc.). Esto asegura que solo una entidad pueda alterar el estado del juego a la vez.

2.  **std::condition_variable (Campana):** Para evitar el busy-waiting (gastar CPU en un bucle while(true)), se implementa un sistema de turnos:

      * [cite\_start]El Héroe toma el candado, realiza su acción (atacar o moverse) y libera el candado[cite: 25].
      * Al terminar, el Héroe "despierta" a todos los monstruos usando sim_cv.notify_all().
      * Los hilos de los Monstruos pasan la mayor parte del tiempo "dormidos" (sim_cv.wait(lock)). Solo despiertan cuando el Héroe los notifica.
      * [cite\_start]Al despertar, cada monstruo (uno por uno, gracias al mutex) realiza su lógica (ver, alertar, moverse o atacar) [cite: 21, 27] y vuelve a dormir, esperando la siguiente señal del Héroe.

Este modelo asegura un orden lógico (Héroe actúa, luego Monstruos reaccionan) y es extremadamente eficiente, ya que los hilos no consumen CPU mientras esperan su turno.

## Autores

[cite\_start](La tarea se puede desarrollar individual o en parejas [cite: 66])
