#include <list>
#include <vector>
#include <cmath> // Para a função abs
#include <algorithm>
#include <iostream>

using namespace std;

typedef struct Aresta{
  int origem;
  int destino;
  float peso;
} Aresta;

typedef struct Componente{
  std::list<int> vertices;
  float variancia;
} Componente;

int encontrarPai(int v, vector<int>& pai) {
    cout << "Encontrar pai de " << v << endl;
    if (pai[v] != v) {
        cout << "Passei aqui:" << pai[v] << endl;
        pai[v] = encontrarPai(pai[v], pai);
    }
    cout << "Valor do pai:" << pai[v] << endl;
    return pai[v];
}

double calcularNovaVariancia(const Componente& componente) {
    return 0.0; // Implementação fictícia
}

void unirComponente(int origem, int destino, vector<int>& pai, vector<int>& rank) {
    int pai_origem = encontrarPai(origem, pai);
    int pai_destino = encontrarPai(destino, pai);

    if (rank[pai_origem] < rank[pai_destino]) {
        pai[pai_origem] = pai_destino;
    } else if (rank[pai_origem] > rank[pai_destino]) {
        pai[pai_destino] = pai_origem;
    } else {
        pai[pai_destino] = pai_origem;
        rank[pai_origem]++;
    }
}

float diferencaIntensidade(int pixel1, int pixel2) {
    return abs(pixel1 - pixel2);
}

vector<Aresta> construirGrafo(const vector<vector<int>>& imagem) {
    int linhas = imagem.size();
    cout << "Tamanho da liha: "<< linhas << endl;
    int colunas = imagem[0].size();
    cout << "Tamanho da liha: " << colunas << endl;
    vector<Aresta> arestas;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (j < colunas - 1) {
                arestas.push_back({i * colunas + j, i * colunas + (j + 1), diferencaIntensidade(imagem[i][j], imagem[i][j + 1])});
            }
            if (i < linhas - 1) {
                arestas.push_back({i * colunas + j, (i + 1) * colunas + j, diferencaIntensidade(imagem[i][j], imagem[i + 1][j])});
            }
        }
    }
    return arestas;
}

void construirMST(vector<Aresta>& arestas, int numeroDeVertices, float limite) {
    vector<int> pai(numeroDeVertices);
    vector<int> rank(numeroDeVertices, 0);
    vector<Componente> componente(numeroDeVertices); 

    for (int i = 0; i < numeroDeVertices; i++) {
        pai[i] = i;
        componente[i] = Componente();
        componente[i].vertices = list<int>();
        componente[i].vertices.push_back(i);
        componente[i].variancia = 0;
    }

    sort(arestas.begin(), arestas.end(), [](const Aresta& a, const Aresta& b) {
        return a.peso < b.peso;
    });

    for (const auto& aresta : arestas) {
        int origem = aresta.origem;
        int destino = aresta.destino;
        float peso = aresta.peso;

        int raizOrigem = encontrarPai(origem, pai);
        int raizDestino = encontrarPai(destino, pai);

        if (raizOrigem != raizDestino) {
            float varianciaInternaOrigem = componente[raizOrigem].variancia;
            float varianciaInternaDestino = componente[raizDestino].variancia;

            if (peso <= limite * (varianciaInternaOrigem + varianciaInternaDestino)) {
                unirComponente(origem, destino, pai, rank);

                int novaRaiz = encontrarPai(origem, pai);
                componente[novaRaiz].vertices.insert(
                    componente[novaRaiz].vertices.end(),
                    componente[raizOrigem].vertices.begin(),
                    componente[raizOrigem].vertices.end()
                );
                componente[novaRaiz].vertices.insert(
                    componente[novaRaiz].vertices.end(),
                    componente[raizDestino].vertices.begin(),
                    componente[raizDestino].vertices.end()
                );
                componente[novaRaiz].variancia = calcularNovaVariancia(componente[novaRaiz]);
            }
        }
    }

    for (int i = 0; i < numeroDeVertices; i++) {
        if (pai[i] == i) {
            cout << "Componente: ";
            for (int vertice : componente[i].vertices) {
                cout << vertice << " ";
            }
            cout << endl;
        }
    }
}

int main() {
    vector<vector<int>> imagem = {
        {10, 20, 30},
        {20, 30, 40},
        {30, 40, 50}
    };

    int linhas = imagem.size();
    int colunas = imagem[0].size();
    int numeroDeVertices = linhas * colunas;
    vector<Aresta> arestas = construirGrafo(imagem);
    float limite = 1.5;

    construirMST(arestas, numeroDeVertices, limite);

    return 0;
}


// // Construção do Grafo:

// // Cada pixel é um vértice.
// // As arestas são criadas entre pixels adjacentes, e o peso de uma aresta é a
// // diferença de intensidade (ou outra característica) entre os pixels. Ordenação
// // das Arestas:

// // As arestas são ordenadas em ordem crescente de peso.
// // Construção da Árvore Geradora Mínima: 

// // Inicialmente, cada pixel é um componente (ou segmento) separado.
// // As arestas são processadas em ordem crescente de peso.
// // Dois componentes são unidos se a diferença entre eles for pequena em
// // comparação com a variância interna dos componentes. Critério de Parada:

