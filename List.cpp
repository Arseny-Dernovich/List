#include "My_features.h"
#include <stdio.h>
const int LIST_CAPACITY = 10;

struct Node {
    int data;
    int next;
    int prev;
    int removed_value;
};

struct LinkedList {
    Node nodes[LIST_CAPACITY];
    int head;
    int tail;
    int free;
    int capacity;
};

enum List {
    BAD_HEAD ,
    BAD_TAIL ,
    BAD_FREE ,
    BAD_LINKAGE
};

int Insert_After (LinkedList *list, int index, int data);
int Insert_Before (LinkedList *list, int index, int data);

void List_Init (LinkedList *list)
{
    for (int i = 0; i < LIST_CAPACITY - 1; i++) {
        list->nodes[i].next = i + 1;
    }

    list->nodes[LIST_CAPACITY - 1].next = -1;

    list->free = 1;


    list->capacity = LIST_CAPACITY;
    list->head = 0;
    list->nodes[list->head].next = -1;
    list->nodes[list->head].prev = -1;

    list->tail = list->head;
}

int Add_To_End (LinkedList *list, int data)
{
      if (list->free == -1)
        return -1;

    return Insert_After (list, list->tail, data);
}

int Add_To_Start (LinkedList *list, int data)
{
      if (list->free == -1)
        return -1;

    return Insert_After (list, list->head, data);
}


int List_Verify (LinkedList *list)
{
    if (list->tail >= list->capacity) {
        printf ("Error: Invalid tail index\n");
        return BAD_TAIL;
    }

    int current = list->nodes[list->head].next;
    while (current != -1) {
        int next = list->nodes[current].next;
        if (next != -1 && list->nodes[next].prev != current) {
            printf ("Error: Bad linkage at index %d\n", current);
            return BAD_LINKAGE;
        }
        current = next;
    }

    int freeIndex = list->free;
    while (freeIndex != -1) {
        if (freeIndex >= list->capacity) {
            printf ("Error: Invalid free index %d\n", freeIndex);
            return BAD_FREE;
        }
        freeIndex = list->nodes[freeIndex].next;
    }

    return complete_value;
}

int Insert_After (LinkedList *list, int index, int data)
{
    if (index < 0 || index >= list->capacity || list->free == -1) {
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
    if (index <= 0 || index >= list->capacity || list->free == -1) {
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

    if (list->nodes[list->head].next == index) {
        list->nodes[list->head].next = newIdx;
    }

    return newIdx;
}

int Remove_Element (LinkedList *list, int index)
{
    if (index <= 0 || index >= list->capacity) {
        return -1;
    }

    list->nodes[index].removed_value = list->nodes[index].data;

    int prevIdx = list->nodes[index].prev;
    int nextIdx = list->nodes[index].next;

    if (prevIdx != -1) {
        list->nodes[prevIdx].next = nextIdx;
    }
    if (nextIdx != -1) {
        list->nodes[nextIdx].prev = prevIdx;
    }

    if (list->tail == index) {
        list->tail = prevIdx;
    }

    list->nodes[index].next = list->free;
    list->nodes[index].prev = -1;
    list->nodes[index].data = 0;
    list->free = index;

    return 0;
}


void Dump_List(LinkedList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "digraph LinkedList {\n");
    fprintf(file, "rankdir=LR; // Горизонтальная ориентация графа\n");
    fprintf(file, "bgcolor = \"#FFFFFF\";\n");
    fprintf(file, "nodesep=0.5;\n");
    fprintf(file, "node [shape=record, style=filled, fillcolor=lightblue fontname=\"Arial\"];\n");

    fprintf(file, "subgraph cluster_nodes {\n");
    fprintf(file, "rank=same;\n");
    fprintf (file , "color = \"white\"");

    for (int i = 0; i < list->capacity; i++) {
        const char *nodeColor = "lightblue";
        int value = list->nodes[i].data;
        if (list->nodes[i].data == 0 && list->nodes[i].prev == -1) {
            nodeColor = "yellow";
            value = list->nodes[i].removed_value;
        } else if (list->nodes[i].data == 0 && list->nodes[i].prev == 0) {
            nodeColor = "gray";
        }

        fprintf(file, "NODE_%d [label=\"IP %d | value = %d | next = %d | prev = %d\", fillcolor=%s];\n",
                i, i, value, list->nodes[i].next, list->nodes[i].prev, nodeColor);

        if (i == list->head) {
            fprintf(file, "NODE_%d [fillcolor=lightgreen]; // head\n", i);
        } else if (i == list->tail) {
            fprintf(file, "NODE_%d [fillcolor=lightcoral]; // tail\n", i);
        }
    }

    fprintf(file, "}\n");

    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(file, "NODE_%d -> NODE_%d [penwidth=2, color=\"#FFFFFF\" , weight = 20];\n", i, i + 1);
    }

    for (int i = 0; i < list->capacity; i++) {

        if (list->nodes[i].data == 0 && list->nodes[i].prev == 0) continue;
        if (list->nodes[i].next != -1) {
            fprintf(file, "NODE_%d -> NODE_%d [label=\"next\", color=green, weight=1];\n", i, list->nodes[i].next);
        }

        if (list->nodes[i].prev != -1) {
            fprintf(file, "NODE_%d -> NODE_%d [label=\"prev\", color=red, weight=1];\n", i, list->nodes[i].prev);
        }
    }

    if (list->head != -1) {
        fprintf(file, "head [shape=plaintext, label=\"head\" , color = pink];\n");
        fprintf(file, "head -> NODE_%d [color=blue, style=dashed];\n", list->head);
    }

    if (list->tail != -1) {
        fprintf(file, "tail [shape=plaintext, label=\"tail\", color = pink];\n", list->tail);
        fprintf(file, "tail -> NODE_%d [color=blue, style=dashed];\n", list->tail);
    }

    int freeIndex = list->free;
    while (freeIndex != -1) {
        fprintf(file, "free [shape=plaintext, label=\"free\", color=pink];\n");
        fprintf(file, "free -> NODE_%d [color=green, style=dashed];\n", freeIndex);
        freeIndex = list->nodes[freeIndex].next;
    }

    fprintf(file, "}\n");
    fclose(file);
}




int main ()
{
    LinkedList list = {};
    List_Init (&list);

    Add_To_Start (&list, 10);
    Add_To_End (&list, 20);
    Add_To_End (&list, 30);
    Add_To_End (&list, 40);
    Add_To_End (&list, 50);
    Add_To_End (&list, 60);
    Insert_After (&list , 2 , 25);
    Remove_Element (&list , 3);
    List_Verify (&list);

    // Add_To_Start (&list, 70);
    // Add_To_Start (&list, 80);
    // Add_To_End (&list, 90);
    // Add_To_Start (&list, 100);

//     printf ("[");
//     for (int i = 0 ; i < LIST_CAPACITY ; i++)
//         printf ("%d ,\t" , list.nodes[i].data);
//     printf ("\n");
    // printf ("[");
    // for (int i = 0 ; i < LIST_CAPACITY ; i++)
    //     printf ("%d ,\t" , list.nodes[i].next);
//
//     printf ("head = %d" , list.head);


    Dump_List (&list, "list.dot");
    system ("dot -Tpng list.dot -o list.png");

    return 0;
}


