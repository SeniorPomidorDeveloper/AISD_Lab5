#include "./libs/interface/menu.h"

int main()
{
    Graph* graph;
    if (init_graph(&graph) == ERROR_ALLOCATION) return ERROR_ALLOCATION;
    int result = main_menu(graph);
    free_Graph(graph);
    return result;
}
