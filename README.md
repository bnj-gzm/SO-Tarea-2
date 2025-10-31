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
![Simulación en acción](Prueba.gif)



## Resultados

El programa compila y ejecuta una simulación visual en la terminal. Se renderiza el mapa, los héroes (mostrados como H1, H2, etc.) y los monstruos (M) en tiempo real.

Un panel de UI adjunto muestra el estado (HP actual/máximo y última acción) de todos los héroes y de los monstruos que han sido activados. La simulación finaliza correctamente mostrando un mensaje de "¡VICTORIA!" (si mueren todos los monstruos) o "GAME OVER" (si mueren todos los héroes).

## Códigos

- config.txt (ejemplo de formato)
```bash
GRID_SIZE 30 20  
HERO_1_HP 150  
HERO_1_ATTACK_DAMAGE 20  
HERO_1_ATTACK_RANGE 3  
HERO_1_START 2 2  
HERO_1_PATH (3,2) (4,2) (5,2) (5,3) (5,4) (6,4)  
MONSTER_COUNT 3  
MONSTER_1_HP 50  
MONSTER_1_ATTACK_DAMAGE 10  
MONSTER_1_VISION_RANGE 5  
MONSTER_1_ATTACK_RANGE 1  
MONSTER_1_COORDS 8 4  
MONSTER_2_HP 50  
MONSTER_2_ATTACK_DAMAGE 10  
MONSTER_2_VISION_RANGE 5  
MONSTER_2_ATTACK_RANGE 1  
MONSTER_2_COORDS 15 10  
MONSTER_3_HP 80  
MONSTER_3_ATTACK_DAMAGE 15  
MONSTER_3_VISION_RANGE 4  
MONSTER_3_ATTACK_RANGE 2  
MONSTER_3_COORDS 5 8
```

