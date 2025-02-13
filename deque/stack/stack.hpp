#pragma once
#include <iostream>

template<class value,class allocator = std::allocator<value>>
class stack{
private:
    value* data;
    size_t size;
    allocator allocat;
    size_t capacity;
    
    void copy(const stack& other);
    void move(stack&& other);
    void destroy();
    void resize(size_t new_capacity);
    
public:
    stack();
    stack(const stack& other);
    stack(stack&& other);
    stack& operator=(const stack& other);
    stack& operator=(stack&& other);
    ~stack();
    
    void push(const value& val);
    void push(value&& val);
    
    void pop();
    
    const value& top() const;
    value& top();
    
    bool empty() const;
    size_t get_size() const;
};

template<class value,class allocator>
stack<value,allocator>::stack() : capacity(8),size(0){
    data = allocat.allocate(capacity);
}

template<class value,class allocator>
stack<value,allocator>::stack(const stack& other){
    copy(other);
}

template<class value,class allocator>
stack<value,allocator>::stack(stack&& other){
    move(std::move(other));
}

template<class value,class allocator>
stack<value,allocator>::~stack(){
    destroy();
}

template<class value,class allocator>
stack<value,allocator>& stack<value,allocator>::operator=(const stack& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}


template<class value,class allocator>
stack<value,allocator>& stack<value,allocator>::operator=(stack&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value,class allocator>
void stack<value,allocator>::push(const value& val){
    if(size == capacity)
        resize(capacity * 2);
    std::allocator_traits<allocator>::construct(allocat,&data[size],val);
    size++;
    
}

template<class value,class allocator>
void stack<value,allocator>::push(value&& val){
    if(size == capacity)
        resize(capacity * 2);
    std::allocator_traits<allocator>::construct(allocat,&data[size],std::move(val));
    size++;
}

template<class value,class allocator>
void stack<value,allocator>::pop(){
    if(empty())
        throw std::invalid_argument("Empty!");
    std::allocator_traits<allocator>::destroy(allocat,&data[size - 1]);
    size--;
}


template<class value,class allocator>
const value& stack<value,allocator>::top() const{
    if(empty())
        throw std::invalid_argument("Empty!");
    return data[size - 1];
}

template<class value,class allocator>
value& stack<value,allocator>::top(){
    if(empty())
        throw std::invalid_argument("Empty!");
    return data[size - 1];
}

template<class value,class allocator>
bool stack<value,allocator>::empty() const{
    return size == 0;
}

template<class value,class allocator>
size_t stack<value,allocator>::get_size() const{
    return size;
}


template<class value,class allocator>
void stack<value,allocator>::copy(const stack& other){
    data = allocat.allocate(other.capacity);
    for(int i = 0;i<other.size;i++)
        std::allocator_traits<allocator>::construct(allocat,&data[i],other.data[i]);
    size = other.size;
    capacity = other.capacity;
}


template<class value,class allocator>
void stack<value,allocator>::move(stack&& other){
    data = other.data;
    size = other.size;
    capacity = other.capacity;
    
    other.data = nullptr;
    other.size = other.capacity = 0;
}

template<class value,class allocator>
void stack<value,allocator>::destroy(){
    for(int i = 0;i<size;i++)
        std::allocator_traits<allocator>::destroy(allocat,&data[i]);
    allocat.deallocate(data,capacity);
}

template<class value,class allocator>
void stack<value,allocator>::resize(size_t new_capacity){
    value* new_data = allocat.allocate(new_capacity);
    for(int i = 0;i<size;i++){
        std::allocator_traits<allocator>::construct(allocat,&new_data[i],std::move(data[i]));
        std::allocator_traits<allocator>::destroy(allocat,&data[i]);
    }
    allocat.deallocate(data,capacity);
    data = new_data;
    capacity = new_capacity;
}


