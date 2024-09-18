#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream> 
#include <stdexcept> 
#include <algorithm> 

struct Node
{
    int data;
    Node *next;
};

struct Vertex;

Node *createNode(int value)
{
    Node *newNode = new Node();
    newNode->data = value;
    newNode->next = nullptr;
    return newNode;
}

void enqueue(Node **front, Node **rear, int value)
{
    Node *newNode = createNode(value);

    if (*rear)
    {
        (*rear)->next = newNode;
        *rear = newNode;
    }
    else
    {
        *front = *rear = newNode;
    }
}

void dequeue(Node **front, Node **rear)
{
    if (*front)
    {
        Node *temp = *front;
        *front = (*front)->next;

        if (*front == nullptr)
        {
            *rear = nullptr;
        }

        delete temp;
    }
}

int getFront(Node *front)
{
    if (front)
    {
        return front->data;
    }
    else
    {
        throw std::runtime_error("Queue is empty");
    }
}

bool isEmpty(Node *front)
{
    return front == nullptr;
}


enum Color
{
    WHITE,
    GRAY,
    BLACK
};

struct Vertex
{
    int id;
    Color color;
    int parent;
    int timeI;
    int timeF;
    int d;

    Vertex(int id = 0, Color color = WHITE, int parent = -1)
        : id(id), color(color), parent(parent), timeI(-1), timeF(-1), d(-1) {}
};

struct Graph
{
    std::vector<std::vector<int>> adj;
    std::vector<Vertex> vertices;
    int timeG;
    bool isDirected;
};

void initializeGraph(Graph &graph, int size)
{
    graph.adj.resize(size);
    graph.vertices.resize(size);
    for (int i = 0; i < size; ++i)
    {
        graph.vertices[i] = Vertex(i);
    }
}

void addEdge(Graph &graph, int u, int v)
{
    graph.adj[u].push_back(v);
    if (!graph.isDirected)
    {
        graph.adj[v].push_back(u);
    }
}

