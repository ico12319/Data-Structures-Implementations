#include "queue.hpp"

int main(int argc, const char * argv[]) {
    queue<int> q;

    
    for(int i = 0;i<50;i++)
        q.push(i);
    
    q.pop();
    q.pop();
    while (!q.empty()) {
        std::cout<<q.front()<<std::endl;
        q.pop();
    }
}
