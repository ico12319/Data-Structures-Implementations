#include "unordered_set.hpp"

int main(int argc, const char * argv[]) {
    unordered_set<int> set;

    for(int i = 0;i<50;i++)
        set.insert(i);

    
    
    set.erase(3);
    auto it = set.c_begin();
    it++;
    it++;
    std::cout<<*it<<std::endl;
    set.print();
    
    
    
    
}
