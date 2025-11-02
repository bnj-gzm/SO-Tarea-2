#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

#define UI_W 50
#define HERO_PAIR 1
#define MONSTER_PAIR 2
#define UI_PAIR 3
#define GAMEOVER_PAIR 4

std::mutex sim_mutex;
std::condition_variable game_tick_cv;
bool simulation_running = true;

struct Coords { int x = 0, y = 0; };

struct HeroConfig {
    int id = 0, hp = 0, attack_damage = 0, attack_range = 0;
    Coords start;
    std::vector<Coords> path;
};

struct MonsterConfig {
    int id = 0, hp = 0, attack_damage = 0, vision_range = 0, attack_range = 0;
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
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string key; ss >> key;

        if (key == "GRID_SIZE") {
            ss >> config.grid_size.x >> config.grid_size.y;
        } else if (key == "MONSTER_COUNT") {
            ss >> monster_count;
            config.monsters.resize(monster_count);
        } else if (key.rfind("HERO_", 0) == 0) {
            size_t id_start = 5, id_end = key.find('_', id_start);
            if (id_end == std::string::npos) continue;
            int id = std::stoi(key.substr(id_start, id_end - id_start));
            if (id > (int)config.heroes.size()) config.heroes.resize(id);
            HeroConfig& h = config.heroes[id - 1]; h.id = id;
            std::string sub = key.substr(id_end + 1);
            
            if (sub == "PATH") { 
                int x, y; char c; 
                while (ss >> c >> x >> c >> y >> c) h.path.push_back({x, y});

                while (file.peek() == ' ' || file.peek() == '(') {
                    if (!std::getline(file, line)) break; 
                    std::stringstream ss_cont(line); 
                    while (ss_cont >> c >> x >> c >> y >> c) {
                        h.path.push_back({x, y}); 
                    }
                }
            
            } else if (sub == "START") ss >> h.start.x >> h.start.y;
            else { int v; ss >> v; if (sub == "HP") h.hp = v; else if (sub == "ATTACK_DAMAGE") h.attack_damage = v; else if (sub == "ATTACK_RANGE") h.attack_range = v; }
        
        } else if (key.rfind("MONSTER_", 0) == 0) {
            size_t id_start = 8, id_end = key.find('_', id_start);
            if (id_end == std::string::npos) continue;
            int id = std::stoi(key.substr(id_start, id_end - id_start));
            if (id > (int)config.monsters.size() || id == 0) continue;
            MonsterConfig& m = config.monsters[id - 1]; m.id = id;
            std::string sub = key.substr(id_end + 1);
            if (sub == "COORDS") ss >> m.coords.x >> m.coords.y;
            else { int v; ss >> v; if (sub == "HP") m.hp = v; else if (sub == "ATTACK_DAMAGE") m.attack_damage = v; else if (sub == "VISION_RANGE") m.vision_range = v; else if (sub == "ATTACK_RANGE") m.attack_range = v; }
        }
    }
    return config;
}

class Simulation;
class Monster;

class Hero {
public:
    int id, hp, hp_max, attack_damage, attack_range;
    Coords current_coords;
    std::vector<Coords> path;
    size_t path_index;
    bool is_alive, finished_path, in_combat;
    std::string last_action = "Iniciando...";
    Hero(const HeroConfig& c) {
        id = c.id; hp = c.hp; hp_max = c.hp;
        attack_damage = c.attack_damage; attack_range = c.attack_range;
        current_coords = c.start; path = c.path;
        path_index = 0; is_alive = true; finished_path = false; in_combat = false;
    }
    void run(Simulation* sim);
};

class Monster {
public:
    int id, hp, hp_max, attack_damage, vision_range, attack_range;
    Coords current_coords;
    bool is_active, is_alive;
    std::string last_action = "Durmiendo...";
    Monster(const MonsterConfig& c) {
        id = c.id; hp = c.hp; hp_max = c.hp;
        attack_damage = c.attack_damage; vision_range = c.vision_range; attack_range = c.attack_range;
        current_coords = c.coords; is_active = false; is_alive = true;
    }
    void run(Simulation* sim);
};

class Simulation {
public:
    Coords grid_size;
    std::vector<Hero> heroes;
    std::vector<Monster> monsters;

    Simulation(const SimulationConfig& c) {
        grid_size = c.grid_size;
        for (auto& h : c.heroes) heroes.emplace_back(h);
        for (auto& m : c.monsters) monsters.emplace_back(m);
    }

