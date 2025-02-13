#pragma once
#include <iostream>

template<class value>
class queue{
private:
    struct node{
        value val;
        node* next;
        
        node(const value& val) : val(val),next(nullptr){}
    };
    
    node* head = nullptr;
    node* tail = nullptr;
    size_t size = 0;
    
    void copy(const queue& other);
    void move(queue&& other);
    void destroy();
    
public:
    queue() = default;
    queue(const queue& other);
    queue(queue&& other);
    ~queue();
    queue& operator=(const queue& other);
    queue& operator=(queue&& other);
    
    void push(const value& val);
    void push(value&& val);
    
    void pop();
    
    const value& front() const;
    value& front();
    
    bool empty() const;
    size_t get_size() const;
};

template<class value>
queue<value>::queue(const queue& other){
    copy(other);
}

template<class value>
queue<value>::queue(queue&& other){
    move(std::move(other));
}

template<class value>
queue<value>& queue<value>::operator=(const queue& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value>
queue<value>& queue<value>::operator=(queue&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value>
queue<value>::~queue(){
    destroy();
}

template<class value>
void queue<value>::push(const value& val){
    node* new_tail = new node(val);
    if(!head)
        head = tail = new_tail;
    else{
        tail->next = new_tail;
        tail = new_tail;
    }
    size++;
}

template<class value>
void queue<value>::push(value&& val){
    node* new_tail = new node(std::move(val));
    if(!head)
        head = tail = new_tail;
    else{
        tail->next = new_tail;
        tail = new_tail;
    }
    size++;
}

template<class value>
void queue<value>::pop(){
    if(empty())
        throw std::invalid_argument("Empty!");
    if(!head->next){
        delete head;
        head = nullptr;
    }
    else{
        node* to_delete = head;
        node* next = head->next;
        head = next;
        delete to_delete;
    }
    size--;
}

template<class value>
const value& queue<value>::front() const{
    if(empty())
        throw std::invalid_argument("Empty!");
    return head->val;
}

template<class value>
value& queue<value>::front(){
    if(empty())
        throw std::invalid_argument("Empty!");
    return head->val;
}

template<class value>
void queue<value>::copy(const queue& other){
    node* other_head = other.head;
    while(other_head){
        push(other_head->val);
        other_head = other_head->next;
    }
}

template<class value>
void queue<value>::move(queue&& other){
    head = other.head;
    tail = other.tail;
    size = other.size;
    
    other.head = other.tail = nullptr;
    other.size = 0;
}

template<class value>
void queue<value>::destroy(){
    node* curr = head;
    while(curr){
        node* to_delete = curr;
        curr = curr->next;
        delete to_delete;
    }
    head = tail = nullptr;
}

template<class value>
bool queue<value>::empty() const{
    return size == 0;
}

template<class value>
size_t queue<value>::get_size() const{
    return size;
}
