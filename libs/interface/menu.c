#include "menu.h"

#include <stdlib.h>
#include <stdio.h>

#include "./libs/gets/gets.h"
#include "./libs/input/input.h"
#include "../export/export.h"

int menu_add_Node(Graph *graph)
{
    char *name;
    long long int port;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name, "\t> Введите имя компьютера: ") == EOF) return EOF;
    if (GetLLInt(&port, "\t> Введите порт: ", check_no_negative) == EOF)
    {
        free(name);
        return EOF;
    }
    int result = add_Node(graph, name, port);
    free(name);
    if (result == 1)
    {
        printf("\t> Компьютер с таким именем уже существует!\n");
        return SUCCESS;
    }
    return result;
}

int menu_add_Edge(Graph *graph)
{
    char *name1;
    char *name2;
    long long int ports_len;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name1, "\t> Введите имя компьютера №1: ") == EOF) return EOF;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name2, "\t> Введите имя компьютера №2: ") == EOF) 
    {
        free(name1);
        return EOF;
    }
    if (GetLLInt(&ports_len, "Введите кол-во порт: ", check_positive) == EOF) 
    {
        free(name1);
        free(name2);
        return EOF;
    }
    unsigned long long int *ports = (unsigned long long int *) malloc(ports_len * sizeof(unsigned long long int));
    if (ports == NULL) return ERROR_ALLOCATION;
    for (long long int i = 0; i < ports_len; i++) 
    {
        long long int port;
        if (GetLLInt(&port, "\t> Введите порт: ", check_no_negative) == EOF) 
        {
            free(name1);
            free(name2);
            free(ports);
            return EOF;
        }
        ports[i] = (unsigned long long int) port;
    }
    long long int delay;
    if (GetLLInt(&delay, "\t> Введите задержку: ", check_no_negative) == EOF)
    {
        free(name1);
        free(name2);
        free(ports);
        return EOF;
    }
    int result = add_Edge(graph, name1, name2, ports, ports_len, delay);
    if (result == 1) 
    {
        free(name1);
        free(name2);
        free(ports);
        printf("\t> Такое ребро уже существует!\n");
        return SUCCESS;
    }
    if (result == -1)
    {
        free(name1);
        free(name2);
        free(ports);
        printf("\t> Такое ребро не найдено!\n");
        return SUCCESS;
    }
    free(name1);
    free(name2);
    free(ports);
    return result;
}

int menu_change_Node(Graph *graph)
{
    char *name;
    long long int port;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name, "\t> Введите имя компьютера: ") == EOF) return EOF;
    if (GetLLInt(&port, "\t> Введите порт: ", check_no_negative) == EOF) 
    {
        free(name);
        return EOF;
    }
    int result = change_data_Node(graph, name, port);
    free(name);
    if (result == 0)
    {
        printf("\t> Компьютера с таким именем не существует!\n");
        return SUCCESS;
    }
    return SUCCESS;
}

int menu_change_Edge(Graph *graph)
{
    char *name1;
    char *name2;
    long long int ports_len;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name1, "\t> Введите имя компьютера №1: ") == EOF) return EOF;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name2, "\t> Введите имя компьютера №2: ") == EOF)
    {
        free(name1);
        return EOF;
    }
    if (GetLLInt(&ports_len, "\t> Введите кол-во порт: ", check_positive) == EOF)
    {
        free(name1);
        free(name2);
        return EOF;
    }
    unsigned long long int *ports = (unsigned long long int *) malloc(ports_len * sizeof(unsigned long long int));
    if (ports == NULL)
    {
        free(name1);
        free(name2);
        return ERROR_ALLOCATION;
    }
    for (long long int i = 0; i < ports_len; i++) 
    {
        long long int port;
        if (GetLLInt(&port, "\t> Введите порт: ", check_no_negative) == EOF) 
        if (ports == NULL)
        {
            free(name1);
            free(name2);
            free(ports);
            return EOF;
        }
        ports[i] = (unsigned long long int) port;
    }
    long long int delay;
    if (GetLLInt(&delay, "\t> Введите задержку: ", check_no_negative) == EOF)
    {
        free(name1);
        free(name2);
        free(ports);
        return EOF;
    }
    int result = change_data_Edge(graph, name1, name2, ports, ports_len, delay);
    if (result == 0) 
    {
        free(name1);
        free(name2);
        free(ports);
        printf("\t> Ребро не найдено!\n");
        return SUCCESS;
    }
    free(name1);
    free(name2);
    free(ports);
    return result == 1 ? SUCCESS : result;
}

int menu_remove_Node(Graph *graph)
{
    char *name;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name, "\t> Введите имя компьютера: ") == EOF) return EOF;
    int result = remove_Node(graph, name);
    free(name);
    if (result == 0)
    {
        printf("\t> Компьютера с таким именем не существует!\n");
        return SUCCESS;
    }
    return result == 1 ? SUCCESS : result;
}