- main.cpp
```bash

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ncurses.h>

#define UI_W 50
#define HERO_PAIR 1
#define MONSTER_PAIR 2
#define UI_PAIR 3
#define GAMEOVER_PAIR 4

std::mutex sim_mutex;
bool simulation_running = true;

struct Coords { int x = 0, y = 0; };

struct HeroConfig {
    int id = 0;
    int hp = 0;
    int attack_damage = 0;
    int attack_range = 0;
    Coords start;
    std::vector<Coords> path;
};

struct MonsterConfig {
    int id = 0;
    int hp = 0;
    int attack_damage = 0;
    int vision_range = 0;
    int attack_range = 0;
    Coords coords;
};

struct SimulationConfig {
    Coords grid_size;
    std::vector<HeroConfig> heroes;
    std::vector<MonsterConfig> monsters;
};

SimulationConfig parseConfig(const std::string& filename) {
    SimulationConfig config;
    std::ifstream file(filename);
    std::string line;
    int monster_count = 0;
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de configuración: " << filename << std::endl;
        exit(1); 
    }
    
    std::cout << "Parseando archivo " << filename << "..." << std::endl;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; 
        std::stringstream ss(line);
        std::string key;
        ss >> key;
        
        if (key == "GRID_SIZE") {
            ss >> config.grid_size.x >> config.grid_size.y;
        } 
        else if (key == "MONSTER_COUNT") {
            ss >> monster_count;
            config.monsters.resize(monster_count);
        } 
        else if (key.rfind("HERO_", 0) == 0) {
            size_t id_start = 5;
            size_t id_end = key.find('_', id_start);
            if (id_end == std::string::npos) continue;
            
            int id = std::stoi(key.substr(id_start, id_end - id_start));
            std::string sub_key = key.substr(id_end + 1);

            if (id > (int)config.heroes.size()) {
                config.heroes.resize(id);
            }
            HeroConfig& hero = config.heroes[id - 1];
            hero.id = id;

            if (sub_key == "PATH") {
                int x, y; char delim; 
                while (ss >> delim >> x >> delim >> y >> delim) {
                    Coords new_coord;
                    new_coord.x = x;
                    new_coord.y = y;
                    hero.path.push_back(new_coord);
                }
            } else if (sub_key == "START") {
                ss >> hero.start.x >> hero.start.y;
            } else {
                int value;
                ss >> value;
                if (sub_key == "HP") hero.hp = value;
                else if (sub_key == "ATTACK_DAMAGE") hero.attack_damage = value;
                else if (sub_key == "ATTACK_RANGE") hero.attack_range = value;
            }
        } 
        else if (key.rfind("MONSTER_", 0) == 0) {
            size_t id_start = 8;
            size_t id_end = key.find('_', id_start);
            if (id_end == std::string::npos) continue;
            
            int id = std::stoi(key.substr(id_start, id_end - id_start));
            std::string sub_key = key.substr(id_end + 1);
            
            if (id > (int)config.monsters.size() || id == 0) continue;
            
            MonsterConfig& monster = config.monsters[id - 1];
            monster.id = id;

            if (sub_key == "COORDS") {
                ss >> monster.coords.x >> monster.coords.y;
            } else {
                int value;
                ss >> value;
                if (sub_key == "HP") monster.hp = value;
                else if (sub_key == "ATTACK_DAMAGE") monster.attack_damage = value;
                else if (sub_key == "VISION_RANGE") monster.vision_range = value;
                else if (sub_key == "ATTACK_RANGE") monster.attack_range = value;
            }
        }
    }
    std::cout << "Parseo completo. Iniciando simulación..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return config;
}

class Simulation;
class Monster;

class Hero {
public:
    int id;
    int hp;
    int hp_max;
    int attack_damage;
    int attack_range;
    Coords current_coords;
    std::vector<Coords> path;
    size_t path_index; 
    bool is_alive;
    bool finished_path;
    std::string last_action = "Iniciando...";

    Hero(const HeroConfig& config) {
        id = config.id;
        hp = config.hp;
        hp_max = config.hp;
        attack_damage = config.attack_damage;
        attack_range = config.attack_range;
        current_coords = config.start;
        path = config.path;
        path_index = 0;
        is_alive = true;
        finished_path = false;
    }

    void run(Simulation* sim); 
};

class Monster {
public:
    int id;
    int hp;
    int hp_max;
    int attack_damage;
    int vision_range;
    int attack_range;
    Coords current_coords;
    bool is_active;
    bool is_alive;
    std::string last_action = "Durmiendo...";

    Monster(const MonsterConfig& config) {
        id = config.id;
        hp = config.hp;
        hp_max = config.hp;
        attack_damage = config.attack_damage;
        vision_range = config.vision_range;
        attack_range = config.attack_range;
        current_coords = config.coords;
        is_active = false; 
        is_alive = true;
    }

    void run(Simulation* sim);
};

class Simulation {
public:
    Coords grid_size; 
    std::vector<Hero> heroes; 
    std::vector<Monster> monsters; 

    Simulation(const SimulationConfig& config) {
        grid_size = config.grid_size;
        for (const auto& h_config : config.heroes) heroes.push_back(Hero(h_config));
        for (const auto& m_config : config.monsters) monsters.push_back(Monster(m_config));
    }

    int manhattanDistance(Coords a, Coords b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    bool allMonstersDead() {
        for(const auto& m : monsters) if(m.is_alive) return false;
        return true;
    }
    bool allHeroesDead() {
        for(const auto& h : heroes) if(h.is_alive) return false;
        return true;
    }
    bool allLivingHeroesFinishedPath() {
        bool at_least_one_hero_alive = false;
        for (const auto& h : heroes) {
            if (h.is_alive) {
                at_least_one_hero_alive = true;
                if (!h.finished_path) return false;
            }
        }
        return at_least_one_hero_alive;
    }
};

void Hero::run(Simulation* sim) {
    while (is_alive && simulation_running) {
        { 
            std::unique_lock<std::mutex> lock(sim_mutex);
            if (!is_alive || !simulation_running) break; 
            
            Monster* target = nullptr; 
            int min_dist = 9999;
            for (Monster& monster : sim->monsters) {
                if (monster.is_alive) {
                    int dist = sim->manhattanDistance(current_coords, monster.current_coords);
                    if (dist <= attack_range && dist < min_dist) {
                        min_dist = dist;
                        target = &monster;
                    }
                }
            }
            
            if (target != nullptr) {
                last_action = "Atacando a Monstruo " + std::to_string(target->id);
                target->hp -= attack_damage;
                if (target->hp <= 0) {
                    target->is_alive = false;
                    last_action = "¡Monstruo " + std::to_string(target->id) + " derrotado!";
                }
            } else if (!finished_path) {
                if (path_index >= path.size()) {
                    last_action = "¡Llegué a la meta!";
                    finished_path = true;
                    if (sim->allLivingHeroesFinishedPath()) simulation_running = false;
                } else {
                    current_coords = path[path_index];
                    path_index++;
                    last_action = "Moviéndome a (" + std::to_string(current_coords.x) + "," + std::to_string(current_coords.y) + ")";
                }
            } else last_action = "Esperando en la meta.";
            
            if (hp <= 0) {
                last_action = "¡He muerto!";
                is_alive = false;
            }
            if (sim->allHeroesDead() || sim->allMonstersDead()) simulation_running = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    simulation_running = false; 
}

void Monster::run(Simulation* sim) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    while (is_alive && simulation_running) {
        { 
            std::unique_lock<std::mutex> lock(sim_mutex);
            if (!is_alive || !simulation_running) continue; 
            Hero* target_hero = nullptr;
            int min_dist = 9999;
            for (Hero& hero : sim->heroes) {
                if (hero.is_alive) {
                    int dist = sim->manhattanDistance(current_coords, hero.current_coords);
                    if (dist < min_dist) {
                        min_dist = dist;
                        target_hero = &hero;
                    }
                }
            }
            if (target_hero == nullptr) last_action = "Esperando...";
            else {
                int dist_to_hero = min_dist; 
                if (!is_active) {
                    if (dist_to_hero <= vision_range) {
                        last_action = "¡He visto a un héroe!";
                        is_active = true;
                        for (Monster& other_monster : sim->monsters) {
                            if (other_monster.id != this->id && !other_monster.is_active) {
                                int dist_to_other = sim->manhattanDistance(current_coords, other_monster.current_coords);
                                if (dist_to_other <= vision_range) {
                                    other_monster.last_action = "¡Alertado por M" + std::to_string(id) + "!";
                                    other_monster.is_active = true;
                                }
                            }
                        }
                    }
                }
                if (is_active) {
                    if (dist_to_hero <= attack_range) {
                        last_action = "¡Atacando a Héroe " + std::to_string(target_hero->id) + "!";
                        target_hero->hp -= attack_damage;
                    } else {
                        if (target_hero->current_coords.x > current_coords.x) current_coords.x++;
                        else if (target_hero->current_coords.x < current_coords.x) current_coords.x--;
                        else if (target_hero->current_coords.y > current_coords.y) current_coords.y++;
                        else if (target_hero->current_coords.y < current_coords.y) current_coords.y--;
                        last_action = "Moviéndome a (" + std::to_string(current_coords.x) + "," + std::to_string(current_coords.y) + ")";
                    }
                }
            }
            if (hp <= 0) {
                last_action = "Derrotado";
                is_alive = false; 
            }
            if (sim->allHeroesDead() || sim->allMonstersDead() || sim->allLivingHeroesFinishedPath()) simulation_running = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void render_loop(Simulation* sim) {
    initscr();             
    int grid_w = sim->grid_size.x;
    int grid_h = sim->grid_size.y;
    int game_win_h = grid_h + 2;
    int game_win_w = grid_w + 2;
    int total_w_needed = game_win_w + UI_W;
    int term_h, term_w;
    getmaxyx(stdscr, term_h, term_w);
    if (term_h < game_win_h || term_w < total_w_needed) {
        endwin();
        std::cerr << "Error: El terminal es muy pequeño." << std::endl;
        simulation_running = false;
        return;
    }
    noecho();
    curs_set(0);
    start_color();
    init_pair(HERO_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(MONSTER_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(UI_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GAMEOVER_PAIR, COLOR_CYAN, COLOR_BLACK);
    WINDOW *gamewin = newwin(game_win_h, game_win_w, 0, 0);
    WINDOW *uiwin = newwin(game_win_h, UI_W, 0, game_win_w);
    timeout(33);
    while (simulation_running) {
        int ch = getch(); 
        if (ch == 'q') simulation_running = false; 
        werase(gamewin);
        werase(uiwin);
        box(gamewin, 0, 0);
        box(uiwin, 0, 0);
        { 
            std::lock_guard<std::mutex> lock(sim_mutex);
            for (const auto& monster : sim->monsters) {
                if (monster.is_alive) {
                    wattron(gamewin, COLOR_PAIR(MONSTER_PAIR));
                    mvwprintw(gamewin, monster.current_coords.y + 1, monster.current_coords.x + 1, "M"); 
                    wattroff(gamewin, COLOR_PAIR(MONSTER_PAIR));
                }
            }
            for (const auto& hero : sim->heroes) {
                if (hero.is_alive) {
                    wattron(gamewin, COLOR_PAIR(HERO_PAIR));
                    mvwprintw(gamewin, hero.current_coords.y + 1, hero.current_coords.x + 1, "H%d", hero.id); 
                    wattroff(gamewin, COLOR_PAIR(HERO_PAIR));
                }
            }
            int stat_line = 1; 
            wattron(uiwin, COLOR_PAIR(UI_PAIR));
            mvwprintw(uiwin, stat_line++, 1, "--- DOOM SIMULADOR (N-Héroes) ---");
            wattroff(uiwin, COLOR_PAIR(UI_PAIR));
            stat_line++; 
            for(const auto& h : sim->heroes) {
                if(h.is_alive)
                    mvwprintw(uiwin, stat_line, 1, "[H%d] HP: %d / %d", h.id, h.hp, h.hp_max);
                else
                    mvwprintw(uiwin, stat_line, 1, "[H%d] ¡MUERTO!", h.id);
                mvwprintw(uiwin, stat_line+1, 3, "Acción: "); 
                waddstr(uiwin, h.last_action.c_str());      
                stat_line += 3;
            }
            stat_line++; 
            mvwprintw(uiwin, stat_line++, 1, "--- Monstruos Vivos ---");
            for(const auto& m : sim->monsters) {
                if(m.is_alive && m.is_active) {
                    mvwprintw(uiwin, stat_line, 1, "[M%d] HP: %d | Acción: ", m.id, m.hp);
                    waddstr(uiwin, m.last_action.c_str()); 
                    stat_line++;
                }
                if (stat_line >= game_win_h - 2) {
                     mvwprintw(uiwin, stat_line, 1, "...y %d más...", (int)sim->monsters.size() - m.id);
                     break;
                }
            }
        }
        wnoutrefresh(gamewin);
        wnoutrefresh(uiwin);
        doupdate();
    }
    timeout(-1);
    bool win_by_kill = sim->allMonstersDead();
    bool win_by_path = sim->allLivingHeroesFinishedPath();
    wclear(uiwin);
    box(uiwin, 0, 0);
    wattron(uiwin, COLOR_PAIR(GAMEOVER_PAIR));
    int final_line = 5;
    if (win_by_kill || win_by_path) {
        mvwprintw(uiwin, final_line++, 5, "¡VICTORIA!");
        if (win_by_kill)
            mvwprintw(uiwin, final_line++, 3, "¡Los Héroes mataron a todos los monstruos!");
        else
            mvwprintw(uiwin, final_line++, 3, "¡Todos los Héroes vivos llegaron a la meta!");
    } else if (sim->allHeroesDead()) {
        mvwprintw(uiwin, final_line++, 5, "GAME OVER");
        mvwprintw(uiwin, final_line++, 3, "Todos los Héroes han sido derrotados.");
    } else {
        mvwprintw(uiwin, final_line++, 5, "SIMULACIÓN DETENIDA");
        mvwprintw(uiwin, final_line++, 3, "El usuario presionó 'q'.");
    }
    mvwprintw(uiwin, final_line+4, 3, "Presiona 'q' para salir.");
    wattroff(uiwin, COLOR_PAIR(GAMEOVER_PAIR));
    wrefresh(uiwin);
    while(getch() != 'q') {}
    delwin(gamewin);
    delwin(uiwin);
    endwin();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " config.txt" << std::endl;
        return 1;
    }
    srand(time(NULL)); 
    SimulationConfig config = parseConfig(argv[1]);
    Simulation sim(config); 
    std::vector<std::thread> threads;
    for (Hero& hero : sim.heroes) threads.push_back(std::thread(&Hero::run, &hero, &sim));
    for (Monster& monster : sim.monsters) threads.push_back(std::thread(&Monster::run, &monster, &sim));
    threads.push_back(std::thread(render_loop, &sim));
    for (std::thread& th : threads) th.join();
    std::cout << "--- Simulación Terminada ---" << std::endl;
    if (sim.allMonstersDead() || sim.allLivingHeroesFinishedPath())
        std::cout << "Resultado: ¡Los Héroes han ganado!" << std::endl;
    else
        std::cout << "Resultado: Los Héroes han sido derrotados." << std::endl;
    return 0;
}
```

- Makefile
```bash
CXX = g++
CXXFLAGS = -std=c++17 -pthread -g -Wall
TARGET = simulador_visual_N

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp -lncurses

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) config.txt
```
## Contacto

Creado por:
- Benjamín Guzmán
- Martin Huiriqueo
