#pragma once
#include <iostream>
#include <stack>

template<class value,class comparator = std::less<value>>
class binary_search_tree{
private:
    struct node{
        value data;
        node* left;
        node* right;
        size_t subtree_size = 1;
        
        static size_t get_subtree_size(const node* node){
            return node ? node->subtree_size : 0;
        }
        
        node(const value& data) : data(data),left(nullptr),right(nullptr),subtree_size(1){}
    };
    
    node* root;
    comparator cmp;
    
    static void print_helper(node* root);
    static void in_order(node* root,std::vector<int>& sorted);
    static node* copy(node* root);
    static bool contains_helper(node* root,const value& val,const comparator& cmp);
    static void destroy(node* root);
    void move(binary_search_tree&& other);
    const value& get_index(size_t index,node* root) const;
    static node* insert_helper(const value& val,node* root,const comparator& cmp);
    static node** find_min(node** root,std::stack<node*>& nodes);
    
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
        
        const value& operator*() const{
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
    binary_search_tree();
    binary_search_tree(const comparator& cmp);
    binary_search_tree(const binary_search_tree& other);
    binary_search_tree(binary_search_tree&& other);
    binary_search_tree& operator=(const binary_search_tree& other);
    binary_search_tree& operator=(binary_search_tree&& other);
    ~binary_search_tree();
    
    void insert(const value& val);
    bool contains(const value& val) const;
    void erase(const value& val);
    
    bool is_empty() const;
    size_t size() const;
    const value& operator[](size_t index) const;
    
    std::vector<int> tree_sort() const;
    
    forward_iterator c_begin() const;
    forward_iterator c_end() const;
    void print() const;
};


template<class value,class comparator>
class binary_search_tree<value,comparator>::node* binary_search_tree<value,comparator>::insert_helper(const value& val, node* root,const comparator& cmp){
    if(!root)
        return new node(val);
    if(cmp(root->data,val))
        root->right = insert_helper(val, root->right, cmp);
    else if(cmp(val,root->data))
        root->left = insert_helper(val, root->left, cmp);
    else
        return root;
    root->subtree_size = node::get_subtree_size(root->left) + node::get_subtree_size(root->right) + 1;
    return root;
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::insert(const value& val){
    root = insert_helper(val, root,cmp);
}

template<class value,class comparator>
bool binary_search_tree<value,comparator>::contains_helper(node* root, const value& val, const comparator& cmp){
    if(!root)
        return false;
    if(cmp(root->data,val))
        return contains_helper(root->right, val, cmp);
    else if(cmp(val,root->data))
        return contains_helper(root->left,val,cmp);
    else
        return true;
}

template<class value,class comparator>
bool binary_search_tree<value,comparator>::contains(const value& val) const{
    return contains_helper(root, val, cmp);
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::erase(const value& val){
    node** curr = &root;
    std::stack<node*> nodes;
    while(*curr){
        nodes.push(*curr);
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
        node** right_min = find_min(&(*curr)->right,nodes);
        *curr = *right_min;
        *right_min = (*right_min)->right;
        
        (*curr)->left = to_delete->left;
        (*curr)->right = to_delete->right;
    }
    while(!nodes.empty()){
        node* curr = nodes.top();
        nodes.pop();
        --curr->subtree_size;
    }
    delete to_delete;
}

template<class value,class comparator>
const value& binary_search_tree<value,comparator>::get_index(size_t index, node* root) const{
    size_t left_subtree_size = node::get_subtree_size(root->left);
    if(left_subtree_size == index)
        return root->data;
    else if(index < left_subtree_size)
        return get_index(index, root->left);
    
    return get_index(index - left_subtree_size - 1, root->right);
}

template<class value,class comparator>
class binary_search_tree<value,comparator>::node** binary_search_tree<value,comparator>::find_min(node** root,std::stack<node*>& nodes){
    node** curr = root;
    while((*curr)->left){
        nodes.push(*curr);
        curr = &(*curr)->left;
    }
    return curr;
}

template<class value,class comparator>
const value& binary_search_tree<value,comparator>::operator[](size_t index) const{
    return get_index(index, root);
}

template<class value,class comparator>
bool binary_search_tree<value,comparator>::is_empty() const{
    return root == nullptr;
}

template<class value,class comparator>
size_t binary_search_tree<value,comparator>::size() const{
    return node::get_subtree_size(root);
}

template<class value,class comparator>
binary_search_tree<value,comparator>::binary_search_tree() : root(nullptr){}

template<class value,class comparator>
binary_search_tree<value,comparator>::binary_search_tree(const binary_search_tree& other){
    root = copy(other.root);
}

template<class value,class comparator>
binary_search_tree<value,comparator>::binary_search_tree(binary_search_tree&& other){
    move(std::move(other));
}

template<class value,class comparator>
class binary_search_tree<value,comparator>::node* binary_search_tree<value,comparator>::copy(node* root){
    if(!root)
        return root;
    node* res = new node(root->data);
    res->left = copy(root->left);
    res->right = copy(root->right);
    res->subtree_size = root->subtree_size;
    return res;
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::move(binary_search_tree&& other){
    this->root = other.root;
    other.root = nullptr;
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::destroy(node* root){
    if(!root)
        return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}

template<class value,class comparator>
binary_search_tree<value,comparator>::~binary_search_tree(){
    destroy(root);
}

template<class value,class comparator>
binary_search_tree<value,comparator>& binary_search_tree<value,comparator>::operator=(const binary_search_tree& other){
    if(this != &other){
        destroy(root);
        root = copy(other.root);
    }
    return *this;
}

template<class value,class comparator>
binary_search_tree<value,comparator>& binary_search_tree<value,comparator>::operator=(binary_search_tree&& other){
    if(this != &other){
        destroy(root);
        move(std::move(other));
    }
    return *this;
}

template<class value,class comparator>
binary_search_tree<value,comparator>::binary_search_tree(const comparator& cmp) : root(nullptr),cmp(cmp){}


template<class value,class comparator>
void binary_search_tree<value,comparator>::in_order(node* root, std::vector<int>& sorted){
    if(!root)
        return;
    in_order(root->left, sorted);
    sorted.push_back(root->data);
    in_order(root->right, sorted);
}


template<class value,class comparator>
std::vector<int> binary_search_tree<value,comparator>::tree_sort() const{
    std::vector<int> sorted;
    in_order(root, sorted);
    return sorted;
}

template<class value,class comparator>
class binary_search_tree<value,comparator>::forward_iterator binary_search_tree<value,comparator>::c_begin() const{
    return forward_iterator(root);
}

template<class value,class comparator>
class binary_search_tree<value,comparator>::forward_iterator binary_search_tree<value,comparator>::c_end() const{
    return forward_iterator(nullptr);
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::print_helper(node* root){
    if(!root)
        return;
    print_helper(root->left);
    std::cout<<root->data<<" ";
    print_helper(root->right);
}

template<class value,class comparator>
void binary_search_tree<value,comparator>::print() const{
    print_helper(root);
}