int menu_remove_Edge(Graph *graph)
{
    char *name1;
    char *name2;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name1, "\t> Введите имя компьютера №1: ") == EOF) return EOF;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name2, "\t> Введите имя компьютера №2: ") == EOF)
    {
        free(name1);
        return EOF;
    }
    int result = remove_Edge(graph, name1, name2);
    if (result == 0) 
    {
        free(name1);
        free(name2);
        printf("\t> Ребро не найдено!\n");
        return SUCCESS;
    }
    free(name1);
    free(name2);
    return result == 1 ? SUCCESS : result;
}

int menu_Graph_to_List(Graph *graph)
{
    Adjacency_List *list;
    int result = Graph_to_List(graph, &list);
    if (result != SUCCESS) 
    {
        printf("\t[ERROR] Запись завершилась ошибкой!\n");
        return result;
    }
    Node_Adjacency_List *node = list->head;
    while (node != NULL)
    {
        printf("%s) ", node->node->name);
        Node_List *edge = node->adjacency;
        while (edge != NULL)
        {
            printf("%s ", edge->node->name);
            edge = edge->next;
        }
        printf("\n");
        node = node->next;
    }
    free_List(list);
    return result;
}

int menu_bypass(Graph *graph)
{
    char *name;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name, "\t> Введите имя компьютера: ") == EOF) return EOF;
    long long int port;
    if (GetLLInt(&port, "\t> Введите порт: ", check_no_negative) == EOF)
    {
            free(name);
            return EOF;
        }
    Adjacency_matrix *matrix;
    unsigned long long int u_port = port;
    if (Graph_to_Matrix(graph, &matrix, &u_port) == ERROR_ALLOCATION)
    {
        free(name);
        return ERROR_ALLOCATION;
    }
    bool *visited;
    int result = bypass_Graph(graph, matrix, name, &u_port, &visited);
    if (result == 1)
    {
        printf("\t> Такого компьютера не существует!\n");
        free(name);
        free_Matrix(matrix);
        free(visited);
        return SUCCESS;
    }
    free(name);
    free_Matrix(matrix);
    free(visited);
    return result;
}

int menu_find_shortcut(Graph *graph)
{
    char *name1;
    char *name2;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name1, "\t> Введите имя компьютера №1: ") == EOF) return EOF;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&name2, "\t> Введите имя компьютера №2: ") == EOF) return EOF;
    Int_Queue *queue;
    int len;
    int result = find_shortcut(graph, name1, name2, &queue, &len);
    free(name1);
    free(name2);
    if (len == -1)
    {
        free_Queue(queue);
        printf("\t> Пути не существует\n");
        return result;
    }
    if (result == -3)
    {
        printf("\t> Компьютеры с такими именами не найдены\n");
        return SUCCESS;
    }
    if (result == -2) return ERROR_ALLOCATION;
    Node_Int_Queue *node = queue->head;
    while (node != NULL)
    {
        Node_List_Node_Graph *node_g = graph->nodes;;
        while (node_g != NULL)
        {
            if (node_g->node->index == node->data)
            {
                printf("%s ", node_g->node->name);
                break;
            }
            node_g = node_g->next;
        }
        node = node->next;
    }
    free_Queue(queue);
    printf("\n");
    printf("len: %d\n", len);
    return result;
}

int menu_splitting(Graph *graph)
{
    int result = splitting_Graph(graph);
    return result;
}

int export_menu(Graph *graph)
{
    char *path;
    char symbol;
    do 
    {
        symbol = fgetc(stdin);
        if (symbol == EOF) return EOF;
    } while (symbol != '\n');
    if (input(&path, "\t> Введите путь до файла: ") == EOF) return EOF;
    int result = export_graph(graph, path);
    free(path);
    if (result != SUCCESS) printf("\t[ERROR] Запись завершилась ошибкой!\n");
    else printf("\t> Запись завершилась успешно.\n");
    return result;
}

bool check_option(const long long int option)
{
    return option >= 0 && option <= 11;
}

bool check_result_menu(const int result)
{
    return result == SUCCESS;
}

void print_menu()
{
    printf("1. Добавить вершину\n");
    printf("2. Добавить ребро\n");
    printf("3. Изменить вершину\n");
    printf("4. Изменить ребро\n");
    printf("5. Удалить вершину\n");
    printf("6. Удалить ребро\n");
    printf("7. Вывесть список смежности\n");
    printf("8. Произвести поиск в ширину\n");
    printf("9. Найти кратчайший путь\n");
    printf("10. Разделить граф на компоненты связности\n");
    printf("11. Экспорт графа для Graphviz\n");
    printf("0. Выход\n");
}

int main_menu(Graph *graph)
{
    long long int option;
    int (*function[]) (Graph *graph) = {menu_add_Node, menu_add_Edge, menu_change_Node, menu_change_Edge, 
                                        menu_remove_Node, menu_remove_Edge, menu_Graph_to_List, menu_bypass,
                                        menu_find_shortcut, menu_splitting, export_menu};
    do 
    {
        print_menu();
        if (GetLLInt(&option, "Введите пункт меню: ", check_option) == EOF) return EOF; 
        if (option == 0) break;
        int result = function[option - 1](graph);
        if (!check_result_menu(result)) return result;
    } while (option != 0);
    return SUCCESS;
}