#pragma once
#include <iostream>

//implementing stack with linked list using push_front and pop_front methods because they provide constant time complexity!
template<class value>
class stack{
private:
    struct node{
        value val;
        node* next;
        
        node(const value& val) : val(val),next(nullptr){}
    };
    
    node* head = nullptr;
    size_t size = 0;
    
    void copy(const stack& other);
    void move(stack&& other);
    void destroy();
    
public:
    stack() = default;
    stack(const stack& other);
    stack(stack&& other);
    ~stack();
    stack& operator=(const stack& other);
    stack& operator=(stack&& other);
    
    void push(const value& val);
    void push(value&& val);
    
    void pop();
    const value& top() const;
    value& top();
    
    bool empty() const;
    size_t get_size() const;
};


template<class value>
void stack<value>::push(const value& val){
    node* new_head = new node(val);
    if(!head)
        head = new_head;
    else{
        new_head->next = head;
        head = new_head;
    }
    size++;
}

template<class value>
void stack<value>::push(value&& val){
    node* new_head = new node(std::move(val));
    if(!head)
        head = new_head;
    else{
        new_head->next = head;
        head = new_head;
    }
    size++;
}

template<class value>
void stack<value>::pop(){
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
void stack<value>::copy(const stack& other){
    node* other_head = other.head;
    head = new node(other_head->val);
    while(other_head){
        other_head = other_head->next;
        if(other_head){
            head->next = new node(other_head->val);
            head = head->next;
        }
    }
    size = other.size;
}

template<class value>
void stack<value>::move(stack&& other){
    head = other.head;
    size = other.size;
    
    other.head = nullptr;
    other.size = 0;
}

template<class value>
void stack<value>::destroy(){
    node* curr = head;
    while(curr){
        node* to_delete = curr;
        curr = curr->next;
        delete to_delete;
    }
    head = nullptr;
    size = 0;
}

template<class value>
stack<value>::~stack(){
    destroy();
}

template<class value>
stack<value>::stack(const stack& other){
    copy(other);
}

template<class value>
stack<value>::stack(stack&& other){
    move(std::move(other));
}

template<class value>
stack<value>& stack<value>::operator=(const stack& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value>
stack<value>& stack<value>::operator=(stack&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value>
const value& stack<value>::top() const{
    if(empty())
        throw std::invalid_argument("Empty!");
    return head->val;
}

template<class value>
value& stack<value>::top(){
    if(empty())
        throw std::invalid_argument("Empty!");
    return head->val;
}


template<class value>
bool stack<value>::empty() const{
    return size == 0;
}

template<class value>
size_t stack<value>::get_size() const{
    return size;
}
