#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include "debug.hpp"

using namespace std;

struct Item {
    bool exists = false;
};

struct Ant {
    int x, y;
    bool carrying = false;
};

class AntClustering {
private:
    int rows, cols;
    int numItems;
    int numAnts;
    int radious = 1;
    int k1 = 0.3, k2 = 0.3;

    vector<vector<Item>> grid;
    vector<vector<bool>> antGrid;
    vector<Ant> ants;

    mt19937 rng;

public:
    AntClustering(int r, int c, int items, int antsCount) : rows(r), cols(c), numItems(items), numAnts(antsCount) {
        rng.seed(random_device{}());

        grid.assign(rows, vector<Item>(cols));
        antGrid.assign(rows, vector<bool>(cols, false));

        placeItems();
        placeAnts();
    }

    void run(int steps, bool visualize = false, int delayMs = 80) {
        if (!visualize) {
            cout << "Estado inicial:\n";
            draw(0);
        }

        for (int t = 1; t <= steps; t++) {
            for (auto &ant : ants) {
                moveAnt(ant);
                act(ant);
            }

            if (visualize) {
                draw(t);
                this_thread::sleep_for(chrono::milliseconds(delayMs));
            }
        }

        if (!visualize) {
            cout << "Estado final:\n";
            draw(steps);
        }
    }

private:
    int wrap(int v, int lim) {
        if (v < 0) return lim-1;
        if (v >= lim) return 0;
        return v;
    }

    int randInt(int a, int b) {
        uniform_int_distribution<int> dist(a,b);
        return dist(rng);
    }

    double randDouble() {
        uniform_real_distribution<double> dist(0.0,1.0);
        return dist(rng);
    }

    void placeItems() {
        int placed = 0;
        while(placed < numItems) {
            int x = randInt(0, rows-1);
            int y = randInt(0, cols-1);
            if(!grid[x][y].exists) {
                grid[x][y].exists = true;
                placed++;
            }
        }
    }

    void placeAnts() {
        int placed = 0;
        while(placed < numAnts) {
            int x = randInt(0, rows-1);
            int y = randInt(0, cols-1);

            if(!antGrid[x][y]) { 
                ants.push_back({x, y, false});
                antGrid[x][y] = true;
                placed++;
            }
        }
    }

    void moveAnt(Ant &ant) {
        static const vector<pair<int,int>> mvs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        auto [dx, dy] = mvs[randInt(0, 3)];

        int nx = wrap(ant.x + dx, rows);
        int ny = wrap(ant.y + dy, cols);

        if(!antGrid[nx][ny]) {
            antGrid[ant.x][ant.y] = false;
            ant.x = nx;
            ant.y = ny;
            antGrid[nx][ny] = true;
        }
    }

    int countItemsAround(int x, int y) {
        int cnt = 0;

        for(int dx = -radious; dx <= radious; dx++) {
            for(int dy = -radious; dy <= radious; dy++) {
                if(dx == 0 and dy == 0) continue;

                int nx = wrap(x + dx, rows);
                int ny = wrap(y + dy, cols);

                if(grid[nx][ny].exists) cnt++;
            }
        }

        return cnt;
    }

    double pickProbability(int ant_items) {
        double f = ant_items/8.0;
        return pow(k1/(k1 + f), 2);
    }

    double dropProbability(int ant_items) {
        double f = ant_items/8.0;
        return pow(f/(k2 + f), 2);
    }

    void act(Ant &ant) {
        int ant_items = countItemsAround(ant.x, ant.y);

        if(!ant.carrying) {
            if(grid[ant.x][ant.y].exists) {
                if(randDouble() < pickProbability(ant_items)) {
                    grid[ant.x][ant.y].exists = false;
                    ant.carrying = true;
                }
            }
        }else {
            if(!grid[ant.x][ant.y].exists) {
                if(randDouble() < dropProbability(ant_items)) {
                    grid[ant.x][ant.y].exists = true;
                    ant.carrying = false;
                }
            }
        }
    }

void draw(int step, bool clear = false) {
    if (clear) {
        cout << "\033[2J\033[1;1H";
    }
        vector<string> view(rows, string(cols, '.'));

        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if(grid[i][j].exists) view[i][j] = 'o';
            }
        }

        for(auto &ant : ants) view[ant.x][ant.y] = ant.carrying ? 'A' : 'a';

        string output;
        output += "Iteracao: " + to_string(step) + "\n\n";

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                output += view[i][j];
                output += " ";
            }
            output += "\n";
        }
        cout << output;
        cout.flush();
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

    AntClustering antC(20, 40, 120, 15);
    // Para rodar sem visualização, use: antC.run(numero_de_passos);
    // Para rodar com visualização, use: antC.run(numero_de_passos, true, delay_em_milisegundos);

    antC.run(10000);

    return 0;
}


