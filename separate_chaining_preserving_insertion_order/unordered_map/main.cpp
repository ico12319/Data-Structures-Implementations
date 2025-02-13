#include "unordered_map.hpp"

int main(int argc, const char * argv[]) {
    unordered_map<int, int> map;
    map.insert(1, 1);
    map.insert(12,3);
    map.insert(4, 5);
        
    
    map.print();
}