void printGraph(const Graph &graph)
{
    for (int i = 0; i < graph.adj.size(); ++i)
    {
        std::cout << "Vértice " << graph.vertices[i].id << " (Cor: " << graph.vertices[i].color << ", Pai: " << graph.vertices[i].parent << "): ";
        for (int v : graph.adj[i])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

struct ListNode
{
    Vertex data;
    ListNode *next;

    ListNode(const Vertex &v) : data(v), next(nullptr) {}
};

void printList(ListNode *head)
{
    while (head != nullptr)
    {
        std::cout << head->data.id << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

void bfs(Graph &graph, int startVertexIndex)
{
    std::vector<bool> visited(graph.adj.size(), false);
    Node *front = nullptr;
    Node *rear = nullptr;

    std::cout << "Iniciando busca em largura a partir do vértice " << startVertexIndex << std::endl;

    enqueue(&front, &rear, startVertexIndex);
    visited[startVertexIndex] = true;

    graph.vertices[startVertexIndex].color = GRAY;
    graph.vertices[startVertexIndex].d = 0;

    std::cout << "Vértice " << startVertexIndex << " marcado como cinza (GRAY) e adicionado à fila" << std::endl;

    while (!isEmpty(front))
    {
        int u = getFront(front);
        dequeue(&front, &rear);

        std::cout << "Desenfileirando vértice " << u << std::endl;

        for (int v : graph.adj[u])
        {
            if (!visited[v])
            {
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

// BUSCA EM PROFUNDIDADE
void dfsVisit(Graph &graph, int u)
{
    graph.timeG = graph.timeG + 1;
    graph.vertices[u].timeI = graph.timeG;
    graph.vertices[u].color = GRAY;

    std::cout << "Visitando vértice " << u << " e pintando de (CINZA). T[" << graph.timeG << "]" << std::endl;

    for (int v : graph.adj[u])
    {
        if (graph.vertices[v].color == WHITE)
        {
            graph.vertices[v].parent = u;
            dfsVisit(graph, v);
        }
    }

    graph.vertices[u].color = BLACK;
    graph.timeG = graph.timeG + 1;
    graph.vertices[u].timeF = graph.timeG;

    std::cout << "Vértice " << u << " marcado como preto (BLACK). T[" << graph.timeG << "]" << std::endl;
}

void dfs(Graph &graph)
{
    for (int i = 0; i < graph.adj.size(); ++i)
    {
        graph.vertices[i].color = WHITE;
        graph.vertices[i].parent = -1;
    }
    graph.timeG = 0;
    std::cout << "\n\n\n---- BUSCA EM PROFUNDIDADE ----" << std::endl;

    for (int i = 0; i < graph.adj.size(); ++i)
    {
        if (graph.vertices[i].color == WHITE)
        {
            std::cout << "Iniciando busca em profundidade a partir do vértice " << i << std::endl;
            dfsVisit(graph, graph.vertices[i].id);
        }
    }

    std::cout << "Busca em profundidade concluída" << std::endl;
}

Vertex findTime(Graph &graph)
{
    int minTimeF = RAND_MAX;
    Vertex minVertex;
    for (const Vertex &v : graph.vertices)
    {
        if (v.timeF < minTimeF)
        {
            minTimeF = v.timeF;
            minVertex = v;
        }
    }
    return minVertex;
}


void addToListN(ListNode **head, const Vertex &v)
{
    ListNode *newNode = new ListNode(v);
    newNode->next = *head;
    *head = newNode;
}

void topological_sort(Graph &graph)
{
    dfs(graph);

    ListNode *lista = nullptr;

    while (!graph.vertices.empty())
    {
        Vertex v = findTime(graph);
        if (v.timeF != -1)
        {
            addToListN(&lista, v);
            graph.vertices.erase(std::remove_if(graph.vertices.begin(), graph.vertices.end(),
                [v](const Vertex &vertex) { return vertex.id == v.id; }), graph.vertices.end());

            for (auto &neighbors : graph.adj)
            {
                neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), v.id), neighbors.end());
            }
        }
    }

    printList(lista);

    ListNode *current = lista;
    while (current)
    {
        ListNode *temp = current;
        current = current->next;
        delete temp;
    }
}

void transpose(Graph &graph) {
    std::vector<std::vector<int>> adjT(graph.adj.size());

    for (int u = 0; u < graph.adj.size(); ++u) {
        for (int v : graph.adj[u]) {
            adjT[v].push_back(u);
        }
    }

    graph.adj = adjT;
}

void printSCC(const std::vector<std::vector<int>> &scc, const Graph &graph) {
    std::cout << "Componentes Fortemente Conectados (SCCs):" << std::endl;
    for (const auto &component : scc) {
        std::cout << "SCC: ";
        for (int v : component) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

void dfsVisit(Graph &graph, int u, std::vector<bool> &visited, std::vector<int> &scc) {
    visited[u] = true;
    scc.push_back(u);

    for (int v : graph.adj[u]) {
        if (!visited[v]) {
            dfsVisit(graph, v, visited, scc);
        }
    }
}

void strongly_connected_components(Graph &graph) {
    dfs(graph);

    transpose(graph);

    std::vector<std::vector<int>> sccs; 
    std::vector<bool> visited(graph.adj.size(), false);
    for (int i = 0; i < graph.adj.size(); ++i) {
        if (!visited[i]) {
            std::vector<int> scc;
            dfsVisit(graph, i, visited, scc);
            sccs.push_back(scc);
        }
    }

    printSCC(sccs, graph);
}

void readGraphFile(const std::string &filename, Graph &graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo.");
    }

    int type;
    file >> type;
    if (type != 0 && type != 1) {
        throw std::runtime_error("Tipo de grafo inválido.");
    }
    graph.isDirected = (type == 1);

    int numVertices;
    file >> numVertices;
    if (numVertices < 0) {
        throw std::runtime_error("Número de vértices inválido.");
    }
    initializeGraph(graph, numVertices);

    int u, v;
    while (file >> u >> v) {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            throw std::runtime_error("Aresta com vértices fora do intervalo.");
        }
        addEdge(graph, u, v);
    }

    file.close();
}

int main()
{
    Graph graph;

    try {
        readGraphFile("input.txt", graph);
        printGraph(graph);
        dfs(graph);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
