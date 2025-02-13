#include "vector.hpp"

int main(int argc, const char * argv[]) {
    
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    
    auto it = v.c_begin();
    it++;
    auto it2 = v.c_end();
    it2--;
    it2--;
    v.erase(it, it2);
    v.print();
    
    
}
