#pragma once
#include <iostream>

template<class key>
class deque{
private:
    struct node{
        key val;
        node* prev;
        node* next;
        
        node(const key& val) : val(val),prev(nullptr),next(nullptr){}
    };
    
    node* head;
    node* tail;
    size_t count;
    
    void copy(const deque& other);
    void move(deque&& other);
    void destroy();
    
    class iterator{
    private:
        node* curr;
        deque* context;
        friend class deque;
    public:
        iterator() = default;
        iterator(node* curr,deque* context) : curr(curr),context(context){}
        iterator& operator++(){
            if(curr)
                curr = curr->next;
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            ++(*this);
            return copy;
        }
        iterator& operator--(){
            if(!curr){
                if(context && !context->empty())
                    curr = context->tail;
            }
            else
                curr = curr->prev;
            return *this;
        }
        iterator operator--(int){
            iterator copy(*this);
            --(*this);
            return copy;
        }
        key& operator*(){
            return curr->val;
        }
        key* operator->(){
            return &(curr->val);
        }
        bool operator==(const iterator& other) const{
            return curr == other.curr && context == other.context;
        }
        bool operator!=(const iterator& other) const{
            return !(*this == other);
        }
    };
    
    
    class const_iterator{
    private:
        node* curr;
        const deque* context;
        friend class deque;
    public:
        const_iterator() = default;
        const_iterator(node* curr,const deque* context) : curr(curr),context(context){}
        const_iterator& operator++(){
            if(curr)
                curr = curr->next;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_iterator& operator--(){
            if(!curr){
                if(context && !context->empty())
                    curr = context->tail;
            }
            else
                curr = curr->prev;
            return *this;
        }
        const_iterator operator--(int){
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        const key& operator*() const{
            return curr->val;
        }
        const key* operator->() const{
            return &(curr->val);
        }
        bool operator==(const const_iterator& other) const{
            return curr == other.curr && context == other.context;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    deque();
    deque(const deque& other);
    deque(deque&& other);
    ~deque();
    deque& operator=(const deque& other);
    deque& operator=(deque&& other);
    
    void push_back(const key& val);
    void push_back(key&& val);
    
    void push_front(const key& val);
    void push_front(key&& val);
    
    void pop_back();
    void pop_front();
    
    bool empty() const;
    size_t size() const;
    
    iterator begin();
    iterator end();
    const_iterator c_begin() const;
    const_iterator c_end() const;
    
    const_iterator erase(const const_iterator it);
    const_iterator insert(const const_iterator it,const key& val); // insert before
    
    void print() const;
};


template<class key>
void deque<key>::push_back(const key& val){
    node* to_add = new node(val);
    if(!head)
        head = tail = to_add;
    else{
        tail->next = to_add;
        to_add->prev = tail;
        tail = to_add;
    }
    count++;
}

template<class key>
void deque<key>::push_back(key&& val){
    node* to_add = new node(std::move(val));
    if(!head)
        head = tail = to_add;
    else{
        tail->next = to_add;
        to_add->prev = tail;
        tail = to_add;
    }
    count++;
}

template<class key>
void deque<key>::push_front(const key& val){
    node* to_add = new node(val);
    if(!head)
        head = tail = to_add;
    else{
        to_add->next = head;
        head->prev = to_add;
        head = to_add;
    }
    count++;
}

template<class key>
void deque<key>::push_front(key&& val){
    node* to_add = new node(std::move(val));
    if(!head)
        head = tail = to_add;
    else{
        to_add->next = head;
        head->prev = to_add;
        head = to_add;
    }
    count++;
}

template<class key>
void deque<key>::pop_back(){
    if(empty())
        return;
    if(size() == 1){
        delete head;
        head = tail = nullptr;
    }
    else{
        node* to_delete = tail;
        tail->prev->next = nullptr;
        tail = tail->prev;
        delete to_delete;
    }
    count--;
}

template<class key>
void deque<key>::pop_front(){
    if(empty())
        return;
    if(size() == 1){
        delete head;
        head = tail = nullptr;
    }
    else{
        node* to_delete = head;
        head->next->prev = nullptr;
        head = head->next;
        delete to_delete;
    }
    count--;
}

template<class key>
class deque<key>::const_iterator deque<key>::erase(const const_iterator it){
    node* to_delete = it.curr;
    if(it == c_end())
        throw std::runtime_error("Invalid iterator");
    if(!to_delete)
        throw std::runtime_error("Invalid iterator");
    if(it == c_begin()){
        pop_front();
        return const_iterator(head,this);
    }
    if(to_delete == tail){
        pop_back();
        return c_end();
    }
    node* orig_next = to_delete->next;
    node* orig_prev = to_delete->prev;
    to_delete->prev->next = orig_next;
    to_delete->next->prev = orig_prev;
    delete to_delete;
    count--;
    return const_iterator(orig_next,this);
}

template<class key>
class deque<key>::const_iterator deque<key>::insert(const const_iterator it,const key& val){
    if(it == c_begin()){
        push_front(val);
        return c_begin();
    }
    if(it == c_end()){
        push_back(val);
        return const_iterator(tail,this);
    }
    node* curr = it.curr;
    node* to_insert = new node(val);
    node* orig_prev = curr->prev;
    to_insert->next = curr;
    curr->prev = to_insert;
    to_insert->prev = orig_prev;
    orig_prev->next = to_insert;
    count++;
    return const_iterator(to_insert,this);
}

template<class key>
deque<key>::deque() : head(nullptr),tail(nullptr),count(0){}

template<class key>
deque<key>::deque(const deque& other){
    copy(other);
}

template<class key>
deque<key>::deque(deque&& other){
    move(std::move(other));
}

template<class key>
deque<key>& deque<key>::operator=(const deque& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class key>
deque<key>& deque<key>::operator=(deque&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class key>
void deque<key>::copy(const deque& other){
    node* other_head = other.head;
    while(other_head){
        push_back(other_head->val);
        other_head = other_head->next;
    }
    count = other.count;
}

template<class key>
void deque<key>::move(deque&& other){
    head = other.head;
    tail = other.tail;
    count = other.count;
    
    other.head = other.tail = nullptr;
    other.count = 0;
}

template<class key>
void deque<key>::destroy(){
    node* curr = head;
    while(curr){
        node* to_delete = curr;
        curr = curr->next;
        delete to_delete;
    }
    head = tail = nullptr;
    count = 0;
}

template<class key>
deque<key>::~deque(){
    destroy();
}

template<class key>
bool deque<key>::empty() const{
    return count == 0;
}

template<class key>
size_t deque<key>::size() const{
    return count;
}

template<class key>
class deque<key>::iterator deque<key>::begin(){
    return iterator(head,this);
}

template<class key>
class deque<key>::iterator deque<key>::end(){
    return iterator(nullptr,this);
}

template<class key>
class deque<key>::const_iterator deque<key>::c_begin() const{
    return const_iterator(head,this);
}

template<class key>
class deque<key>::const_iterator deque<key>::c_end() const{
    return const_iterator(nullptr,this);
}

template<class key>
void deque<key>::print() const{
    node* curr = head;
    while(curr){
        std::cout<<curr->val<<" ";
        curr = curr->next;
    }
}
