#include <bits/stdc++.h>
#include <chrono>
#include <thread>

using namespace std;

// ≽(•⩊ •マ≼
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) { 
    os << "{";
    int n = (int)v.size();
    for (int i = 0; i < n; i++) {
        os << v[i];
        if (i < n - 1) os << ", ";
    }
    os << "}";
    return os;
}

void _print() { }
template <typename T, typename... U>
void _print(T a, U... b) {
    if (sizeof...(b)) {
        cerr << a << ", ";
        _print(b...);
    } else cerr << a;
}

#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = [", _print(x), cerr << "]" << endl
#else
#define debug(...)
#endif

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

    void run(int steps) {
        cout << "Estado inicial:" << endl;
        draw(0);
        for (int t = 1; t <= steps; t++) {
            for (auto &ant : ants) {
                moveAnt(ant);
                act(ant);
            }
        }
        cout << "Estado final:" << endl;
        draw(steps);
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
        double k = 0.3;
        return pow(k/(k + f), 2);
    }

    double dropProbability(int ant_items) {
        double f = ant_items/8.0;
        double k = 0.3;
        return pow(f/(k + f), 2);
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

    void draw(int step) {
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
    antC.run(10000);

    return 0;
}
/*   /\_/\
*   (= ._.)
*   / >  \>
*/



