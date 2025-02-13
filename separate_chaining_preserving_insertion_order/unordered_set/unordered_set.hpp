#pragma once
#include <iostream>
#include <list>
#include <forward_list>

template<class key,class hasher = std::hash<key>>
class unordered_set{
private:
    using iterator = std::list<key>::const_iterator;
    std::list<key> data;
    std::vector<std::forward_list<iterator>> hash_table;
    const double max_load_factor;
    hasher hash_function;
    
    void resize(size_t new_capacity);
    double load_factor() const;
    
    class hash_iterator{
    private:
        std::list<key>::const_iterator it;
        friend class unordered_set;
        
    public:
        hash_iterator(std::list<key>::const_iterator it) : it(it){};
        hash_iterator& operator++(){
            ++it;
            return *this;
        }
        hash_iterator operator++(int){
            hash_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const key* operator->() const{
            return &(*it);
        }
        const key& operator*() const{
            return *it;
        }
        bool operator==(const hash_iterator& other) const{
            return it == other.it;
        }
        bool operator!=(const hash_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    unordered_set();
    unordered_set(size_t size,const hasher& hash_function,double load_factor);
    void insert(const key& _key);
    bool contains(const key& _key) const;
    void erase(const key& _key);
    hash_iterator find(const key& _key) const;
    size_t size() const;
    bool is_empty() const;
    hash_iterator c_begin() const;
    hash_iterator c_end() const;
    void print() const;
};

template<class key,class hasher>
unordered_set<key,hasher>::unordered_set() : max_load_factor(0.75){
    hash_table.resize(8);
}

template<class key,class hasher>
unordered_set<key,hasher>::unordered_set(size_t size,const hasher& hash_fucntion,double load_factor) : hash_function(hash_function),max_load_factor(load_factor){
    hash_table.resize(8);
}

template<class key,class hasher>
void unordered_set<key,hasher>::insert(const key& _key){
    if(load_factor() >= max_load_factor)
        resize(hash_table.size() * 2);
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto iter = std::find_if(current_bucket.begin(),current_bucket.end(),[&_key](iterator it){return *it == _key;});
    
    if(iter != current_bucket.end())
        return;
    data.push_back(_key);
    current_bucket.push_front(--data.end());
}

template<class key,class hasher>
bool unordered_set<key,hasher>::contains(const key& _key) const{
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto iter = std::find_if(current_bucket.begin(),current_bucket.end(),[&_key](iterator it){return *it == _key;});
    
    return iter != current_bucket.end();
}

template<class key,class hasher>
void unordered_set<key,hasher>::erase(const key& _key){
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto lambda = [&_key,this](iterator it){
        if(*it == _key){
            data.erase(it);
            return true;
        }
        return false;
    };
    current_bucket.remove_if(lambda);
}

template<class key,class hasher>
class unordered_set<key,hasher>::hash_iterator unordered_set<key,hasher>::find(const key& _key) const{
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto iter = std::find_if(current_bucket.begin(),current_bucket.end(),[&_key](iterator it){return *it == _key;});
    if(iter == current_bucket.end())
        return c_end();
    return hash_iterator(*iter);
}

template<class key,class hasher>
bool unordered_set<key,hasher>::is_empty() const{
    return data.empty();
}

template<class key,class hasher>
size_t unordered_set<key,hasher>::size() const{
    return data.size();
}

template<class key,class hasher>
class unordered_set<key,hasher>::hash_iterator unordered_set<key,hasher>::c_begin() const{
    return hash_iterator(data.begin());
}

template<class key,class hasher>
class unordered_set<key,hasher>::hash_iterator unordered_set<key,hasher>::c_end() const{
    return hash_iterator(data.end());
}

template<class key,class hasher>
void unordered_set<key,hasher>::resize(size_t new_capacity){
    std::vector<std::forward_list<iterator>> new_data(new_capacity);
    for(auto it = data.begin();it!=data.end();it++){
        auto new_bucket_index = hash_function(*it) % new_capacity;
        new_data[new_bucket_index].push_front(it);
    }
    hash_table = std::move(new_data);
}

template<class key,class hasher>
double unordered_set<key,hasher>::load_factor() const{
    return static_cast<double>(data.size()) / hash_table.size();
}

template<class key,class hasher>
void unordered_set<key,hasher>::print() const{
    for(const auto& el : data)
        std::cout<<el<<" ";
}
