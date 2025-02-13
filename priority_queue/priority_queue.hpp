#pragma once
#include <iostream>

template<class value,class comparator = std::less<value>>
class priority_queue{
private:
    std::vector<int> heap;
    comparator cmp;
    
    static size_t get_left_child_index(size_t parent_index);
    static size_t get_right_child_index(size_t parent_index);
    static size_t get_parent_index(size_t child_index);
    
    void heapify(size_t index);
public:
    priority_queue() = default;
    priority_queue(std::vector<int>& v);
    priority_queue(const comparator& cmp);
    
    void push(const value& val);
    void pop();
    const value& top() const;
    bool is_empty() const;
    size_t size() const;
};

template<class value,class comparator>
size_t priority_queue<value,comparator>::get_left_child_index(size_t parent_index){
    return 2 * parent_index + 1;
}

template<class value,class comparator>
size_t priority_queue<value, comparator>::get_right_child_index(size_t parent_index){
    return 2 * parent_index + 2;
}

template<class value,class comparator>
size_t priority_queue<value,comparator>::get_parent_index(size_t child_index){
    return (child_index - 1) / 2;
}

template<class value,class comparator>
void priority_queue<value,comparator>::push(const value& val){
    heap.push_back(val);
    int element_index = heap.size() - 1;
    int parent_index = get_parent_index(element_index);
    while(element_index > 0 && cmp(heap[parent_index],heap[element_index])){
        std::swap(heap[element_index],heap[parent_index]);
        element_index = parent_index;
        parent_index = get_parent_index(element_index);
    }
}

template<class value,class comparator>
void priority_queue<value,comparator>::pop(){
    if(heap.empty())
        throw std::invalid_argument("Empty heap!");
    std::swap(heap[0],heap[heap.size() - 1]);
    heap.pop_back();
    heapify(0);
}

template<class value,class comparator>
const value& priority_queue<value,comparator>::top() const{
    if(heap.empty())
        throw std::invalid_argument("Empty heap!");
    return heap[0];
}

template<class value,class comparator>
void priority_queue<value,comparator>::heapify(size_t index){
    while(true){
        size_t left_child_index = get_left_child_index(index);
        size_t right_child_index = get_right_child_index(index);
        
        bool go_left = cmp(heap[index],heap[left_child_index]) && left_child_index < heap.size();
        bool go_right = cmp(heap[index],heap[right_child_index]) && right_child_index < heap.size();
        if(go_left && go_right){
            if(heap[left_child_index] > heap[right_child_index]){
                std::swap(heap[index],heap[left_child_index]);
                index = left_child_index;
            }
            else{
                std::swap(heap[index],heap[right_child_index]);
                index = right_child_index;
            }
        }
        else if(go_left){
            std::swap(heap[index],heap[left_child_index]);
            index = left_child_index;
        }
        else if(go_right){
            std::swap(heap[index],heap[right_child_index]);
            index = right_child_index;
        }
        else
            break;
     }
}

template<class value,class comparator>
bool priority_queue<value,comparator>::is_empty() const{
    return heap.empty();
}

template<class value,class comparator>
size_t priority_queue<value,comparator>::size() const{
    return heap.size();
}

template<class value,class comparator>
priority_queue<value,comparator>::priority_queue(std::vector<int>& v){
    heap = v;
    for(int i = heap.size() / 2 - 1;i>=0;i--)
        heapify(i);
}

template<class value,class comparator>
priority_queue<value,comparator>::priority_queue(const comparator& cmp) : cmp(cmp){}
