#ifndef MENU_H
#define MENU_H

#include "../error_codes.h"
#include "../graph/graph.h"

int menu_add_Node(Graph *graph);
int menu_add_Edge(Graph *graph);
int menu_change_Node(Graph *graph);
int menu_change_Edge(Graph *graph);
int menu_remove_Node(Graph *graph);
int menu_remove_Edge(Graph *graph);
int menu_Graph_to_List(Graph *graph);
int menu_bypass(Graph *graph);
int menu_find_shortcut(Graph *graph);
int menu_splitting(Graph *graph);

int export_menu(Graph *graph);

int main_menu(Graph *graph);

#endif // MENU_H