#include "binary_search_tree.hpp"


int main(){
    binary_search_tree<int> bst;
    bst.insert(1);
    bst.insert(0);
    bst.insert(100);
    bst.insert(12);
   
    auto it = bst.c_begin();
    it++;
    std::cout<<*it<<std::endl;
}
