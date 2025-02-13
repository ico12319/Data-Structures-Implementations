#pragma once
#include <iostream>

template<class value>
class singly_linked_list{
private:
    
    struct node{
        value val;
        node* next;
        
        node(const value& val) : val(val),next(nullptr){}
    };
    
    node* head = nullptr;
    node* tail = nullptr;
    size_t size;
    
    void destroy();
    void copy(const singly_linked_list& other);
    void move(singly_linked_list&& other);
    
    
    class iterator{
    private:
        node* curr_element;
        friend class singly_linked_list<value>;
        
    public:
        iterator(node* curr_element) : curr_element(curr_element){}
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
        value& operator*(){
            return curr_element->val;
        }
        
        value* operator->(){
            return &curr_element->val;
        }
        
        bool operator==(const iterator& other) const{
            return curr_element == other.curr_element;
        }
        
        bool operator!=(const iterator& other) const{
            return !(*this == other);
        }
    };
    
    class const_iterator{
    private:
        node* curr_element;
        friend class singly_linked_list<value>;
        
    public:
        const_iterator(node* curr_element) : curr_element(curr_element){}
        const_iterator(const iterator& it) : curr_element(it.curr_element){}
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
        const value* operator->() const{
            return &curr_element->val;
        }
        const value& operator*() const{
            return curr_element->val;
        }
        
        bool operator==(const const_iterator& other) const{
            return curr_element == other.curr_element;
        }
        
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    singly_linked_list();
    singly_linked_list(const singly_linked_list& other);
    singly_linked_list& operator=(const singly_linked_list& other);
    singly_linked_list& operator=(singly_linked_list&& other);
    singly_linked_list(singly_linked_list&& other);
    ~singly_linked_list();
    
    void push_back(const value& val);
    void push_front(const value& val);
    void pop_front();
    
    const value& front() const;
    const value& back() const;
    bool is_empty() const;
    size_t get_size() const;
    
    iterator begin(){
        return iterator(head);
    }
    iterator end(){
        return iterator(nullptr);
    }
    const_iterator c_begin() const{
        return const_iterator(head);
    }
    const_iterator c_end() const{
        return const_iterator(nullptr);
    }
    
    const_iterator insert_after(const value& val,const const_iterator& it);
    const_iterator remove_after(const const_iterator& it);
    void print() const;
};

template<class value>
void singly_linked_list<value>::push_back(const value& val){
    class singly_linked_list<value>::node* to_add = new singly_linked_list<value>::node(val);
    if(!tail)
        head = tail = to_add;
    else{
        tail->next = to_add;
        tail = to_add;
    }
    size++;
}

template<class value>
void singly_linked_list<value>::push_front(const value& val){
    node* to_add = new node(val);
    if(!head)
        head = tail = to_add;
    else{
        to_add->next = head;
        head = to_add;
    }
    size++;
}

template<class value>
const value& singly_linked_list<value>::front() const{
    if(!head)
        throw std::invalid_argument("Empty list!");
    return head->val;
}


template<class value>
const value& singly_linked_list<value>::back() const{
    if(!tail)
        throw std::invalid_argument("Empty list!");
    return tail->val;
}

template<class value>
void singly_linked_list<value>::copy(const singly_linked_list& other){
    node* curr = other.head;
    while(curr){
        push_back(curr->val);
        curr = curr->next;
    }
}

template<class value>
void singly_linked_list<value>::move(singly_linked_list&& other){
    this->head = other.head;
    this->tail = other.tail;
    this->size = other.size;
    
    other.size = 0;
    other.head = other.tail = nullptr;
}

template<class value>
void singly_linked_list<value>::destroy(){
    node* curr = head;
    while(curr){
        node* to_delete = curr;
        curr = curr->next;
        delete to_delete;
    }
    head = tail = nullptr;
    size = 0;
}

template<class value>
singly_linked_list<value>::singly_linked_list() : head(nullptr),tail(nullptr),size(0){}

template<class value>
singly_linked_list<value>::singly_linked_list(const singly_linked_list& other){
    copy(other);
    size = other.size;
}

template<class value>
singly_linked_list<value>::singly_linked_list(singly_linked_list&& other){
    move(std::move(other));
}

template<class value>
singly_linked_list<value>& singly_linked_list<value>::operator=(const singly_linked_list& other){
    if(this!=&other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value>
singly_linked_list<value>& singly_linked_list<value>::operator=(singly_linked_list&& other){
    move(std::move(other));
}

template<class value>
singly_linked_list<value>::~singly_linked_list(){
    destroy();
}

template<class value>
bool singly_linked_list<value>::is_empty() const{
    return size == 0;
}

template<class value>
size_t singly_linked_list<value>::get_size() const{
    return size;
}


template<class value>
void singly_linked_list<value>::pop_front(){
    if(!head)
        throw std::invalid_argument("Empty list!");
    
    if(head == tail){
        delete head;
        head = tail = nullptr;
    }
    else{
        node* to_delete = head;
        head = head->next;
        to_delete->next = nullptr;
        delete to_delete;
    }
    
    size--;
}

template<class value>
class singly_linked_list<value>::const_iterator singly_linked_list<value>::insert_after(const value& val, const const_iterator& it){
    if(it == c_end())
        return c_end();
    node* current = it.curr_element;
    node* new_node = new node(val);
    node* orig_next = current->next;
    current->next = new_node;
    new_node->next = orig_next;
    if(current == tail)
        tail = new_node;
    
    return const_iterator(new_node);
}

template<class value>
class singly_linked_list<value>::const_iterator singly_linked_list<value>::remove_after(const const_iterator& it){
    if(it == c_end() || !it.curr_element->next)
        return c_end();
    node* current = it.curr_element;
    node* to_delete = current->next;
    node* new_next = current->next->next;
    current->next = new_next;
    size--;
    if(to_delete == tail)
        tail = current;
    delete to_delete;
    return const_iterator(new_next);
    
}


template<class value>
void singly_linked_list<value>::print() const{
    node* curr = head;
    while(curr){
        std::cout<<curr->val<<" ";
        curr = curr->next;
    }
    std::cout<<std::endl;
}
