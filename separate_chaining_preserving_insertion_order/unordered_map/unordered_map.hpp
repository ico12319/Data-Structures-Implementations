#pragma once
#include <iostream>
#include <forward_list>
#include <list>

template<class key,class value,class hasher = std::hash<key>>
class unordered_map{
private:
    using iterator = std::list<std::pair<key,value>>::const_iterator;
    std::vector<std::forward_list<iterator>> hash_table;
    std::list<std::pair<key,value>> data;
    const double max_load_factor;
    hasher hash_function;
    
    size_t load_factor() const;
    void resize(size_t new_capacity);
    
    class order_iterator{
    private:
        std::list<std::pair<key,value>>::const_iterator it;
        friend class unordered_map;
    public:
        order_iterator(std::list<std::pair<int,int>>::const_iterator it) : it(it){}
        order_iterator& operator++(){
            ++it;
            return *this;
        }
        order_iterator operator++(int){
            order_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const std::pair<key,value>& operator*() const{
            return *it;
        }
        const std::pair<key,value>* operator->() const{
            return &(*it);
        }
        bool operator==(const order_iterator& other) const{
            return it == other.it;
        }
        bool operator!=(const order_iterator& other) const{
            return !(*this == other);
        }
    };
public:
    unordered_map();
    unordered_map(double load_factor,const hasher& hash);
    unordered_map(size_t size,double load_factor,const hasher& hash);
    void insert(const key& _key,const value& _val);
    void erase(const key& _key);
    bool contains(const key& _key) const;
    order_iterator find(const key& _key) const;
    order_iterator begin() const;
    order_iterator end() const;
    size_t size() const;
    bool is_empty() const;
    void print() const;
};

template<class key,class value,class hasher>
unordered_map<key, value,hasher>::unordered_map() : max_load_factor(0.75){
    hash_table.resize(8);
}

template<class key,class value,class hasher>
void unordered_map<key,value,hasher>::insert(const key& _key, const value& _value){
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto it = std::find_if(current_bucket.begin(), current_bucket.end(), [&_key](iterator it){return it->first == _key;});
    if(it != current_bucket.end())
        return;
    data.emplace_back(_key,_value);
    current_bucket.push_front(--data.end());
}

template<class key,class value,class hasher>
bool unordered_map<key,value,hasher>::contains(const key& _key) const{
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto it = std::find_if(current_bucket.begin(), current_bucket.end(), [&_key](iterator it){return it->first == _key;});
    return it != current_bucket.end();
}

template<class key,class value,class hasher>
void unordered_map<key,value,hasher>::print() const{
    for(const auto& el : data)
        std::cout<<el.first<<" "<<el.second<<std::endl;
}

template<class key,class value,class hasher>
void unordered_map<key,value,hasher>::erase(const key& _key){
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto it = std::find_if(current_bucket.begin(),current_bucket.end(),[&_key](iterator it){return it->first == _key;});
    if(it == current_bucket.end())
        return;
    current_bucket.remove_if([&_key,this](iterator it)
        {if(it->first == _key){
        data.erase(it);
        return true;
    }
        return false;
    });
}

template<class key,class value,class hasher>
size_t unordered_map<key,value,hasher>::load_factor() const{
    return data.size() / hash_table.size();
}

template<class key,class value,class hasher>
unordered_map<key, value,hasher>::unordered_map(size_t size,double load_factor,const hasher& hash) : max_load_factor(load_factor),hash_function(hash){
    hash_table.resize(size);
}


template<class key,class value,class hasher>
unordered_map<key, value,hasher>::unordered_map(double load_factor,const hasher& hash) : max_load_factor(load_factor),hash_function(hash){}

template<class key,class value,class hasher>
size_t unordered_map<key,value,hasher>::size() const{
    return data.size();
}

template<class key,class value,class hasher>
bool unordered_map<key,value,hasher>::is_empty() const{
    return data.empty();
}

template<class key,class value,class hasher>
class unordered_map<key,value,hasher>::order_iterator unordered_map<key,value,hasher>::begin() const{
    return order_iterator(data.begin());
}


template<class key,class value,class hasher>
class unordered_map<key,value,hasher>::order_iterator unordered_map<key,value,hasher>::end() const{
    return order_iterator(data.end());
}


template<class key,class value,class hasher>
class unordered_map<key,value,hasher>::order_iterator unordered_map<key,value,hasher>::find(const key& _key) const{
    auto bucket_index = hash_function(_key) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto it = std::find_if(current_bucket.begin(),current_bucket.end(),[&_key](iterator it){return it->first == _key;});
    if(it == current_bucket.end())
        return order_iterator(data.end());
    
    return order_iterator(*it);
}


template<class key,class value,class hasher>
void unordered_map<key,value,hasher>::resize(size_t new_capacity){
    std::vector<std::forward_list<iterator>> new_table(new_capacity);
    for(auto it = data.begin();it!=data.end();it++){
        auto new_bucket_index = hash_function(it.first) % new_capacity;
        new_table[new_bucket_index].push_front(it);
    }
    data = std::move(new_table);
}
