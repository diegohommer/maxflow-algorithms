#ifndef KHEAP_H
#define KHEAP_H

#include <graph.hpp>
#include <vector>
#include <limits>
#include <math.h>

// Struct representing a node in the heap, containing the vertex, its associated capacity, and the incoming edge
struct HeapNode {
    int vertex;             // The vertex associated with this heap node
    int capacity;           // The capacity (used as the key for the heap ordering)
    Edge* incoming_edge;    // The incoming edge associated with the vertex (used in flow algorithms)

    // Comparison operator to compare HeapNodes based on their capacity (for max-heap)
    bool operator>(const HeapNode& other) const {
        return capacity > other.capacity;
    }
};

// Class implementing a k-ary heap data structure, where the heap stores HeapNodes (vertex, capacity, incoming_edge)
// It provides operations such as insert, delete max, and updates on the heap structure.
class KHeap {
public:
    // Constructor that initializes the heap with the specified number of vertices and the k value for the k-ary heap
    KHeap(int total_vertices, int new_k);

    // Retrieves the capacity of a given vertex in the heap
    int get_vertex_cap(int vertex);

    // Removes and returns the node with the maximum capacity (heap's root element)
    HeapNode deletemax();

    // Updates the capacity of a given vertex, potentially changing its position in the heap
    void update(int vertex, int capacity, Edge* incoming_edge);

    // Inserts a new node (vertex, capacity, incoming_edge) into the heap
    void insert(int vertex, int capacity, Edge* incoming_edge);

    // Returns the current size (number of nodes) of the heap
    int get_size();

private:
    int k;                              // The branching factor of the k-ary heap (number of children per node)
    std::vector<HeapNode> heap;          // The vector storing the heap's nodes (HeapNode list)
    std::vector<int> pos;                // The vector storing the position of each vertex in the heap

    // Helper function to swap two nodes in the heap
    void swap_nodes(int index, int index2);

    // Helper function to maintain the heap property by moving a node up the heap
    void heapify_up(int index);

    // Helper function to maintain the heap property by moving a node down the heap
    void heapify_down(int index);
};

#endif // KHEAP_H
