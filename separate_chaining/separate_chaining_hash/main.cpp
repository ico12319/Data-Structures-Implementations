#include <list>
#include <iostream>
#include "unordered_set.hpp"


int main(int argc, const char * argv[]) {
    unordered_set<int> set;
    set.insert(12);
    set.insert(54);
    set.insert(1);
    set.insert(88);
    set.insert(11);
    set.insert(12);
    
    set.erase(12);
    set.print();
}
