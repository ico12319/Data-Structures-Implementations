#include "union_find.h"

int main(int argc, const char * argv[]) {
    union_find uf(5);
    uf.union_by_size(0, 1);
    uf.union_by_size(1, 2);
    
    std::cout<<uf.connected_components_count()<<std::endl;
}