    int manhattanDistance(Coords a, Coords b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }
    bool allMonstersDead() { for (auto& m : monsters) if (m.is_alive) return false; return true; }
    bool allHeroesDead() { for (auto& h : heroes) if (h.is_alive) return false; return true; }
    bool allLivingHeroesFinishedPath() { bool alive = false; for (auto& h : heroes) { if (h.is_alive) { alive = true; if (!h.finished_path) return false; } } return alive; }
};

void Hero::run(Simulation* sim) {
    while (is_alive && simulation_running) {
        std::unique_lock<std::mutex> lock(sim_mutex);
        game_tick_cv.wait(lock);
        if (!is_alive || !simulation_running) break;

        Monster* target = nullptr; int min_dist = 9999;
        for (auto& m : sim->monsters) {
            if (m.is_alive) {
                int d = sim->manhattanDistance(current_coords, m.current_coords);
                if (d <= attack_range && d < min_dist) { min_dist = d; target = &m; }
            }
        }

        if (target) {
            in_combat = true;
            last_action = "Atacando M" + std::to_string(target->id);
            target->hp -= attack_damage;
            if (target->hp <= 0) { target->is_alive = false; last_action = "M" + std::to_string(target->id) + " muerto"; }
        } else {
            if (in_combat) {
                bool still_enemy = false;
                for (auto& m : sim->monsters) {
                    if (m.is_alive && sim->manhattanDistance(current_coords, m.current_coords) <= attack_range) { still_enemy = true; break; }
                }
                if (still_enemy) {
                    last_action = "En combate";
                } else {
                    in_combat = false;
                    last_action = "Salí del combate";
                }
            }
            if (!in_combat) {
                if (!finished_path) {
                    if (path_index >= path.size()) { finished_path = true; last_action = "Meta"; }
                    else { current_coords = path[path_index++]; last_action = "Mover a (" + std::to_string(current_coords.x) + "," + std::to_string(current_coords.y) + ")"; }
                } else last_action = "Esperando en la meta";
            }
        }

        if (hp <= 0) { is_alive = false; last_action = "Muerto"; }
        if (sim->allHeroesDead() || sim->allMonstersDead() || sim->allLivingHeroesFinishedPath()) simulation_running = false;
    }
}

void Monster::run(Simulation* sim) {
    while (is_alive && simulation_running) {
        std::unique_lock<std::mutex> lock(sim_mutex);
        game_tick_cv.wait(lock);
        if (!is_alive || !simulation_running) break;

        Hero* tgt = nullptr; int min = 9999;
        for (auto& h : sim->heroes) {
            if (h.is_alive) {
                int d = sim->manhattanDistance(current_coords, h.current_coords);
                if (d < min) { min = d; tgt = &h; }
            }
        }

        if (!tgt) last_action = "Esperando";
        else {
            if (!is_active) {
                if (min <= vision_range) {
                    last_action = "Veo a héroe";
                    is_active = true;
                    for (auto& o : sim->monsters) {
                        if (o.id != id && !o.is_active && sim->manhattanDistance(current_coords, o.current_coords) <= vision_range) {
                            o.is_active = true; o.last_action = "Alerta M" + std::to_string(id);
                        }
                    }
                }
            }
            if (is_active) {
                if (min <= attack_range) {
                    last_action = "Atacando H" + std::to_string(tgt->id);
                    tgt->hp -= attack_damage;
                } else {
                    if (tgt->current_coords.x > current_coords.x) current_coords.x++;
                    else if (tgt->current_coords.x < current_coords.x) current_coords.x--;
                    else if (tgt->current_coords.y > current_coords.y) current_coords.y++;
                    else if (tgt->current_coords.y < current_coords.y) current_coords.y--;
                    last_action = "Mover a (" + std::to_string(current_coords.x) + "," + std::to_string(current_coords.y) + ")"; 
                }
            }
        }

        if (hp <= 0) { is_alive = false; last_action = "Muerto"; }
        if (sim->allHeroesDead() || sim->allMonstersDead() || sim->allLivingHeroesFinishedPath()) simulation_running = false;
    }
}

