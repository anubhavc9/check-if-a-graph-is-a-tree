#include <stdio.h>
#include <stdlib.h>

struct ArrayQueue
{
    int front, rear;
    int capacity;
    int *array;
};

struct ArrayQueue* createQueue(int capacity)
{
    struct ArrayQueue *Q;
    Q = malloc(sizeof(struct ArrayQueue));
    if(!Q)
        return (NULL);
    Q->capacity = capacity;
    Q->front = Q->rear = -1;
    Q->array = malloc(sizeof(int)*capacity);
}
int isEmptyQueue(struct ArrayQueue *Q)
{
    return(Q->front == -1);
}
int isFullQueue(struct ArrayQueue *Q)
{
    return((Q->rear+1)%Q->capacity==Q->front);
}
void enQueue(struct ArrayQueue *Q, int data)
{
    if(isFullQueue(Q))
        printf("Overflow");
    else
    {
        Q->rear = (Q->rear + 1) % Q->capacity;
        Q->array[Q->rear] = data;
        if(Q->front == -1) // first time enqueue
            Q->front = Q->rear;
    }
}
int deQueue(struct ArrayQueue *Q)
{
    int data = -1;
    if(isEmptyQueue(Q))
    {
        printf("Queue is empty\n");
        return (-1);
    }
    else
    {
        data = Q->array[Q->front];
        if(Q->front == Q->rear)
            Q->front = Q->rear = -1;
        else
            Q->front=(Q->front+1)%Q->capacity;
    }
    return data;
}
void display(struct ArrayQueue *Q)
{
    int i;
    if (Q->front == - 1)
        printf("Queue is empty \n");
    else
    {
        for (i = Q->front; i <= Q->rear; i++)
            printf("%d\t",Q->array[i]);
    }
}
struct AdjListNode
{
    int dest;
    struct AdjListNode *next;
};
struct AdjList
{
    struct AdjListNode *head;
};
struct Graph
{
    int V;
    struct AdjList *array; // an array of pointers to the head node of all adj lists
    int *visited;
};
struct AdjListNode *newAdjListNode(int dest)
{
    struct AdjListNode *newnode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newnode->dest = dest;
    newnode->next = NULL;
    return newnode;
}
struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));
    graph->visited = (struct Graph *)malloc(V * sizeof(int));
    int i;
    for(i = 0; i < V; i++)
    {
        graph->array[i].head = NULL;
        graph->visited[i] = -1;
    }
    return graph;
}
void addEdge(struct Graph *graph, int src, int dest)
{
    struct AdjListNode *newnode = newAdjListNode(dest);
    newnode->next = graph->array[src].head;
    graph->array[src].head = newnode;

    newnode = newAdjListNode(src);
    newnode->next = graph->array[dest].head;
    graph->array[dest].head = newnode;
}
void printGraph(struct Graph *graph)
{
    int v;
    for(v = 0; v < graph->V; v++)
    {
        struct AdjListNode *pCrawl = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\nhead", v);
        while(pCrawl)
        {
            printf(", %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}
int BFS(struct Graph *graph, int startVertex, int traversal[])
{
    /*visited = 1 (vertex is visited), visited = 0 (vertex is in queue), vertex = -1 (vertex is unvisited)*/
    int traversal_index = 0;
    struct ArrayQueue *Q = createQueue(100);

    graph->visited[startVertex] = 0; // put the starting vertex as 0 & enqueue it
    enQueue(Q, startVertex);

    while(!isEmptyQueue(Q))
    {
        int current_vertex = deQueue(Q);
        //printf("Visited: %d\n",current_vertex);
        traversal[traversal_index] = current_vertex;
        traversal_index++;
        graph->visited[current_vertex] = 1;

        struct AdjListNode *temp = graph->array[current_vertex].head;
        while(temp)
        {
            int adjVertex = temp->dest;
            /*if any of current vertex's adjacent vertex is already present in the queue, graph contains cycle*/
            if(graph->visited[adjVertex] == 0)
                return 1;
            /*if current vertex's adjacent vertex is unvisited, enqueue it & mark it as 0*/
            if(graph->visited[adjVertex] == -1)
            {
                graph->visited[adjVertex] = 0;
                enQueue(Q, adjVertex);
            }
            temp = temp->next;
        }
    }
    return 0;
}
int main()
{
    int V, choice, src, dest, i;
    int notConnectedFlag = 0, hasCycleFlag;
    printf("Enter the number of vertices: ");
    scanf("%d",&V);
    int traversal[V];

    struct Graph *graph = createGraph(V);

    /*Initializing the traversal array to all -1*/
    for(i = 0; i < V; i++)
        traversal[i] = -1;

    while(1)
    {
        printf("1. Add an edge\n");
        printf("2. Display the graph\n");
        printf("3. Check if the graph is a tree\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            while(1)
            {
                printf("Enter edge in the form 'a b'\n");
                scanf("%d %d", &src, &dest);
                if(src >= 0 && src < V && dest >= 0 && dest < V)
                    break;
                else
                    printf("Invalid edge!\n");
            }
            addEdge(graph, src, dest);
            break;
        case 2:
            printGraph(graph);
            break;
        case 3:
            /*An undirected graph is a tree if it has no cycle & it is connected*/
            hasCycleFlag = BFS(graph, 0, traversal);
            if(hasCycleFlag == 1)
                printf("Graph contains a cycle\n");
            else
                printf("No cycle in the graph\n");

            printf("BFS traversal of the graph:\n");
            for(i = 0; i < graph->V; i++)
                printf("%d\t",traversal[i]);
            printf("\n");

            /*if the graph has no cycle, check if the graph is connected*/
            if(hasCycleFlag == 0)
            {
                for(i = 0; i < V; i++)
                {
                    if(traversal[i] == -1)
                        notConnectedFlag = 1;
                }
                if(notConnectedFlag)
                    printf("Graph is not connected\n");
                else
                    printf("Graph is connected\n");
            }
            if(!hasCycleFlag && !notConnectedFlag)
                printf("Graph is a tree, hence it is minimally connected\n");
            else
                printf("Graph is not a tree. So it is not minimally connected\n");
            break;
        case 4:
            exit(0);
        }
    }
    return 0;
}
