#include "graph.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../error_codes.h"

int init_graph(Graph **graph)
{
    *graph = (Graph *) malloc(sizeof(Graph));
    if (*graph == NULL) return ERROR_ALLOCATION;
    (*graph)->nodes = NULL;
    (*graph)->len = 0;
    return SUCCESS;
}

int add_Node(Graph *graph, const char *name, const unsigned long long int port)
{
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        if (strcmp(name, node->node->name) == 0) return 1; // FOUND
        node = node->next;
    }
    Node_Graph *new_node = (Node_Graph *) malloc(sizeof(Node_Graph));
    if (new_node == NULL) return ERROR_ALLOCATION;
    Node_List_Node_Graph *new_list = (Node_List_Node_Graph *) malloc(sizeof(Node_List_Node_Graph));
    if (new_list == NULL) 
    {
        free(new_node);
        return ERROR_ALLOCATION;
    }
    new_node->name = (char *) malloc((strlen(name) + 1) * sizeof(char));
    if (new_node->name == NULL) 
    {
        free(new_node);
        free(new_list);
        return ERROR_ALLOCATION;
    }
    strcpy(new_node->name, name);
    new_list->node = new_node;
    new_list->next = graph->nodes;
    graph->nodes = new_list;
    new_node->port = port;
    new_node->edges = NULL;
    graph->len++;
    return SUCCESS;
}

int add_Edge(Graph *graph, const char *name1, const char *name2, unsigned long long int *ports, const size_t len_ports,  const unsigned long long int delay)
{
    if (name1 == name2) return 0;
    Node_List_Node_Graph *node = graph->nodes;
    Node_Graph *node1 = NULL, *node2 = NULL;
    while (node != NULL)
    {
        bool result1 = strcmp(name1, node->node->name) == 0; 
        bool result2 = strcmp(name2, node->node->name) == 0; 
        if (result1)
        {
            node1 = node->node;
        }
        else if (result2)
        {
            node2 = node->node;
        }
        node = node->next;
    }
    if (node1 == NULL || node2 == NULL) return -1; // NOT FOUND
    Node_List_Edge_Graph *node_edge = node1->edges;
    while (node_edge != NULL)
    {
        if (strcmp(name2, ((Node_Graph *) node_edge->node->node2)->name) == 0 ||
            strcmp(name2, ((Node_Graph *) node_edge->node->node1)->name) == 0) return 1; // FOUND
        node_edge = node_edge->next;
    }
    Edge_Graph *new_edge = (Edge_Graph *) malloc(sizeof(Edge_Graph));
    if (new_edge == NULL) return ERROR_ALLOCATION;
    new_edge->node1 = node1;
    new_edge->node2 = node2;
    new_edge->ports = (unsigned long long int *) malloc(len_ports * sizeof(unsigned long long int));
    if (new_edge->ports == NULL) return ERROR_ALLOCATION;
    memcpy(new_edge->ports, ports, len_ports * sizeof(unsigned long long int));
    new_edge->delay = delay;
    new_edge->len_ports = len_ports;
    Node_List_Edge_Graph *new_node1_edge = (Node_List_Edge_Graph *) malloc(sizeof(Node_List_Edge_Graph));
    if (new_node1_edge == NULL)
    {
        free(new_edge);
        return ERROR_ALLOCATION;
    }
    Node_List_Edge_Graph *new_node2_edge = (Node_List_Edge_Graph *) malloc(sizeof(Node_List_Edge_Graph));
    if (new_node2_edge == NULL)
    {
        free(new_node1_edge);
        free(new_edge);
        return ERROR_ALLOCATION;
    }
    new_node1_edge->node = new_edge;
    new_node1_edge->next = node1->edges;

    new_node2_edge->node = new_edge;
    new_node2_edge->next = node2->edges;

    node1->edges = new_node1_edge;
    node2->edges = new_node2_edge;
    return SUCCESS;
}

