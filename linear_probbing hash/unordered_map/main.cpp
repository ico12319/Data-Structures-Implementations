#include "unordered_map.hpp"

int main(int argc, const char * argv[]) {
    unordered_map<int,int> map;
    map.insert(1, 1);
    map.insert(2, 4);
    map.insert(12, 5);
    
    map.insert(54, 1);
    map.insert(1, 2);
    
    map.erase(12);
    map.print();
}
