#include <iostream>
#include <vector>
#include <list>

// QUEUE FUNCTIONS
struct Node {
    int data;
    Node* next;
};

Node* createNode(int value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->next = nullptr;
    return newNode;
}

void enqueue(Node** front, Node** rear, int value) {
    Node* newNode = createNode(value);

    if (*rear) {
        (*rear)->next = newNode;
        *rear = newNode;
    } else {
        *front = *rear = newNode;
    }
}

void dequeue(Node** front, Node** rear) {
    if (*front) {
        Node* temp = *front;
        *front = (*front)->next;

        if (*front == nullptr) {
            *rear = nullptr;
        }

        delete temp;
    }
}

int getFront(Node* front) {
    if (front) {
        return front->data;
    } else {
        throw std::runtime_error("Queue is empty");
    }
}

bool isEmpty(Node* front) {
    return front == nullptr;
}

// GRAPH FUNCTIONS
enum Color { WHITE, GRAY, BLACK };

struct Vertex {
    int id;       
    Color color;  
    int parent;   
    int time;
    int d;

    Vertex(int id = 0, Color color = WHITE, int parent = -1)
        : id(id), color(color), parent(parent) {}
};

struct Graph {
    std::vector<std::list<int>> adj;
    std::vector<Vertex> vertices;
};

void initializeGraph(Graph& graph, int size) {
    graph.adj.resize(size);          
    graph.vertices.resize(size);   
    for (int i = 0; i < size; ++i) {
        graph.vertices[i] = Vertex(i); 
    }
}

void addEdge(Graph& graph, int u, int v) {
    graph.adj[u].push_back(v);
    graph.adj[v].push_back(u);
}

void printGraph(const Graph& graph) {
    for (int i = 0; i < graph.adj.size(); ++i) {
        std::cout << "Vértice " << graph.vertices[i].id << " (Cor: " << graph.vertices[i].color << ", Pai: " << graph.vertices[i].parent << "): ";
        for (int v : graph.adj[i]) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

void bfs(Graph& graph, int startVertexIndex) {
    std::vector<bool> visited(graph.adj.size(), false);
    Node* front = nullptr;
    Node* rear = nullptr;

    std::cout << "Iniciando busca em largura a partir do vértice " << startVertexIndex << std::endl;

    enqueue(&front, &rear, startVertexIndex);
    visited[startVertexIndex] = true;

    graph.vertices[startVertexIndex].color = GRAY;
    graph.vertices[startVertexIndex].d = 0;

    std::cout << "Vértice " << startVertexIndex << " marcado como cinza (GRAY) e adicionado à fila" << std::endl;

    while (!isEmpty(front)) {
        int u = getFront(front);
        dequeue(&front, &rear);

        std::cout << "Desenfileirando vértice " << u << std::endl;

        for (int v : graph.adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                enqueue(&front, &rear, v);
                graph.vertices[v].color = GRAY;
                graph.vertices[v].d = graph.vertices[u].d + 1;
                graph.vertices[v].parent = u;

                std::cout << "Vértice " << v << " marcado como cinza (GRAY) e adicionado à fila" << std::endl;
                std::cout << "Distância do vértice " << v << " ao vértice inicial: " << graph.vertices[v].d << std::endl;
                std::cout << "Pai do vértice " << v << ": " << graph.vertices[v].parent << std::endl;
            }
        }

        graph.vertices[u].color = BLACK;
        std::cout << "Vértice " << u << " marcado como preto (BLACK)" << std::endl;
    }

    std::cout << "Busca em largura concluída" << std::endl;
}

int main() {
    int size = 5;
    Graph graph;
    
    initializeGraph(graph, size);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 4);

    printGraph(graph);
    bfs(graph, 2);

    return 0;
}