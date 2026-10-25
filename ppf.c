
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V 9
#define NAME 40
#define QUEUE_SIZE V

/* =========================================
   CAMPUS ROUTE FINDER
   Language: C
   Concepts: Graph, Adjacency List, BFS, DFS
   ========================================= */

typedef struct Node
{
    int vertex;
    struct Node *next;
} Node;

typedef struct
{
    char name[NAME];
    Node *head;
} Location;

typedef struct
{
    int vertices;
    Location campus[V];
} Graph;

/* Campus locations */
const char *locationNames[V] = {
    "Main Gate",
    "Library",
    "Block 1",
    "Block 2",
    "Cafeteria",
    "Auditorium",
    "Parking",
    "Computer Lab",
    "Sports Complex"
};

/* ---------- Utility Functions ---------- */

void line(void)
{
    printf("================================================\n");
}

void pauseScreen(void)
{
    printf("\nPress ENTER to continue...");
    getchar();
}

void initializeGraph(Graph *g)
{
    int i;

    g->vertices = V;

    for (i = 0; i < V; i++)
    {
        strcpy(g->campus[i].name, locationNames[i]);
        g->campus[i].head = NULL;
    }
}

/* ---------- Graph Operations ---------- */

/* Add an undirected edge using adjacency list */
void addEdge(Graph *g, int u, int v)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->vertex = v;
    newNode->next = g->campus[u].head;
    g->campus[u].head = newNode;

    newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->vertex = u;
    newNode->next = g->campus[v].head;
    g->campus[v].head = newNode;
}

void createCampus(Graph *g)
{
    initializeGraph(g);

    addEdge(g, 0, 1); /* Main Gate - Library */
    addEdge(g, 0, 6); /* Main Gate - Parking */
    addEdge(g, 1, 2); /* Library - Block 1 */
    addEdge(g, 1, 4); /* Library - Cafeteria */
    addEdge(g, 2, 3); /* Block 1 - Block 2 */
    addEdge(g, 2, 7); /* Block 1 - Computer Lab */
    addEdge(g, 3, 5); /* Block 2 - Auditorium */
    addEdge(g, 4, 5); /* Cafeteria - Auditorium */

    /* Sports Complex is intentionally disconnected */
}

void displayLocations(Graph *g)
{
    int i;

    line();
    printf("             CAMPUS LOCATIONS\n");
    line();

    for (i = 0; i < g->vertices; i++)
        printf("  %d. %s\n", i + 1, g->campus[i].name);

    printf("\nEnter the location NUMBER when prompted.\n");
}

void displayGraph(Graph *g)
{
    int i;
    Node *temp;

    line();
    printf("           CAMPUS ADJACENCY LIST\n");
    line();

    for (i = 0; i < g->vertices; i++)
    {
        printf("\n%-18s -> ", g->campus[i].name);

        temp = g->campus[i].head;

        if (temp == NULL)
            printf("No connections");

        while (temp != NULL)
        {
            printf("%s", g->campus[temp->vertex].name);

            if (temp->next != NULL)
                printf(" -> ");

            temp = temp->next;
        }

        printf("\n");
    }
}

/* ---------- BFS: Shortest Route ---------- */

