#pragma once
#include <iostream>

class union_find{
private:
    std::vector<int> parent_array;
    std::vector<int> height;
    int connected_components;
public:
    union_find(int n);
    int find(int node);
    bool are_in_one_group(int node1,int node2);
    bool union_by_height(int node1, int node2);
    int connected_components_count() const;
};
