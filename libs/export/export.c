#include "export.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error_codes.h"

int export_graph(const Graph *graph, const char *path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return ERROR_FILE_OPEN;
    }
    if (fprintf(file, "graph Graph {\n\tnode [shape = circle];\n") < 37)
    {
        fclose(file);
        return ERROR_FILE_WRITE;
    }
    Node_List_Node_Graph *node = graph->nodes;
    Node_List_Edge_Graph *visited = NULL;
    while (node!= NULL)
    {
        Node_List_Edge_Graph *edge = node->node->edges;
        printf("key: %s\n", node->node->name);
        if (edge == NULL)
        {
            if (fprintf(file, "\t%s;\n", node->node->name) < 4)
            {
                fclose(file);
                return ERROR_FILE_WRITE;
            }
        }
        while (edge!= NULL)
        {
            Node_List_Edge_Graph *node_vis = visited;
            bool check = true;
            while (node_vis != NULL)
            {
                if (node_vis->node == edge->node)
                {
                    check = false;
                    break;
                }
                node_vis = node_vis->next;
            }
            if (check)
            {
                node_vis = (Node_List_Edge_Graph *) malloc(sizeof(Node_List_Edge_Graph));
                node_vis->node = edge->node;
                node_vis->next = visited;
                visited = node_vis;
            }
            if (check) if (fprintf(file, "\t%s -- %s;\n", ((Node_Graph *) edge->node->node1)->name, ((Node_Graph *) edge->node->node2)->name) < 8)
            {
                fclose(file);
                return ERROR_FILE_WRITE;
            }
            edge = edge->next;
        }

        node = node->next;
    }
    if (fprintf(file, "}\n") != 2)
    {
        fclose(file);
        return ERROR_FILE_WRITE;
    }
    fclose(file);
    while (visited != NULL)
    {
        Node_List_Edge_Graph *prev = visited;
        visited = visited->next;
        free(prev);
    }
    return SUCCESS;
}