int remove_Edge(Graph *graph, const char *name1, const char *name2)
{
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        if (strcmp(name1, node->node->name) == 0)
        {
            Node_List_Edge_Graph *edges1 = node->node->edges, *edges1_prev = NULL;
            Node_Graph *node1 = node->node;
            while (edges1 != NULL)
            {
                bool result1 = strcmp(name2, ((Node_Graph *) edges1->node->node1)->name) == 0;
                bool result2 = strcmp(name2, ((Node_Graph *) edges1->node->node2)->name) == 0;
                Node_Graph *node2;
                if (result1)
                {
                    node2 = (Node_Graph *) edges1->node->node1;
                }
                else if (result2)
                {
                    node2 = (Node_Graph *) edges1->node->node2;
                }
                if (result1 || result2)
                {
                    Node_List_Edge_Graph *edges2 = node2->edges, *edges2_prev = NULL;
                    while (edges2 != NULL)
                    {
                        if (edges2->node == edges1->node)
                        {
                            if (edges2_prev == NULL)
                            {
                                node2->edges = edges2->next;
                            }
                            else
                            {
                                edges2_prev->next = edges2->next;
                            }
                            free(edges2);
                            break;
                        }
                        edges2_prev = edges2;
                        edges2 = edges2->next;
                    }
                    if (edges1_prev == NULL)
                    {
                        node1->edges = edges1->next;
                    }
                    else
                    {
                        edges1_prev->next = edges1->next;
                    }
                    free(edges1->node->ports);
                    free(edges1->node);
                    free(edges1);
                    return 1; // FOUND
                }
                edges1_prev = edges1;
                edges1 = edges1->next;
            }
        }
        node = node->next;
    }
    return 0; // NOT FOUND
}

int remove_Node(Graph *graph, const char *name)
{
    Node_List_Node_Graph *node = graph->nodes, *node_prev = NULL;
    while (node != NULL)
    {
        if (strcmp(name, node->node->name) == 0)
        {
            Node_List_Edge_Graph *edges = node->node->edges;
            while (edges != NULL)
            {
                remove_Edge(graph, ((Node_Graph *) edges->node->node1)->name, ((Node_Graph *) edges->node->node2)->name);
                edges = node->node->edges;
            }
            if (node_prev == NULL)
            {
                graph->nodes = node->next;
            }
            else
            {
                node_prev->next = node->next;
            }
            free(node->node->name);
            free(node->node);
            free(node);
            graph->len--;
            return 1;
        }
        node_prev = node;
        node = node->next;
    }
    return 0;
}

int change_data_Node(Graph *graph, const char *name, const unsigned long long int port)
{
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        if (strcmp(name, node->node->name) == 0)
        {
            node->node->port = port;
            return 1;
        }
        node = node->next;
    }
    return 0;
}

int change_data_Edge(Graph *graph, const char *name1, const char *name2, unsigned long long int *ports, const size_t len_ports,  const unsigned long long int delay)
{
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        if (strcmp(name1, node->node->name) == 0)
        {
            Node_List_Edge_Graph *edges1 = node->node->edges;
            while (edges1 != NULL)
            {
                bool result1 = strcmp(name2, ((Node_Graph *) edges1->node->node1)->name) == 0;
                bool result2 = strcmp(name2, ((Node_Graph *) edges1->node->node2)->name) == 0;
                if (result1 || result2)
                {
                    edges1->node->delay = delay;
                    free(edges1->node->ports);
                    edges1->node->ports = (unsigned long long int *) malloc(len_ports * sizeof(unsigned long long int));
                    if (edges1->node->ports == NULL) return ERROR_ALLOCATION;
                    memcpy(edges1->node->ports, ports, len_ports * sizeof(unsigned long long int));
                    return 1; // FOUND
                }
                edges1 = edges1->next;
            }
        }
        node = node->next;
    }
    return 0;
}

void free_List(Adjacency_List *list)
{
    Node_Adjacency_List *node = list->head;
    while (node != NULL) 
    {
        Node_List *node_list = node->adjacency;
        while (node_list != NULL)
        {
            Node_List *node_list_prev = node_list;
            node_list = node_list->next;
            free(node_list_prev);
        }
        Node_Adjacency_List *prev = node;
        node = node->next;
        free(prev);
    }
    free(list);
}

int Graph_to_List(const Graph *graph, Adjacency_List **list)
{
    *list = (Adjacency_List *) malloc(sizeof(Adjacency_List));
    if (*list == NULL) return ERROR_ALLOCATION;
    (*list)->head = NULL;
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        Node_Adjacency_List *new_node = (Node_Adjacency_List *) malloc(sizeof(Node_Adjacency_List));
        if (new_node == NULL) 
        {
            free_List(*list);
            return ERROR_ALLOCATION;
        }
        new_node->node = node->node;
        Node_List_Edge_Graph *edges = node->node->edges;
        new_node->adjacency = NULL;
        while (edges != NULL)
        {
            Node_List *new_node_list = (Node_List *) malloc(sizeof(Node_List));
            if (new_node_list == NULL) 
            {
                free_List(*list);
                return ERROR_ALLOCATION;
            }
            new_node_list->node = edges->node->node1 == node->node ? edges->node->node2 : edges->node->node1;
            new_node_list->next = new_node->adjacency;
            new_node->adjacency = new_node_list;
            edges = edges->next;
        }
        new_node->next = (*list)->head;
        (*list)->head = new_node;
        node = node->next;
    }
    return SUCCESS;
}

