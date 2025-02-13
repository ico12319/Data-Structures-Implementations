#include "unordered_set.hpp"

int main(int argc, const char * argv[]) {
    unordered_set<int> set;
    set.insert(1);
    set.insert(12);
    set.insert(3);
    
    
    set.erase(12);
    set.erase(3);
    set.print();
}
