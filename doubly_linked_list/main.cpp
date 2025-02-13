#include "doubly_linked_list.hpp"

int main(int argc, const char * argv[]) {
    doubly_linked_list<int> list;
    
    list.push_back(12);
    list.push_front(33);
    list.push_back(44);
    list.push_front(72);
    
    auto it = list.c_begin();
    it++;
    list.insert(88, it);
    list.print();
}
