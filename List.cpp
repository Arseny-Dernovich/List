#include "My_features.h"
#include <stdio.h>
#define LIST_CAPACITY 10

struct Node {
    int data;
    int next;
    int prev;
};

struct LinkedList {
    Node nodes[LIST_CAPACITY];
    int head;
    int tail;
    int free;
    int capacity;
};


void List_Init (LinkedList *list)
{
    list->capacity = LIST_CAPACITY;
    list->head = -1;
    list->tail = -1;
    list->free = 0;

    for (int i = 0; i < LIST_CAPACITY - 1; i++) {
        list->nodes[i].next = i + 1;
    }
    list->nodes[LIST_CAPACITY - 1].next = -1;
}

int Add_To_End (LinkedList *list, int data)
{
    if (list->free == -1)
        return -1;

    int newIdx = list->free;
    list->free = list->nodes[newIdx].next;

    list->nodes[newIdx].data = data;
    list->nodes[newIdx].next = -1;
    list->nodes[newIdx].prev = list->tail;

    if (list->tail != -1) {
        list->nodes[list->tail].next = newIdx;
    }
    list->tail = newIdx;

    if (list->head == -1) {
        list->head = newIdx;
    }

    return newIdx;
}

int Add_To_Start (LinkedList *list, int data)
{
    if (list->free == -1) return -1;

    int newIdx = list->free;
    list->free = list->nodes[newIdx].next;

    list->nodes[newIdx].data = data;
    list->nodes[newIdx].next = list->head;
    list->nodes[newIdx].prev = -1;

    if (list->head != -1) {
        list->nodes[list->head].prev = newIdx;
    }
    list->head = newIdx;

    if (list->tail == -1) {
        list->tail = newIdx;
    }

    return newIdx;
}

int Insert_After (LinkedList *list, int index, int data)
{
    if (index < 0 || index >= list->capacity || list->free == -1) {
        printf ("Неверный индекс!!!");
        return -1;
    }

    int newIdx = list->free;
    list->free = list->nodes[newIdx].next;

    list->nodes[newIdx].data = data;
    list->nodes[newIdx].next = list->nodes[index].next;
    list->nodes[newIdx].prev = index;

    if (list->nodes[index].next != -1) {
        list->nodes[list->nodes[index].next].prev = newIdx;
    }
    list->nodes[index].next = newIdx;

    if (list->tail == index) {
        list->tail = newIdx;
    }

    return newIdx;
}

int Insert_Before (LinkedList *list, int index, int data)
{
    if (index < 0 || index >= list->capacity || list->free == -1) {
        printf ("Неверный индекс!!!");
        return -1;
    }

    int newIdx = list->free;
    list->free = list->nodes[newIdx].next;

    list->nodes[newIdx].data = data;
    list->nodes[newIdx].prev = list->nodes[index].prev;
    list->nodes[newIdx].next = index;

    if (list->nodes[index].prev != -1) {
        list->nodes[list->nodes[index].prev].next = newIdx;
    }
    list->nodes[index].prev = newIdx;

    if (list->head == index) {
        list->head = newIdx;
    }

    return newIdx;
}

int Remove_Element (LinkedList *list, int index)
{
    if (index < 0 || index >= list->capacity) {
        printf("Error: Invalid index\n");
        return -1;
    }

    if (list->nodes[index].prev == -1 && list->nodes[index].next == -1 && list->head != index) {
        printf("Error: Element already free or does not exist in the list\n");
        return -1;
    }


    if (index == list->head) {

        list->head = list->nodes[index].next;
        if (list->head != -1) {
            list->nodes[list->head].prev = -1;
        } else {
            list->tail = -1;
        }
    }

    else if (index == list->tail) {

        list->tail = list->nodes[index].prev;
        if (list->tail != -1) {
            list->nodes[list->tail].next = -1;
        } else {
            list->head = -1;
        }
    }

    else {

        int prevIdx = list->nodes[index].prev;
        int nextIdx = list->nodes[index].next;

        if (prevIdx != -1) {
            list->nodes[prevIdx].next = nextIdx;
        }
        if (nextIdx != -1) {
            list->nodes[nextIdx].prev = prevIdx;
        }
    }


    list->nodes[index].next = list->free;
    list->nodes[index].prev = -1;
    list->nodes[index].data = 0;
    list->free = index;

    return 0;
}



void Dump_List (struct LinkedList *list, const char *filename)
{
    FILE *file = fopen (filename, "w");
    fprintf (file, "digraph LinkedList {\n");
    fprintf (file, "rankdir=LR;\n");
    fprintf (file, "node [shape=record, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n");


    for (int i = list->head; i != -1; i = list->nodes[i].next) {
        fprintf (file, "node%d [label=\"<index> IP: %d | <data> data: %d | <next> next: %d | <prev> prev: %d\"];\n",
                i, i, list->nodes[i].data, list->nodes[i].next, list->nodes[i].prev);
    }

    for (int i = list->head; i != -1 && list->nodes[i].next != -1; i = list->nodes[i].next) {
        fprintf (file, "node%d:<next> -> node%d:<index> [label=\"next\", color=green, fontcolor=green];\n",
                i, list->nodes[i].next);
    }

    for (int i = list->tail; i != -1 && list->nodes[i].prev != -1; i = list->nodes[i].prev) {
        fprintf (file, "node%d:<prev> -> node%d:<index> [label=\"prev\", color=red, fontcolor=red];\n",
                i, list->nodes[i].prev);
    }


    if (list->head != -1) {
        fprintf (file, "head [shape=plaintext, label=\"head\"];\n");
        fprintf (file, "head -> node%d:<index> [color=blue, style=dashed];\n", list->head);
    }

    if (list->tail != -1) {
        fprintf (file, "tail [shape=plaintext, label=\"tail\"];\n");
        fprintf (file, "tail -> node%d:<index> [color=blue, style=dashed];\n", list->tail);
    }

    fprintf (file, "}\n");
    fclose (file);
}



int main()
{
    LinkedList list = {};
    List_Init (&list);

    Add_To_Start (&list, 10);
    Add_To_End (&list, 20);
    Add_To_End (&list, 30);
    Add_To_End (&list, 40);
    Add_To_End (&list, 50);
    Add_To_End (&list, 60);
    Insert_After (&list , 0 , 25);
    // Add_To_Start (&list, 70);
    // Add_To_Start (&list, 80);
    // Add_To_End (&list, 90);
    // Add_To_Start (&list, 100);

//     printf ("[");
//     for (int i = 0 ; i < LIST_CAPACITY ; i++)
//         printf ("%d ,\t" , list.nodes[i].data);
//     printf ("\n");
//     printf ("[");
//     for (int i = 0 ; i < LIST_CAPACITY ; i++)
//         printf ("%d ,\t" , list.nodes[i].next);
//
//     printf ("head = %d" , list.head);

    Dump_List (&list, "list.dot");

    system ("dot -Tpng list.dot -o list.png");

    return 0;
}
