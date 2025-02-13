#pragma once
#include <iostream>

class union_find{
private:
    std::vector<int> parent_array;
    std::vector<int> sizes;
    int connected_components;
    
public:
    union_find(int n);
    bool are_in_one_group(int node1,int node2);
    int find(int node);
    bool union_by_size(int node1,int node2);
    int connected_components_count() const;
};