// // Um limite é utilizado para decidir se dois componentes devem ser unidos,
// // baseado em um parâmetro de escala.
// #include <list>
// #include <vector>
// #include <cmath> // Para a função abs
// #include <algorithm>
// #include <iostream>

// using namespace std;

// typedef struct Aresta{
//   int origem;
//   int destino;
//   float peso;
// } Aresta;

// typedef struct Componente{
//   std::list<int> vertices;
//   float variancia;
// } Componente;

// int encontrarPai(int v,vector<int> pai){
//   if(pai[v] != v){
//     pai[v] = encontrarPai(pai[v],pai);
//   }
// }

// double calcularNovaVariancia(const Componente& componente) {
//     return 0.0; // Implementação fictícia
// }

// void unirComponente(int origem,int destino,vector<int> pai,vector<Componente> rank){
//   int pai_origem = encontrarPai(origem,pai);
//   int pai_destino = encontrarPai(destino,pai);

//   if(rank[pai_origem].variancia < rank[pai_destino].variancia){
//     pai[pai_origem] = pai_destino;
//   }else if(rank[pai_origem].variancia > rank[pai_destino].variancia){
//     pai[pai_destino] = pai_origem;
//   }else{
//     pai[pai_origem] = pai_destino;
//   }
// }

// float diferencaIntensidade(int pixel1, int pixel2){
//   return abs(pixel1 - pixel2);
// }

// vector<Aresta> construirGrafo(vector<vector<int>> imagens){
//   int linhas = imagens.size();
//   int colunas = imagens[0].size();
//   vector<Aresta> arestas = {};
//   for(int i = 0; i < linhas; i++){
//     for(int j = 0; j < colunas; j++){
//        if(j < colunas - 1){
//          arestas.push_back({i * colunas + j, i * colunas + (j + 1), diferencaIntensidade(imagens[i][j], imagens[i][j + 1])});
//        } 
//       if(i < linhas - 1){
//         arestas.push_back({i * colunas + j, (i + 1) * colunas + j,diferencaIntensidade(imagens[i][j],imagens[i+1][j])});
//       }
//     }
//   }
//   return arestas;
// }

// // // Função principal para construir a MST
//     void construirMST(vector<Aresta> arestas, int numeroDeVertices, float limite){
//       vector<int> pai(numeroDeVertices);
//       vector<int> rank(numeroDeVertices);
//       vector<Componente> componente(numeroDeVertices);

//       for(int i = 0; i < numeroDeVertices; i++){
//         pai[i] = i;
//         rank[i] = 0;
//         componente[i] = Componente();
//         componente[i].vertices = list<int>();
//         componente[i].vertices.insert(componente[i].vertices.end(),i);
//         componente[i].variancia = 0;
//       }

//       sort(arestas.begin(), arestas.end(), [](const Aresta& a, const Aresta& b) {
//           return a.peso < b.peso;
//       });
      
//       for(int i = 0; i < arestas.size() ; i++){
//         Aresta aresta = arestas[i];
//         int origem = aresta.origem;
//         int destino = aresta.destino;
//         float peso = aresta.peso;

//         int raizOrigem = encontrarPai(origem,pai);
//         int raizDestino = encontrarPai(destino,pai);

//         if(raizOrigem != raizDestino){ 
//           float varianciaInternaOrigem = componente[raizOrigem].variancia;
//           float varianciaInternaDestino = componente[raizDestino].variancia;

//           if(peso <= limite * (varianciaInternaOrigem + varianciaInternaDestino)){
//             unirComponente(origem,destino,pai,rank);

//             int novaRaiz = encontrarPai(origem,pai);
//             componente[novaRaiz].vertices.insert(
//               componente[novaRaiz].vertices.end(),
//               componente[raizOrigem].vertices.begin(),
//               componente[raizOrigem].vertices.end()
//             );
//             componente[novaRaiz].vertices.insert(
//               componente[novaRaiz].vertices.end(),
//               componente[raizDestino].vertices.begin(),
//               componente[raizDestino].vertices.end()
//             );
//             componente[novaRaiz].variancia = calcularNovaVariancia(componente[novaRaiz]);
//           }
//         }
//       }
      
//       for(int i = 0; i < numeroDeVertices; i++){
//         if(pai[i] == i){
//           //cout << componente[i].vertices << endl;
//         }
//       }
//     }
// #include <iostream>
// using namespace std;

// int main(){
//   vector<std::vector<int>> imagem = {
//       {10, 20, 30},
//       {20, 30, 40},
//       {30, 40, 50}
//   };
  
//   int linhas = imagem.size();
//   int colunas = imagem[0].size();
//   int numeroDeVertices = linhas * colunas;
//   std::vector<Aresta> arestas = construirGrafo(imagem);
//   double limite = 1.5;
  
//   construirMST(arestas, numeroDeVertices, limite);
  
//   return 0;
// }

// // #include <queue>
// // #include <vector>

// // using namespace std;

// // int main(){
// //   queue<int> fila;
// //   for(int i=0;i<10;i++){
// //     fila.push(i);
// //   }
// //   cout << fila.front() << endl;
// //   fila.pop();
// //   cout << fila.front() << endl;

// //   for(int i=0;i<9;i++){
// //     fila.pop();
// //     if(fila.empty()){
// //       cout << "Vazio na iteração " << i+1 << endl;
// //     }
// //   }
// // }