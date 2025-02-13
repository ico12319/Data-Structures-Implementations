#pragma once
#include <iostream>
#include <stack>

template<class key,class value,class comparator = std::less<key>>
class map{
private:
    struct node{
        std::pair<key,value> data;
        node* left;
        node* right;
        
        node(const std::pair<key,value>& data) : data(data),left(nullptr),right(nullptr){}
    };
    
    node* root;
    size_t count;
    comparator cmp;
    
    static void print_helper(node* root);
    static node* copy(node* root);
    static void destroy(node* root);
    void move(map&& other);
    static node** find_min(node** root);
    
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
        
        const std::pair<key,value>& operator*() const{
            return nodes.top()->data;
        }
        
        bool operator==(const forward_iterator& other) const{
            return nodes == other.nodes;
        }
        
        bool operator!=(const forward_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    map();
    map(const comparator& cmp);
    map(const map& other);
    map(map&& other);
    map& operator=(const map& other);
    map& operator=(map&& other);
    ~map();
    
    void insert(const key& _key,const value& val);
    void erase(const key& _key);
    bool contains(const key& _key) const;
    
    bool is_empty() const;
    size_t size() const;
    void print() const;
    
    forward_iterator c_begin() const;
    forward_iterator c_end() const;
};

template<class key,class value,class comparator>
void map<key,value,comparator>::insert(const key& _key,const value& val){
    auto element_to_add = std::make_pair(_key,val);
    node** curr = &root;
    while(*curr){
        if(cmp((*curr)->data.first,_key))
            curr = &(*curr)->right;
        else if(cmp(_key,(*curr)->data.first))
            curr = &(*curr)->left;
        else
            return;
    }
    *curr = new node(element_to_add);
    count++;
}

template<class key,class value,class comparator>
bool map<key,value,comparator>::contains(const key& _key) const{
    node* curr = root;
    while(curr){
        if(cmp((curr->data).first,_key))
            curr = curr->right;
        else if(cmp(_key,(curr->data).first))
            curr = curr->left;
        else
            return true;
    }
    return false;
}

template<class key,class value,class comparator>
void map<key,value,comparator>::erase(const key& _key){
    node** curr = &root;
    while(*curr){
       if(cmp((*curr)->data.first,_key))
           curr = &(*curr)->right;
        else if(cmp(_key,(*curr)->data.first))
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
    count--;
}

template<class key,class value,class comparator>
class map<key,value,comparator>::forward_iterator map<key,value,comparator>::c_begin() const{
    return forward_iterator(root);
}

template<class key,class value,class comparator>
class map<key,value,comparator>::forward_iterator map<key,value,comparator>::c_end() const{
    return forward_iterator(nullptr);
}

template<class key,class value,class comparator>
map<key,value,comparator>::map() : root(nullptr),count(0){}

template<class key,class value,class comparator>
map<key,value,comparator>::map(const comparator& cmp) : cmp(cmp){}

template<class key,class value,class comparator>
map<key,value,comparator>::map(const map& other){
    this->root = copy(other.root);
    this->count = other.count;
}

template<class key,class value,class comparator>
class map<key,value,comparator>::node* map<key,value,comparator>::copy(node* root){
    if(!root)
        return root;
    node* res = new node(root->data);
    res->left = copy(root->left);
    res->right = copy(root->right);
    return res;
}

template<class key,class value,class comparator>
class map<key,value,comparator>::node** map<key,value,comparator>::find_min(node** root){
    node** curr = root;
    while((*curr)->left)
        curr = &(*curr)->left;
    return curr;
}

template<class key,class value,class comparator>
map<key,value,comparator>& map<key,value,comparator>::operator=(const map<key,value,comparator>& other){
    if(this != &other){
        destroy(root);
        root = copy(other.root);
        this->count = other.count;
    }
    return *this;
}

template<class key,class value,class comparator>
void map<key,value,comparator>::destroy(node* root){
    if(!root)
        return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}


template<class key,class value,class comparator>
map<key,value,comparator>::~map(){
    destroy(root);
}

template<class key,class value,class comparator>
void map<key,value,comparator>::move(map&& other){
    this->root = other.root;
    this->count = other.count;
    
    other.root = nullptr;
    other.count = 0;
}

template<class key,class value,class comparator>
map<key,value,comparator>::map(map&& other){
    move(std::move(other));
}

template<class key,class value,class comparator>
map<key,value,comparator>& map<key,value,comparator>::operator=(map&& other){
    if(this != &other){
        destroy(root);
        move(std::move(other));
    }
    return *this;
}

template<class key,class value,class comparator>
void map<key,value,comparator>::print_helper(node* root){
    if(!root)
        return;
    print_helper(root->left);
    std::cout<<root->data.first<<" "<<root->data.second<<std::endl;
    print_helper(root->right);
}

template<class key,class value,class comparator>
void map<key,value,comparator>::print() const{
    print_helper(root);
}

template<class key,class value,class comparator>
bool map<key,value,comparator>::is_empty() const{
    return count == 0;
}

template<class key,class value,class comparator>
size_t map<key,value,comparator>::size() const{
    return count;
}
