#include "union_by_height.h"

int main(int argc, const char * argv[]) {
    union_find uf(4);
    uf.union_by_height(0, 1);
    uf.union_by_height(1, 2);
    
    std::cout<<uf.are_in_one_group(0, 3)<<std::endl;
}
