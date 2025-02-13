#pragma once
#include <iostream>

namespace CONSTANTS {
constexpr size_t INITIAL_CAPACITY = 8;
constexpr size_t GROWTH_FACTOR = 2;
}

template<class value,class allocator = std::allocator<value>>
class queue{
private:
    value* data;
    size_t size;
    size_t head;
    size_t tail;
    size_t capacity;
    allocator allocat;
    
    void move(queue&& other);
    void copy(const queue& other);
    void destroy();
    void resize(size_t new_capacity);
    
    void move_index(size_t& index,bool forward) const;

public:
    queue();
    queue(const queue& other);
    queue(queue&& other);
    queue& operator=(const queue& other);
    queue& operator=(queue&& other);
    ~queue();
    
    void push(const value& val);
    void push(value&& val);
    
    void pop();
    
    const value& front() const;
    value& front();
    
    size_t get_size() const;
    bool empty() const;
};

template<class value,class allocator>
queue<value,allocator>::queue() : size(0),head(0),tail(0),capacity(0){
    data = allocat.allocate(CONSTANTS::INITIAL_CAPACITY);
}

template<class value,class allocator>
void queue<value,allocator>::move_index(size_t& index,bool forward) const{
    if(forward)
        ++(index) %= capacity;
    else
        index == 0 ? capacity - 1 : index - 1;
}

template<class value,class allocator>
void queue<value,allocator>::push(const value& val){
    if(size == capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    std::allocator_traits<allocator>::construct(allocat,&data[tail],val);
    move_index(tail,true);
    size++;
}

template<class value,class allocator>
void queue<value,allocator>::push(value&& val){
    if(size == capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    std::allocator_traits<allocator>::construct(allocat,&data[tail],std::move(val));
    move_index(tail,true);
    size++;
}

template<class value,class allocator>
void queue<value,allocator>::pop(){
    if(empty())
        throw std::invalid_argument("Empty");
    std::allocator_traits<allocator>::destroy(allocat,&data[head]);
    move_index(head,true);
    size--;
}


template<class value,class allocator>
const value& queue<value,allocator>::front() const{
    return data[head];
}

template<class value,class allocator>
value& queue<value,allocator>::front(){
    return data[head];
}

template<class value,class allocator>
void queue<value,allocator>::destroy(){
    if(head == tail && !empty()){
        std::allocator_traits<allocator>::destroy(allocat,&data[head]);
        move_index(head, true);
    }
    
    while(head != tail){
        std::allocator_traits<allocator>::destroy(allocat,&data[head]);
        move_index(head, true);
    }
    allocat.deallocate(data,capacity);
    head = tail = capacity = size = 0;
    data = nullptr;
}

template<class value,class allocator>
queue<value,allocator>::~queue(){
    destroy();
}

template<class value,class allocator>
void queue<value,allocator>::copy(const queue& other){
    data = allocat.allocate(other.capacity);
    for(int i = other.head;i!=other.tail;(i+=1) %= other.capacity)
        std::allocator_traits<allocator>::construct(allocat,&data[i],other.data[i]);
    
    size = other.size;
    capacity = other.capacity;
    head = other.head;
    tail = other.tail;
}

template<class value,class allocator>
void queue<value,allocator>::move(queue&& other){
    data = other.data;
    size = other.size;
    capacity = other.capacity;
    head = other.head;
    tail = other.tail;
    
    other.data = nullptr;
    other.size = other.tail = other.capacity = other.head = 0;
}

template<class value,class allocator>
queue<value,allocator>::queue(const queue& other){
    copy(other);
}

template<class value,class allocator>
queue<value, allocator>::queue(queue&& other){
    move(std::move(other));
}

template<class value,class allocator>
queue<value,allocator>& queue<value,allocator>::operator=(const queue& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value,class allocator>
queue<value,allocator>& queue<value,allocator>::operator=(queue&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value,class allocator>
void queue<value, allocator>::resize(size_t new_capacity){
    value* new_data = allocat.allocate(new_capacity);
    for(int i = 0;i<size;i++){
        auto front = data[(head + i) % capacity];
        std::allocator_traits<allocator>::construct(allocat,&new_data[i],front);
        std::allocator_traits<allocator>::destroy(allocat,&data[(head + i) % capacity]);
    }
    allocat.deallocate(data,capacity);
    data = new_data;
    capacity = new_capacity;
}

template<class value,class allocator>
bool queue<value,allocator>::empty() const{
    return size == 0;
}

template<class value,class allocator>
size_t queue<value,allocator>::get_size() const{
    return size;
}