int Graph_to_Matrix(const Graph *graph, Adjacency_matrix **matrix, const unsigned long long int *port)
{
    *matrix = (Adjacency_matrix *) malloc(sizeof(Adjacency_matrix));
    if (*matrix == NULL) return ERROR_ALLOCATION;
    (*matrix)->matrix = (int **) malloc(graph->len * sizeof(int *));
    if ((*matrix)->matrix == NULL)
    {
        free(*matrix);
        return ERROR_ALLOCATION;
    }
    (*matrix)->len = graph->len;
    Node_List_Node_Graph *node = graph->nodes;
    for (size_t i = 0; i < graph->len; ++i)
    {
        node->node->index = i;
        (*matrix)->matrix[i] = (int *) malloc(graph->len * sizeof(unsigned long long int));
        if ((*matrix)->matrix[i] == NULL)
        {
            free((*matrix)->matrix);
            free(*matrix);
            return ERROR_ALLOCATION;
        }
        for (size_t j = 0; j < graph->len; ++j)
        {
            (*matrix)->matrix[i][j] = 0;
        }
        node = node->next;
    }
    node = graph->nodes;
    while (node != NULL)
    {
        Node_List_Edge_Graph *edges = node->node->edges;
        while (edges != NULL)
        {
            if (port != NULL)
            {
                size_t i;
                for (i = 0; i < edges->node->len_ports && edges->node->ports[i] != *port; ++i);
                if (i == edges->node->len_ports) 
                {
                    edges = edges->next;
                    continue;
                }
            }
            size_t index1 = ((Node_Graph *) edges->node->node1)->index;
            size_t index2 = ((Node_Graph *) edges->node->node2)->index;
            (*matrix)->matrix[index1][index2] = edges->node->delay;
            (*matrix)->matrix[index2][index1] = edges->node->delay;
            edges = edges->next;
        }
        node = node->next;
    }
    return SUCCESS;
}

