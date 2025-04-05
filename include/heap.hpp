#ifndef KHEAP_H
#define KHEAP_H

#include <vector>
#include <limits>
#include <math.h>

struct HeapNode {
    int vertex;
    int capacity;
    bool operator>(const HeapNode& other) const {
        return capacity > other.capacity;  
    }
};

class KHeap {
public:
    KHeap(int total_vertices, int new_k);

    int get_vertex_dist(int vertex);
    HeapNode deletemax();
    void update(int vertex, int capacity);
    void insert(int vertex, int capacity);
    int get_size();
private:
    int k;
    std::vector<HeapNode> heap;
    std::vector<int> pos;

    void swap_nodes(int index, int index2);
    int heapify_up(int index);
    int heapify_down(int index);
    double logk(double n);
};

#endif // KHEAP_H