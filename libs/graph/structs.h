#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

typedef struct Edge_Graph
{
    void *node1;
    void *node2;
    unsigned long long int *ports;
    size_t len_ports;
    unsigned long long int delay;
} Edge_Graph;

typedef struct Node_List_Edge_Graph
{
    Edge_Graph *node;
    struct Node_List_Edge_Graph *next;
} Node_List_Edge_Graph;

typedef struct Node_Graph
{
    char *name;
    unsigned long long int port;
    struct Node_List_Edge_Graph *edges;
    size_t index;
} Node_Graph;

typedef struct Node_List_Node_Graph
{
    Node_Graph *node;
    struct Node_List_Node_Graph *next;
} Node_List_Node_Graph;

typedef struct Graph
{
    Node_List_Node_Graph *nodes;
    size_t len;
} Graph;

typedef struct Adjacency_matrix
{
    int **matrix;
    size_t len;
} Adjacency_matrix;

typedef struct Node_List
{
    Node_Graph *node;
    struct Node_List *next;
} Node_List;

typedef struct Node_Adjacency_List
{
    Node_Graph *node;
    Node_List *adjacency;
    struct Node_Adjacency_List *next;
} Node_Adjacency_List;

typedef struct Adjacency_List
{
    Node_Adjacency_List *head;
} Adjacency_List;

typedef struct Node_Int_Queue
{
    size_t data;
    struct Node_Int_Queue *next;
} Node_Int_Queue;

typedef struct Int_Queue
{
    Node_Int_Queue *head;
    Node_Int_Queue *tail;
} Int_Queue;

#endif // STRUCTS_H