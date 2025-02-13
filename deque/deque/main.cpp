#include <iostream>
#include "deque.hpp"
#include <deque>


int main(int argc, const char * argv[]) {
    deque<int> deq;
    
    deq.push_front(84);
    deq.push_back(12);
    deq.push_back(24);
    deq.push_back(102);
    
   
    deq.pop_back();
    deq.pop_back();
    deq.pop_front();
    for(const auto& el : deq)
        std::cout<<el<<" ";

    
}
