#include <iostream>
#include <vector>
#include <list>

enum Color { WHITE, GRAY, BLACK };

struct Vertex {
    int id;       
    Color color;  
    int parent;   
    int tempo

    Vertex(int id = 0, Color color = WHITE, int parent = -1)
        : id(id), color(color), parent(parent) {}
};


void initializeGraph(std::vector<std::list<int>>& adj, std::vector<Vertex>& vertices, int V) {
    adj.resize(V);          
    vertices.resize(V);   
    for (int i = 0; i < V; ++i) {
        vertices[i] = Vertex(i); 
    }
}

void addEdge(std::vector<std::list<int>>& adj, int u, int v) {
    adj[u].push_back(v);
}


void printGraph(const std::vector<std::list<int>>& adj, const std::vector<Vertex>& vertices) {
    for (int i = 0; i < adj.size(); ++i) {
        std::cout << "Vértice " << vertices[i].id << " (Cor: " << vertices[i].color << ", Pai: " << vertices[i].parent << "): ";
        for (int v : adj[i]) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int V = 5;
    std::vector<std::list<int>> adj;
    std::vector<Vertex> vertices;
    
    initializeGraph(adj, vertices, V);

    addEdge(adj, 0, 1);
    addEdge(adj, 0, 4);
    addEdge(adj, 1, 2);
    addEdge(adj, 1, 3);
    addEdge(adj, 2, 4);
    addEdge(adj, 3, 4);

    printGraph(adj, vertices);

    return 0;
}