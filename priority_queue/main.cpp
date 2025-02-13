#include "priority_queue.hpp"

int main(int argc, const char * argv[]) {
    std::vector<int> v = {9,1,0,4,12,55,101};
    priority_queue<int> pq(v);
    
   
    pq.pop();
    pq.pop();
    std::cout<<pq.top()<<std::endl;
}
