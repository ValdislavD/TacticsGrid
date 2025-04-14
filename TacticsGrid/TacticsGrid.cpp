#include <iostream>
#include <windows.h>
#include <cmath> // для abs()

const int GRID_SIZE = 10;
char grid[GRID_SIZE][GRID_SIZE];

class Unit {
public:
    int x, y;
    char symbol;
    int hp;
    bool alive;

    Unit(int x_, int y_, char symbol_) : x(x_), y(y_), symbol(symbol_), hp(100), alive(true) {}

    void move(int dx, int dy) {
        int newX = x + dx;
        int newY = y + dy;

        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
            x = newX;
            y = newY;
        }
    }

    bool isAdjacentTo(const Unit& other) {
        return std::abs(x - other.x) + std::abs(y - other.y) == 1;
    }

    void attack(Unit& target) {
        if (isAdjacentTo(target) && target.alive) {
            std::cout << (symbol == 'P' ? "Вы атакуете врага!\n" : "Враг атакует вас!\n");
            target.hp -= 30;
            if (target.hp <= 0) {
                target.alive = false;
                std::cout << "Враг повержен!\n";
            }
            else {
                std::cout << "HP врага: " << target.hp << "\n";
            }
        }
        else {
            std::cout << "Враг слишком далеко для атаки!\n";
        }
    }
};

void initGrid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = '.';
}

void placeUnits(const Unit& player, const Unit& enemy) {
    if (player.alive) grid[player.x][player.y] = player.symbol;
    if (enemy.alive) grid[enemy.x][enemy.y] = enemy.symbol;
}

void printGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void updateGrid(const Unit& player, const Unit& enemy) {
    initGrid();
    placeUnits(player, enemy);
    printGrid();
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Unit player(0, 0, 'P');
    Unit enemy(9, 9, 'E');

    bool running = true;

    while (running) {
        updateGrid(player, enemy);
        std::cout << "HP игрока: " << (player.alive ? player.hp : 0) << "\n";
        std::cout << "HP врага: " << (enemy.alive ? enemy.hp : 0) << "\n";

        if (!player.alive) {
            std::cout << "\nВы проиграли! Игра окончена.\n";
            running = false;
        }

        std::cout << "Ход игрока (WASD — движение, F — атака, Q — выход): ";
        char input;
        std::cin >> input;

        switch (input) {
        case 'w': case 'ц': player.move(-1, 0); break;
        case 's': case 'ы': player.move(1, 0); break;
        case 'a': case 'ф': player.move(0, -1); break;
        case 'd': case 'в': player.move(0, 1); break;
        case 'f': case 'а': player.attack(enemy); break;
        case 'q': case 'й': running = false; break;
        default: std::cout << "Неверная команда!\n"; break;
        }

        // Ход врага (простой ИИ с атакой)
        if (enemy.alive) {
            if (enemy.isAdjacentTo(player)) {
                enemy.attack(player);
            }
            else {
                int dx = 0, dy = 0;
                if (std::abs(enemy.x - player.x) > std::abs(enemy.y - player.y)) {
                    dx = (player.x > enemy.x) ? 1 : -1;
                }
                else if (enemy.y != player.y) {
                    dy = (player.y > enemy.y) ? 1 : -1;
                }

                // Проверка столкновения с игроком перед движением
                if (!(enemy.x + dx == player.x && enemy.y + dy == player.y)) {
                    enemy.move(dx, dy);
                }
            }
        }

    }

    return 0;
}
