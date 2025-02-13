#include "stack.hpp"

int main(int argc, const char * argv[]) {
    stack<int> st;
    for(int i = 0;i<50;i++)
        st.push(i);
    st.pop();
    st.pop();
    while(!st.empty()){
        std::cout<<st.top()<<" ";
        st.pop();
    }
}
