#include "linked_deque.h"


int main(int argc, const char * argv[]) {
    deque<int> d;
    d.push_back(12);
    d.push_back(24);
    d.push_front(108);
    d.push_front(33);
    d.push_front(42);
    
    d.pop_back();
    d.pop_back();
    d.print();
    
}