void render_loop(Simulation* sim) {
    initscr(); noecho(); curs_set(0); start_color();
    init_pair(HERO_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(MONSTER_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(UI_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GAMEOVER_PAIR, COLOR_CYAN, COLOR_BLACK);

    int gw = sim->grid_size.x, gh = sim->grid_size.y;
    int win_h = gh + 2;
    WINDOW* gamewin = newwin(win_h, gw + 2, 0, 0);
    WINDOW* uiwin = newwin(win_h, UI_W, 0, gw + 2);
    timeout(33);

    while (simulation_running) {
        int c = getch(); if (c == 'q') simulation_running = false;
        werase(gamewin); werase(uiwin);
        box(gamewin, 0, 0); box(uiwin, 0, 0);
        {
            std::lock_guard<std::mutex> lk(sim_mutex);
            
            for (auto& m : sim->monsters) if (m.is_alive) { wattron(gamewin, COLOR_PAIR(MONSTER_PAIR)); mvwprintw(gamewin, m.current_coords.y + 1, m.current_coords.x + 1, "M"); wattroff(gamewin, COLOR_PAIR(MONSTER_PAIR)); }
            
            for (auto& h : sim->heroes) if (h.is_alive) { wattron(gamewin, COLOR_PAIR(HERO_PAIR)); mvwprintw(gamewin, h.current_coords.y + 1, h.current_coords.x + 1, "H%d", h.id); wattroff(gamewin, COLOR_PAIR(HERO_PAIR)); }

            int l = 1;
            wattron(uiwin, COLOR_PAIR(UI_PAIR)); mvwprintw(uiwin, l++, 1, "--- DOOM---"); wattroff(uiwin, COLOR_PAIR(UI_PAIR)); l++;

            for (auto& h : sim->heroes) {
                if (l >= win_h - 3) break;
                if (h.is_alive) mvwprintw(uiwin, l++, 1, "[H%d] HP:%d/%d", h.id, h.hp, h.hp_max);
                else mvwprintw(uiwin, l++, 1, "[H%d] ¡MUERTO!", h.id);
                mvwprintw(uiwin, l++, 3, "%s", h.last_action.c_str());
                l++;
            }

            mvwprintw(uiwin, l++, 1, "--- Monstruos Activos ---");
            l++;
            for (auto& m : sim->monsters) {
                if (l >= win_h - 3) {
                    mvwprintw(uiwin, l++, 1, "...y %d más...", (int)sim->monsters.size() - m.id);
                    break;
                }
                if (m.is_alive && m.is_active) { 
                    mvwprintw(uiwin, l++, 1, "[M%d] HP:%d/%d", m.id, m.hp, m.hp_max);
                    mvwprintw(uiwin, l++, 3, "%s", m.last_action.c_str());
                    l++;
                }
            }
        }
        wnoutrefresh(gamewin); wnoutrefresh(uiwin); doupdate();
    }

    timeout(-1);
    bool winK = sim->allMonstersDead(), winP = sim->allLivingHeroesFinishedPath();
    wclear(uiwin); box(uiwin, 0, 0); wattron(uiwin, COLOR_PAIR(GAMEOVER_PAIR));
    int l = 5;
    if (winK || winP) {
        mvwprintw(uiwin, l++, 5, "VICTORIA");
        if (winK) mvwprintw(uiwin, l++, 3, "derrotaron a los monstruos.");
        else mvwprintw(uiwin, l++, 3, "llegaron a la meta.");
    } else {
        mvwprintw(uiwin, l++, 5, "GAME OVER");
        mvwprintw(uiwin, l++, 3, "Los heroes fueron derrotados.");
    }
    mvwprintw(uiwin, l + 2, 3, "Presiona 'q' para salir.");
    wattroff(uiwin, COLOR_PAIR(GAMEOVER_PAIR));
    wrefresh(uiwin);
    while (getch() != 'q') {}
    endwin();
}

void ticker_loop(Simulation*) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (simulation_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (!simulation_running) break;
        game_tick_cv.notify_all();
    }
    game_tick_cv.notify_all();
}

int main(int argc, char* argv[]) {
    if (argc < 2) { std::cerr << "Uso: " << argv[0] << " config.txt\n"; return 1; }
    srand(time(NULL));
    SimulationConfig cfg = parseConfig(argv[1]);
    Simulation sim(cfg);
    std::vector<std::thread> t;
    
    for (auto& h : sim.heroes) t.emplace_back(&Hero::run, &h, &sim);
    for (auto& m : sim.monsters) t.emplace_back(&Monster::run, &m, &sim);
    
    t.emplace_back(render_loop, &sim);
    t.emplace_back(ticker_loop, &sim);
    
    for (auto& th : t) th.join();

    bool winK = sim.allMonstersDead();
    bool winP = sim.allLivingHeroesFinishedPath();
    
    std::cout << "--- Simulación Terminada ---\n";
    if (winK || winP) {
        if (winK) std::cout << "Victoria: derrotaron a los monstruos.\n";
        else std::cout << "Victoria: llegaron a la meta.\n";
    } else {
        std::cout << "Derrota: los héroes fueron derrotados.\n";
    }
}