void free_Matrix(Adjacency_matrix *matrix)
{
    for (size_t i = 0; i < matrix->len; ++i)
    {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    free(matrix);
}

int init_Queue(Int_Queue **queue)
{
    *queue = (Int_Queue *) malloc(sizeof(Int_Queue));
    if (*queue == NULL) return ERROR_ALLOCATION;
    (*queue)->head = NULL;
    (*queue)->tail = NULL;
    return SUCCESS;
}

int push_back(Int_Queue *list, size_t value)
{
    Node_Int_Queue *new_node = (Node_Int_Queue *) malloc(sizeof(Node_Int_Queue));
    if (new_node == NULL) return ERROR_ALLOCATION;
    new_node->data = value;
    new_node->next = NULL;
    if (list->head == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    return SUCCESS;
}

int push(Int_Queue *list, const int value)
{
    Node_Int_Queue *new_ptr = (Node_Int_Queue *) malloc(sizeof(Node_Int_Queue));
    if (new_ptr == NULL) return ERROR_ALLOCATION;
    new_ptr->data = value;
    new_ptr->next = list->head;
    list->head = new_ptr;
    if (list->tail == NULL) list->tail = new_ptr;
    return SUCCESS;
}

void pop(Int_Queue* list)
{
    Node_Int_Queue *prev = list->head;
    list->head = list->head->next;
    free(prev);
    if (list->head == NULL) list->tail = NULL;
}

bool Queue_is_empty(Int_Queue *list)
{
    return list->head == NULL;
}

void free_Queue(Int_Queue *queue)
{
    Node_Int_Queue *node = queue->head;
    while (node != NULL)
    {
        Node_Int_Queue *prev = node;
        node = node->next;
        free(prev);
    }
    free(queue);
}

int bypass_Graph(const Graph *graph, const Adjacency_matrix *matrix, const char *name, const unsigned long long int *port, bool **visited)
{

    *visited = (bool *) calloc(matrix->len , sizeof(bool));
    Int_Queue *q;
    if (init_Queue(&q) == ERROR_ALLOCATION) return ERROR_ALLOCATION; 
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        if (strcmp(name, node->node->name) == 0)
        {
            (*visited)[node->node->index] = true;
            push_back(q, node->node->index);
            break;
        }
        node = node->next;
    }
    if (node == NULL) return 1;
    size_t vis;
    while (!Queue_is_empty(q))
    {
        vis = q->head->data;
        Node_List_Node_Graph *node = graph->nodes;
        for (size_t i = 1; i <= vis; ++i) node = node->next;
        if (port != NULL) {if (node->node->port == *port) printf("%s ", node->node->name);}
        else printf("%s ", node->node->name);
        pop(q);

        for (size_t i = 0; i < matrix->len; ++i)
        {
            if (matrix->matrix[vis][i] != 0 && matrix->matrix[vis][i] != INF && !(*visited)[i])
            {
                (*visited)[i] = true;
                push_back(q, i);
            }
        }
    }
    free_Queue(q);
    printf("\n");
    return SUCCESS;
}

int deikstra(Adjacency_matrix *graph, const int from, const int to, Int_Queue *result)
{
    int *distance = (int *) malloc(graph->len * sizeof(int));
    if (distance == NULL) return -2;
    int *visit = (int *) malloc(graph->len * sizeof(int));
    if (visit == NULL) return -2;
    if (visit == NULL) 
    {
        free(distance);
        return ERROR_ALLOCATION;
    }
    int *v = (int *) malloc(graph->len * sizeof(int));
    if (v == NULL) 
    {
        free(distance);
        free(visit);
        return ERROR_ALLOCATION;
    }
    for (size_t i = 0; i < graph->len; ++i)
    {
        distance[i] = INF;
        visit[i] = -1;
        v[i] = false;
    }
    distance[from] = 0;
    int pos = from;
    int next_pos = from;
    while (pos != to)
    {
        v[pos] = true;
        int min_d = INF;
        for (int i = 0; i < (int) graph->len; ++i)
        {
            if (pos != i && graph->matrix[pos][i] > 0 && !v[i])
            {
                if (distance[i] > distance[pos] + graph->matrix[pos][i])
                {
                   distance[i] = distance[pos] + graph->matrix[pos][i];
                   visit[i] = pos;
                }
                if (graph->matrix[pos][i] < min_d)
                {
                    next_pos = i;
                    min_d = graph->matrix[pos][i];
                }
            }
        }
        if (next_pos == pos) return -1;
        pos = next_pos;
    }
    v[to] = true;
    int next = to;
    while (next != -1)
    {
        push(result, next);
        next = visit[next];
    }
    int answer = distance[to];
    free(distance);
    free(visit);
    free(v);
    return answer;
}

int find_shortcut(const Graph *graph, const char *from, const char *to, Int_Queue **result, int *len)
{
    *len = 0;
    if (init_Queue(result) == ERROR_ALLOCATION) return ERROR_ALLOCATION;
    size_t index_from;
    size_t index_to;
    Node_List_Node_Graph *node = graph->nodes;
    for (index_from = 0; index_from < graph->len && strcmp(node->node->name, from) != 0; index_from++) node = node->next;
    if (index_from == graph->len)
    {
        free_Queue(*result);
        return -3;
    }
    node = graph->nodes;
    for (index_to = 0; index_to < graph->len && strcmp(node->node->name, to) != 0; index_to++) node = node->next;
    if (index_to == graph->len)
    {
        free_Queue(*result);
        return -3;
    }
    Adjacency_matrix *matrix;
    if (Graph_to_Matrix(graph, &matrix, &(node->node->port)) == -2) return -2;
    *len = deikstra(matrix, index_from, index_to, *result);
    free_Matrix(matrix);
    return SUCCESS;
}

int splitting_Graph(Graph *graph)
{
    Adjacency_matrix *matrix;
    if (Graph_to_Matrix(graph, &matrix, NULL) == ERROR_ALLOCATION) return ERROR_ALLOCATION;
    bool *visited_old = (bool *) calloc(graph->len, sizeof(bool));
    if(visited_old == NULL) return ERROR_ALLOCATION;
    bool *visited;
    size_t com_num = 1;
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL) 
    {
        if (!visited_old[node->node->index])
        {
            printf("%zu) ", com_num);
            bypass_Graph(graph, matrix, node->node->name, NULL, &visited);
            for (size_t i = 0; i < graph->len; ++i)
            {
                visited_old[i] = visited[i] || visited_old[i];
            }
            ++com_num;
            free(visited);
        }
        node = node->next;
    }
    free(visited_old);
    free_Matrix(matrix);
    return SUCCESS;
}

void free_Graph(Graph *graph)
{
    Node_List_Node_Graph *node = graph->nodes;
    while (node != NULL)
    {
        remove_Node(graph, node->node->name);
        node = graph->nodes;
    }
    free(graph);
}