void findShortestRoute(Graph *g)
{
    int src, dest, i;
    int queue[QUEUE_SIZE];
    int front = 0, rear = 0;
    int visited[V] = {0};
    int parent[V];
    int path[V], count = 0;
    int u;

    displayLocations(g);

    printf("\nEnter source number: ");
    if (scanf("%d", &src) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    printf("Enter destination number: ");
    if (scanf("%d", &dest) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    if (src < 1 || src > V || dest < 1 || dest > V)
    {
        printf("\n[ERROR] Invalid location number.\n");
        return;
    }

    src--;
    dest--;

    for (i = 0; i < V; i++)
        parent[i] = -1;

    visited[src] = 1;
    queue[rear++] = src;

    while (front < rear)
    {
        u = queue[front++];

        if (u == dest)
            break;

        Node *temp = g->campus[u].head;

        while (temp != NULL)
        {
            int v = temp->vertex;

            if (!visited[v])
            {
                visited[v] = 1;
                parent[v] = u;
                queue[rear++] = v;
            }

            temp = temp->next;
        }
    }

    if (!visited[dest])
    {
        printf("\n[NO ROUTE FOUND]\n");
        printf("%s is unreachable from %s.\n",
               g->campus[dest].name,
               g->campus[src].name);
        return;
    }

    /* Reconstruct path from destination to source */
    u = dest;

    while (u != -1)
    {
        path[count++] = u;
        u = parent[u];
    }

    line();
    printf("              SHORTEST ROUTE\n");
    line();

    for (i = count - 1; i >= 0; i--)
    {
        printf("%s", g->campus[path[i]].name);

        if (i > 0)
            printf("  -->  ");
    }

    printf("\n\nTotal edges: %d\n", count - 1);
    printf("Algorithm: Breadth-First Search (BFS)\n");
}

/* ---------- DFS: Reachable Locations ---------- */

void dfsUtil(Graph *g, int u, int visited[])
{
    Node *temp;

    visited[u] = 1;
    printf("  [VISITED] %s\n", g->campus[u].name);

    temp = g->campus[u].head;

    while (temp != NULL)
    {
        int v = temp->vertex;

        if (!visited[v])
            dfsUtil(g, v, visited);

        temp = temp->next;
    }
}

void exploreCampus(Graph *g)
{
    int src, i;
    int visited[V] = {0};

    displayLocations(g);

    printf("\nEnter starting location number: ");

    if (scanf("%d", &src) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    if (src < 1 || src > V)
    {
        printf("Invalid location number.\n");
        return;
    }

    line();
    printf("           DFS CAMPUS EXPLORATION\n");
    line();

    dfsUtil(g, src - 1, visited);

    printf("\nDisconnected / unreachable locations:\n");

    for (i = 0; i < V; i++)
    {
        if (!visited[i])
            printf("  [UNREACHABLE] %s\n", g->campus[i].name);
    }
}

/* ---------- Connectivity Report ---------- */

void connectivityReport(Graph *g)
{
    int visited[V] = {0};
    int i, components = 0;

    line();
    printf("             CONNECTIVITY REPORT\n");
    line();

    for (i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            components++;
            printf("\nConnected component %d:\n", components);
            dfsUtil(g, i, visited);
        }
    }

    printf("\nTotal connected components: %d\n", components);

    if (components == 1)
        printf("All campus locations are connected.\n");
    else
        printf("Some campus locations are disconnected.\n");
}

/* ---------- Free Allocated Memory ---------- */

void freeGraph(Graph *g)
{
    int i;

    for (i = 0; i < g->vertices; i++)
    {
        Node *temp = g->campus[i].head;

        while (temp != NULL)
        {
            Node *next = temp->next;
            free(temp);
            temp = next;
        }

        g->campus[i].head = NULL;
    }
}

/* ---------- Main Menu ---------- */

int main(void)
{
    Graph campus;
    int choice;

    createCampus(&campus);

    do
    {
        printf("\n\n");
        line();
        printf("           CAMPUS ROUTE FINDER\n");
        printf("        Smart Campus Navigation\n");
        line();

        printf("  1. View Campus Locations\n");
        printf("  2. View Adjacency List\n");
        printf("  3. Find Shortest Route (BFS)\n");
        printf("  4. Explore Reachable Locations (DFS)\n");
        printf("  5. Campus Connectivity Report\n");
        printf("  6. Exit\n");

        line();
        printf("  Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Exiting safely.\n");
            break;
        }

        switch (choice)
        {
            case 1:
                displayLocations(&campus);
                break;

            case 2:
                displayGraph(&campus);
                break;

            case 3:
                findShortestRoute(&campus);
                break;

            case 4:
                exploreCampus(&campus);
                break;

            case 5:
                connectivityReport(&campus);
                break;

            case 6:
                printf("\nThank you for using Campus Route Finder!\n");
                break;

            default:
                printf("\nInvalid choice. Please select 1-6.\n");
        }

    } while (choice != 6);

    freeGraph(&campus);

    return 0;
}

