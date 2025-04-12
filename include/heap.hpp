#ifndef KHEAP_H
#define KHEAP_H

#include <graph.hpp>
#include <vector>
#include <limits>
#include <math.h>

struct HeapNode {
    int vertex;
    int capacity;
    Edge* incoming_edge;
    bool operator>(const HeapNode& other) const {
        return capacity > other.capacity;  
    }
};

class KHeap {
public:
    KHeap(int total_vertices, int new_k);

    int get_vertex_cap(int vertex);
    HeapNode deletemax();
    void update(int vertex, int capacity, Edge* incoming_edge);
    void insert(int vertex, int capacity, Edge* incoming_edge);
    int get_size();
private:
    int k;
    std::vector<HeapNode> heap;
    std::vector<int> pos;

    void swap_nodes(int index, int index2);
    void heapify_up(int index);
    void heapify_down(int index);
};

#endif // KHEAP_H