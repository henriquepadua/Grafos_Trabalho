#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <climits>

using namespace std;

struct Pixel {
    int x, y;
    int dist;
};

vector<vector<int>> IFT(vector<vector<int> > &matrix, vector<pair<int, int>> &seeds){
    int height = matrix.size();
    int width = matrix[0].size();

    vector<vector<int> > dist_matrix(height, vector<int>(width, INT_MAX));
    vector<vector<int> > label_matrix(height, vector<int>(width, 0));

    queue<Pixel> q;

    for (auto& seed : seeds) {
        int x = seed.first;
        int y = seed.second;

        dist_matrix[x][y] = 0;
        label_matrix[x][y] = x * width + y;
        q.push({x, y, 0});
    }

    while (!q.empty()) {
        Pixel pixel = q.front();
        q.pop();

        int x = pixel.x;
        int y = pixel.y;

        vector<pair<int, int>> neighbors = {{x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}};

        for (auto& neighbor : neighbors){
            int nx = neighbor.first;
            int ny = neighbor.second;

            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                int path_cost = 1;
                if (dist_matrix[nx][ny] > dist_matrix[x][y] + path_cost) {
                    dist_matrix[nx][ny] = dist_matrix[x][y] + path_cost;
                    label_matrix[nx][ny] = label_matrix[x][y];
                    q.push({nx, ny, dist_matrix[nx][ny]});
                }
            }
        }
    }

    return label_matrix;
}

void printAllTree(const vector<vector<int>>& label_matrix){
    for (const auto& row : label_matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void printIsolatedTree(const vector<vector<int>>& label_matrix) {
    int rows = label_matrix.size();
    int cols = label_matrix[0].size();

    unordered_set<int> etiquetas_impressas;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int etiqueta = label_matrix[i][j];

            if (etiquetas_impressas.find(etiqueta) == etiquetas_impressas.end()) {
                cout << "Árvore " << etiqueta << ":" << endl;

                for (int x = 0; x < rows; ++x) {
                    for (int y = 0; y < cols; ++y) {
                        if (label_matrix[x][y] == etiqueta) {
                            cout << label_matrix[x][y] << " ";
                        } else {
                            cout << "  ";
                        }
                    }
                    cout << endl;
                }
                cout << endl;
                etiquetas_impressas.insert(etiqueta);
            }
        }
    }
}

int main() {
    vector<vector<int>> image = {
        {1, 1, 1, 2, 2},
        {1, 1, 3, 2, 2},
        {1, 3, 3, 3, 2},
        {4, 4, 4, 4, 2},
        {4, 4, 4, 4, 4}
    };
    vector<pair<int, int>> seeds = {{0, 0}, {2, 2}};

    auto result = IFT(image, seeds);

    printAllTree(result);

    printIsolatedTree(result);

    return 0;
}