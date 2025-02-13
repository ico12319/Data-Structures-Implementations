#pragma once
#include <iostream>

template<class value>
class doubly_linked_list{
private:
    struct node{
        value val;
        node* next;
        node* prev;
        
        node(int val) : val(val),next(nullptr),prev(nullptr){}
    };
    
    node* head;
    node* tail;
    size_t size;
    
    void copy(const doubly_linked_list& other);
    void move(doubly_linked_list&& other);
    void destroy();
    
    class iterator{
    private:
        node* curr_element;
        doubly_linked_list* context;
        friend class doubly_linked_list<value>;
        
    public:
        iterator(node* curr_element,doubly_linked_list* context) : curr_element(curr_element),context(context){}
        iterator& operator++(){
            if(curr_element)
                curr_element = curr_element->next;
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            ++(*this);
            return copy;
        }
        iterator& operator--(){
            if(curr_element)
                curr_element = curr_element->prev;
            else
                curr_element = context->tail;
            return *this;
        }
        iterator operator--(int){
            iterator copy(*this);
            --(*this);
            return copy;
        }
        value& operator*(){
            return curr_element->val;
        }
        value* operator->(){
            return &curr_element->val;
        }
        bool operator==(const iterator& other) const{
            return curr_element == other.curr_element && context == other.context;
        }
        bool operator!=(const iterator& other) const{
            return !(*this == other);
        }
    };
    
    class const_iterator{
    private:
        node* curr_element;
        const doubly_linked_list* context;
        friend class doubly_linked_list<value>;
        
