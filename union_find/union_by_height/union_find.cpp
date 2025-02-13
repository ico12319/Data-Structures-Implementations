#include <stdio.h>
#include "union_by_height.h"

union_find::union_find(int n){
    parent_array.resize(n);
    height.resize(n,0);
    for(int i = 0;i<n;i++)
        parent_array[i] = i;
}

int union_find::find(int node){
    if(node == parent_array[node])
        return node;
    return parent_array[node] = find(parent_array[node]);
}

bool union_find::are_in_one_group(int node1, int node2){
    return find(node1) == find(node2);
}

bool union_find::union_by_height(int node1, int node2){
    int ultimate_parent1 = find(node1);
    int ultimate_parent2 = find(node2);
    
    if(ultimate_parent1 == ultimate_parent2)
        return false;
    if(height[ultimate_parent1] < height[ultimate_parent2])
        parent_array[ultimate_parent1] = ultimate_parent2;
    else if(height[ultimate_parent1] > height[ultimate_parent2])
        parent_array[ultimate_parent2] = ultimate_parent1;
    else{
        parent_array[ultimate_parent2] = ultimate_parent1;
        height[ultimate_parent1]++;
    }
    connected_components--;
    return true;
}


int union_find::connected_components_count() const{
    return connected_components;
}
