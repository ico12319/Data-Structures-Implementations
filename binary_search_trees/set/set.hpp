#pragma once
#include <iostream>
#include <stack>

template<class value,class comparator = std::less<value>>
class set{
private:
    struct node{
        value data;
        node* left;
        node* right;
        
        node(int val) : data(val),left(nullptr),right(nullptr){}
    };
    
    node* root;
    size_t size;
    comparator cmp;
    
    static node** find_min(node** root){
        node** curr = root;
        while((*curr)->left)
            curr = &(*curr)->left;
        return curr;
    }
    
    static node* copy(node* root);
    static void destroy(node* root);
    void move(set&& other);
    static void print_helper(node* root);
    
    class forward_iterator{
    private:
        std::stack<node*> nodes;
        
        void push_left(node* root){
            node* curr = root;
            while(curr){
                nodes.push(curr);
                curr = curr->left;
            }
        }
    public:
        forward_iterator(node* root){
            push_left(root);
        }
        forward_iterator& operator++(){
            node* curr = nodes.top();
            nodes.pop();
            if(curr->right)
                push_left(curr->right);
            return *this;
        }
        forward_iterator operator++(int){
            forward_iterator copy(*this);
            ++(*this);
            return copy;
        }
        value& operator*() const{
            return nodes.top()->data;
        }
        bool operator==(const forward_iterator& other) const{
            return nodes == other.nodes;
        }
        bool operator!=(const forward_iterator& other) const{
            return !(*this == other);
        }
    };
    
    class backward_iterator{
    private:
        std::stack<node*> nodes;
        
        void push_right(node* root){
            node* curr = root;
            while(curr){
                nodes.push(curr);
                curr = curr->right;
            }
        }
    public:
        backward_iterator(node* root){
            push_right(root);
        }
        backward_iterator& operator--(){
            node* curr = nodes.top();
            nodes.pop();
            if(curr->left)
                push_right(curr->left);
        }
        backward_iterator operator--(int){
            backward_iterator copy(*this);
            ++(*this);
            return copy;
        }
        value& operator*() const{
            return nodes.top()->data;
        }
        bool operator==(const backward_iterator& other) const{
            return nodes == other.nodes;
        }
        bool operator!=(const backward_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    set();
    set(const set& other);
    set(set&& other);
    set& operator=(const set& other);
    set& operator=(set&& other);
    ~set();
    
    void insert(const value& val);
    bool contains(const value& val) const;
    void erase(const value& val);
    
    void print() const;
    bool is_empty() const;
    size_t get_size() const;
    
    forward_iterator begin() const;
    backward_iterator end() const;
};

template<class value,class comparator>
void set<value,comparator>::insert(const value& val){
    node** curr = &root;
    while(*curr){
        if(cmp((*curr)->data,val))
            curr = &(*curr)->right;
        else if(cmp(val,(*curr)->data))
            curr = &(*curr)->left;
        else
            return;
    }
    *curr = new node(val);
    size++;
}

template<class value,class comparator>
bool set<value,comparator>::contains(const value& val) const{
    node* curr = root;
    while(curr){
        if(cmp(curr->data,val))
            curr = curr->right;
        else if(cmp(val,curr->data))
            curr = curr->left;
        else
            return true;
    }
    return false;
}


template<class value,class comparator>
void set<value,comparator>::erase(const value& val){
    node** curr = &root;
    while(*curr){
        if(cmp((*curr)->data,val))
            curr = &(*curr)->right;
        else if(cmp(val,(*curr)->data))
            curr = &(*curr)->left;
        else
            break;
    }
    if(!(*curr))
        return;
    node* to_delete = *curr;
    if(!(*curr)->left && !(*curr)->right)
        *curr = nullptr;
    else if(!(*curr)->left)
        *curr = (*curr)->right;
    else if(!(*curr)->right)
        *curr = (*curr)->left;
    else{
        node** right_min = find_min(&(*curr)->right);
        *curr = *right_min;
        *right_min = (*right_min)->right;
        
        (*curr)->left = to_delete->left;
        (*curr)->right = to_delete->right;
    }
    delete to_delete;
    size--;
}

template<class value,class comparator>
class set<value,comparator>::forward_iterator set<value,comparator>::begin() const{
    return forward_iterator(root);
}

template<class value,class comparator>
class set<value,comparator>::backward_iterator set<value,comparator>::end() const{
    return bacward_iterator(root);
}

template<class value,class comparator>
bool set<value,comparator>::is_empty() const{
    return size == 0;
}

template<class value,class comparator>
size_t set<value,comparator>::get_size() const{
    return size;
}

template<class value,class comparator>
void set<value,comparator>::destroy(node* root){
    if(!root)
        return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}

template<class value,class comparator>
class set<value,comparator>::node* set<value,comparator>::copy(node* root){
    if(!root)
        return root;
    node* res = new node(root->data);
    res->left = copy(root->left);
    res->right = copy(root->right);
    return res;
}

template<class value,class comparator>
void set<value,comparator>::print_helper(node* root){
    if(!root)
        return;
    print_helper(root->left);
    std::cout<<root->data<<" ";
    print_helper(root->right);
}

template<class value,class comparator>
void set<value,comparator>::print() const{
    print_helper(root);
}

template<class value,class comparator>
void set<value,comparator>::move(set&& other){
    this->root = other.root;
    this->size = other.size;
    
    other.root = nullptr;
    other.size = 0;
}

template<class value,class comparator>
set<value,comparator>::set() : root(nullptr),size(0){}

template<class value,class comparator>
set<value,comparator>::set(const set& other){
    this->root = copy(other.root);
}

template<class value,class comparator>
set<value,comparator>::set(set&& other){
    move(std::move(other));
}

template<class value,class comparator>
set<value,comparator>& set<value,comparator>::operator=(const set& other){
    if(this != &other){
        destroy(root);
        this->root = copy(other.root);
        this->size = other.size;
    }
    return *this;
}

template<class value,class comparator>
set<value,comparator>& set<value,comparator>::operator=(set&& other){
    if(this != &other){
        destroy(root);
        move(std::move(other));
    }
    return *this;
}

template<class value,class comparator>
set<value,comparator>::~set(){
    destroy(root);
}


