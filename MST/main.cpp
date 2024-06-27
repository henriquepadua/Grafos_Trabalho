#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Estrutura para representar um pixel
struct Pixel {
    int x, y;
    Vec3b color;
};

// Estrutura para representar uma aresta
struct Edge {
    int u, v;
    double weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Função para calcular a diferença de intensidade entre dois pixels
double intensityDifference(const Pixel& p1, const Pixel& p2) {
    Vec3b c1 = p1.color;
    Vec3b c2 = p2.color;
    return sqrt((c1[0] - c2[0]) * (c1[0] - c2[0]) +
                (c1[1] - c2[1]) * (c1[1] - c2[1]) +
                (c1[2] - c2[2]) * (c1[2] - c2[2]));
}

// Função para construir o grafo de imagem
vector<Edge> buildGraph(const Mat& image, vector<Pixel>& pixels) {
    int rows = image.rows;
    int cols = image.cols;
    vector<Edge> edges;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Pixel p1 = {x, y, image.at<Vec3b>(y, x)};
            pixels.push_back(p1);

            if (x > 0) {
                Pixel p2 = {x - 1, y, image.at<Vec3b>(y, x - 1)};
                edges.push_back({y * cols + x, y * cols + x - 1, intensityDifference(p1, p2)});
            }
            if (y > 0) {
                Pixel p2 = {x, y - 1, image.at<Vec3b>(y - 1, x)};
                edges.push_back({y * cols + x, (y - 1) * cols + x, intensityDifference(p1, p2)});
            }
        }
    }

    return edges;
}

// Classe para representar um conjunto disjunto com união por rank e compressão de caminho
class DisjointSet {
private:
    vector<int> parent, rank, size;

public:
    DisjointSet(int n) : parent(n), rank(n, 0), size(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]); // Compressão de caminho
        }
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
                size[rootU] += size[rootV];
            } else {
                parent[rootU] = rootV;
                size[rootV] += size[rootU];
                if (rank[rootU] == rank[rootV]) {
                    rank[rootV]++;
                }
            }
        }
    }

    int getSize(int u) {
        int root = find(u);
        return size[root];
    }
};

// Função para encontrar a AGM usando o algoritmo de Kruskal
vector<Edge> findMST(vector<Edge>& edges, int numVertices) {
    sort(edges.begin(), edges.end());
    DisjointSet ds(numVertices);
    vector<Edge> mst;

    for (Edge& edge : edges) {
        if (ds.find(edge.u) != ds.find(edge.v)) {
            mst.push_back(edge);
            ds.unionSets(edge.u, edge.v);
        }
    }

    return mst;
}

// Função principal para segmentação de imagens
Mat imageSegmentation(const Mat& image, double k) {
    int rows = image.rows;
    int cols = image.cols;
    int numPixels = rows * cols;

    vector<Pixel> pixels;
    vector<Edge> edges = buildGraph(image, pixels);
    vector<Edge> mst = findMST(edges, numPixels);

    DisjointSet ds(numPixels);
    vector<double> threshold(numPixels, k);

    for (Edge& edge : mst) {
        int u = edge.u;
        int v = edge.v;
        int setU = ds.find(u);
        int setV = ds.find(v);

        if (setU != setV) {
            if (edge.weight <= min(threshold[setU], threshold[setV])) {
                ds.unionSets(setU, setV);
                int newSet = ds.find(setU);
                threshold[newSet] = edge.weight + k / ds.getSize(newSet);
            }
        }
    }

    Mat segmented(image.size(), CV_8UC3);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int comp = ds.find(y * cols + x);
            segmented.at<Vec3b>(y, x) = pixels[comp].color;
        }
    }

    return segmented;
}

// Exemplo de uso
int main() {
    Mat image = imread("imagem.jpg");

    if (image.empty()) {
        cerr << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    double k = 500.0; // Parâmetro para controle da segmentação
    Mat segmented = imageSegmentation(image, k);

    imshow("Original", image);
    imshow("Segmentada", segmented);
    waitKey(0);

    return 0;
}
