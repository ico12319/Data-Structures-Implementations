#include "singly_linked_list.hpp"

int main(int argc, const char * argv[]) {
    singly_linked_list<int> list;
    
    list.push_back(12);
    list.push_front(54);
    list.push_back(105);
    list.push_front(55);
    
    auto it = list.begin();
    list.insert_after(33, it);
    it++;
    list.remove_after(it);
    list.print();

}
