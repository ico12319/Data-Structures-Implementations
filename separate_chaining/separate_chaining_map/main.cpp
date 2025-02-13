#include "unordered_map.hpp"

int main(int argc, const char * argv[]) {
    unordered_map<int, int> map;
    map.insert(1, 1);
    map.insert(2,6);
    
    map.insert(55,32);
    map.insert(12, 65);
    map.insert(33, 4);
    map.insert(1, 5);
    
    map.erase(55);
    map.erase(1);
    map.erase(33);
    map.print();
}
