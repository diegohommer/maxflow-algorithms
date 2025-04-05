#include "heap.hpp"
#include <iostream>

KHeap::KHeap(int total_vertices, int new_k) : pos(total_vertices, -1), k(new_k) {}

int KHeap::get_vertex_cap(int vertex){
    if (pos[vertex] == -1) { 
        return std::numeric_limits<int>::max(); 
    }
    return heap[pos[vertex]].capacity;
}

HeapNode KHeap::deletemax(){
    HeapNode max = heap.front();
    swap_nodes(0, heap.size() - 1);
    pos[max.vertex] = -1;
    heap.pop_back();
    if(heap.size() > 1)
        heapify_down(0);

    return max;
}

void KHeap::update(int vertex, int capacity, Edge* pred){
    int vertex_index = pos[vertex];
    heap[vertex_index] = HeapNode{vertex, capacity, pred};
    int sift_up_count = heapify_up(vertex_index);
}

void KHeap::insert(int vertex, int capacity, Edge* pred){
    heap.push_back(HeapNode{vertex, capacity, pred});
    pos[vertex] = heap.size() - 1;
}

int KHeap::get_size(){
    return heap.size();
}

void KHeap::swap_nodes(int index, int index2){
    int v1 = heap[index].vertex;
    int v2 = heap[index2].vertex;

    std::swap(heap[index], heap[index2]);   // Swap head capacities
    std::swap(pos[v1], pos[v2]);            // Swap hash indexes
}

int KHeap::heapify_up(int index) {
    int sift_counter = 0;
    while(index > 0){
        int parent = (index - 1) / k;

        if(heap[index] > heap[parent]){
            swap_nodes(index,parent);
            index = parent;
        }else{
            break;
        }
    }
    return sift_counter;
}

int KHeap::heapify_down(int index) {
    int sift_counter = 0;
    while (index < heap.size()){
        int min = index;

        for (int i = 1; i <= k; i++) {
            int child = (k * index) + i;
            if (child < heap.size() && heap[child] > heap[min]) {
                min = child;
            }
        }

        if(min != index){
            swap_nodes(index,min);
            index = min;
        }else{
            break;
        }
    }
    return sift_counter;
}
