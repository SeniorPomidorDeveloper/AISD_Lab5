#ifndef GRAPH_H
#define GRAPH_H

#include "structs.h"

int init_graph(Graph **graph);

int add_Node(Graph *graph, const char *name, const unsigned long long int port);
int add_Edge(Graph *graph, const char *name1, const char *name2, unsigned long long int *ports, const size_t len_ports,  const unsigned long long int delay);

int remove_Edge(Graph *graph, const char *name1, const char *name2);
int remove_Node(Graph *graph, const char *name);

int change_data_Node(Graph *graph, const char *name, const unsigned long long int port);
int change_data_Edge(Graph *graph, const char *name1, const char *name2, unsigned long long int *ports, const size_t len_ports,  const unsigned long long int delay);

int Graph_to_List(const Graph *graph, Adjacency_List **list);
void free_List(Adjacency_List *list);

void free_Queue(Int_Queue *queue);

int Graph_to_Matrix(const Graph *graph, Adjacency_matrix **matrix, const unsigned long long int *port);
void free_Matrix(Adjacency_matrix *matrix);

int bypass_Graph(const Graph *graph, const Adjacency_matrix *matrix, const char *name, const unsigned long long int *port, bool **visited);
int find_shortcut(const Graph *graph, const char *from, const char *to, Int_Queue **result, int *len);
int splitting_Graph(Graph *graph);

void free_Graph(Graph *graph);

#endif // GRAPH_H