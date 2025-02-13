#include "map.hpp"

int main(int argc, const char * argv[]) {
    map<int,int> map;
    map.insert(1,1);
    map.insert(0, 5);
    map.insert(12, 5);
    map.insert(8, 6);
    
    
    auto it = map.c_begin();
    it++;
    std::cout<<(*it).first<<std::endl;
    
    map.print();
}
