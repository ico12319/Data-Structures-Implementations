#include "set.hpp"


int main(int argc, const char * argv[]) {
    set<int> set;
    set.insert(18);
    set.insert(0);
    set.insert(88);
    set.insert(3);
    
    
    auto it = set.begin();
    it++;
    it++;
    std::cout<<*it<<std::endl;
}