    public:
        const_iterator(node* curr_element,const doubly_linked_list* context) : curr_element(curr_element),context(context){}
        const_iterator(const iterator& it) : curr_element(it.curr_element),context(it.context){}
        const_iterator& operator++(){
            if(curr_element)
                curr_element = curr_element->next;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_iterator& operator--(){
            if(curr_element)
                curr_element = curr_element->prev;
            else
                curr_element = context->tail;
            return *this;
        }
        const_iterator operator--(int){
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        
        const value& operator*() const{
            return curr_element->val;
        }
        const value* operator->() const{
            return &curr_element->val;
        }
        bool operator==(const const_iterator& other) const{
            return curr_element == other.curr_element && context == other.context;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    doubly_linked_list();
    doubly_linked_list(const doubly_linked_list& other);
    doubly_linked_list(doubly_linked_list&& other);
    ~doubly_linked_list();
    doubly_linked_list& operator=(const doubly_linked_list& other);
    doubly_linked_list& operator=(doubly_linked_list&& other);
    
    void push_back(const value& val);
    void push_front(const value& val);
    void pop_back();
    void pop_front();
    const value& front() const;
    const value& back() const;
    
    value& front();
    value& back();
    void print() const;
    bool is_empty() const;
    size_t get_size() const;
    
    iterator begin();
    iterator end();
    const_iterator c_begin() const;
    const_iterator c_end() const;
    
    const_iterator insert(const value& val,const const_iterator& it);
    const_iterator remove(const const_iterator& it);
};

//4 -> 1 -> 2 -> 3
template<class value>
void doubly_linked_list<value>::push_back(const value& val){
    node* new_node = new node(val);
    if(!tail)
        head = tail = new_node;
    else{
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }
    size++;
}

template<class value>
void doubly_linked_list<value>::push_front(const value& val){
    node* new_node = new node(val);
    if(!head)
        head = tail = new_node;
    else{
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }
    size++;
}
 // 1 -> 2 -> 3 -> 4
template<class value>
void doubly_linked_list<value>::pop_back(){
    if(is_empty())
        throw std::invalid_argument("Empty list!");
    
    if(head == tail){
        delete tail;
        head = tail = nullptr;
    }
    else{
        node* to_delete = tail;
        to_delete->prev->next = nullptr;
        tail = tail->prev;
        delete to_delete;
    }
    size--;
}
// 1 -> 2 -> 3
template<class value>
void doubly_linked_list<value>::pop_front(){
    if(is_empty())
        throw std::invalid_argument("Empty list!");
    
    if(head == tail){
        delete head;
        head = tail = nullptr;
    }
    else{
        node* to_delete = head;
        to_delete->next->prev = nullptr;
        head = head->next;
        delete to_delete;
    }
    size--;
}

template<class value>
void doubly_linked_list<value>::copy(const doubly_linked_list& other){
    node* curr = other.head;
    while(curr != other.tail){
        push_back(curr->val);
        curr = curr->next;
    }
}

template<class value>
void doubly_linked_list<value>::move(doubly_linked_list&& other){
    this->head = other.head;
    this->tail = other.tail;
    this->size = other.size;
    
    other.size = 0;
    other.head = other.tail = nullptr;
}

template<class value>
void doubly_linked_list<value>::destroy(){
    node* curr = head;
    while(curr != tail){
        node* to_delete = curr;
        curr = curr->next;
        delete to_delete;
    }
}

template<class value>
doubly_linked_list<value>::doubly_linked_list() : head(nullptr),tail(nullptr),size(0){}

template<class value>
doubly_linked_list<value>& doubly_linked_list<value>::operator=(const doubly_linked_list& other){
    if(this != &other){
        destroy();
        copy(other);
        this->size = other.size;
    }
    return *this;
}

template<class value>
doubly_linked_list<value>& doubly_linked_list<value>::operator=(doubly_linked_list&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value>
doubly_linked_list<value>::~doubly_linked_list(){
    destroy();
}

template<class value>
doubly_linked_list<value>::doubly_linked_list(const doubly_linked_list& other){
    copy(other);
    this->size = other.size;
}

template<class value>
doubly_linked_list<value>::doubly_linked_list(doubly_linked_list&& other){
    move(std::move(other));
}

template<class value>
const value& doubly_linked_list<value>::front() const{
    if(!head)
        throw std::invalid_argument("Empty list");
    return head->val;
}

template<class value>
const value& doubly_linked_list<value>::back() const{
    if(!tail)
        throw std::invalid_argument("Empty list");
    return tail->val;
}

template<class value>
value& doubly_linked_list<value>::front(){
    if(!head)
        throw std::invalid_argument("Empty list");
    return head->val;
}

template<class value>
value& doubly_linked_list<value>::back(){
    if(!tail)
        throw std::invalid_argument("Empty list");
    return tail->val;
}

template<class value>
bool doubly_linked_list<value>::is_empty() const{
    return size == 0;
}

template<class value>
size_t doubly_linked_list<value>::get_size() const{
    return size;
}

template<class value>
void doubly_linked_list<value>::print() const{
    node* curr = head;
    while(curr){
        std::cout<<curr->val<<" ";
        curr = curr->next;
    }
}


template<class value>
class doubly_linked_list<value>::const_iterator doubly_linked_list<value>::c_begin() const{
    return const_iterator(head,this);
}

template<class value>
class doubly_linked_list<value>::const_iterator doubly_linked_list<value>::c_end() const{
    return const_iterator(nullptr, this);
}

template<class value>
class doubly_linked_list<value>::iterator doubly_linked_list<value>::begin(){
    return iterator(head, this);
}

template<class value>
class doubly_linked_list<value>::iterator doubly_linked_list<value>::end(){
    return iterator(nullptr, this);
}

template<class value>
class doubly_linked_list<value>::const_iterator doubly_linked_list<value>::insert(const value& val, const const_iterator& it){
    if(it == c_begin()){
        push_front(val);
        return const_iterator(head, this);
    }
    if(it == c_end()){
        push_back(val);
        return const_iterator(tail, this);
    }
    node* new_node = new node(val);
    node* curr_node = it.curr_element;
    node* orig_prev = curr_node->prev;
    orig_prev->next = new_node;
    new_node->prev = orig_prev;
    new_node->next = curr_node;
    curr_node->prev = new_node;
    size++;
    return const_iterator(new_node, this);
}

template<class value>
class doubly_linked_list<value>:: const_iterator doubly_linked_list<value>::remove(const const_iterator& it){
    
    node* to_delete = it.curr_element;
    if(to_delete == tail){
        pop_back();
        return end();
    }
    if(to_delete == head){
        pop_front();
        return begin();
    }
    node* next = to_delete->next;
    node* prev = to_delete->prev;
    to_delete->prev = nullptr;
    to_delete->next = nullptr;
    prev->next = next;
    next->prev = prev;
    delete to_delete;
    size--;
    return const_iterator(next, this);
}
