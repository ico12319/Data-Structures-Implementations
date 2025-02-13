#pragma once
#include <iostream>
#include <list>

template<class key,class hasher = std::hash<key>>
class unordered_set{
private:
    using iterator = std::list<key>::iterator;
    std::list<key> data;
    std::vector<std::pair<iterator,size_t>> hash_table;
    double max_load_factor = 0.75;
    hasher hash;
    
    void rehash(size_t new_capacity);
    double load_factor() const;
    
    iterator get_element_by_chain(size_t bucket_index,const key& element);
    
    class const_iterator{
    private:
        std::list<key>::const_iterator it;
        
    public:
        const_iterator(std::list<key>::const_iterator it) : it(it){}
        const_iterator& operator++(){
            ++it;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const key* operator->() const{
            return &(*it);
        }
        const key& operator*() const{
            return *it;
        }
        bool operator==(const const_iterator& other) const{
            return it == other.it;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    unordered_set(size_t initial_capacity = 8);
    std::pair<bool,const_iterator> insert(const key& element);
    const_iterator find(const key& element) const;
    bool erase(const key& element);
    bool erase(const const_iterator it);
    
    bool empty() const;
    size_t size() const;
    
    const_iterator c_begin() const;
    const_iterator c_end() const;
    
    
    void print() const;
};


template<class key,class hasher>
unordered_set<key,hasher>::unordered_set(size_t initial_capacity){
    hash_table.resize(initial_capacity,std::pair<iterator,size_t>(data.end(),0));
}

template<class key,class hasher>
typename unordered_set<key,hasher>::iterator unordered_set<key,hasher>::get_element_by_chain(size_t bucket_index, const key& _key){
    auto chain_info = hash_table[bucket_index];
    auto iter = chain_info.first;
    auto chain_size = chain_info.second;
    if(chain_size == 0)
        return data.end();
    for(int i = 0;i<chain_size;i++){
        if(*iter == _key)
            return iter;
        iter++;
    }
    return data.end();
}

template<class key,class hasher>
std::pair<bool,typename unordered_set<key,hasher>::const_iterator> unordered_set<key,hasher>::insert(const key& element){
    auto bucket_index = hash(element) % hash_table.size();
    auto& current_bucket = hash_table[bucket_index];
    auto iter = get_element_by_chain(bucket_index, element);
    auto size = current_bucket.second;
    
    if(iter != data.end())
        return std::make_pair(false, const_iterator(iter));
    if(size == 0){
        data.push_front(element);
        current_bucket.first = data.begin();
    }
    else{
        auto new_iter = data.insert(current_bucket.first, element);
        current_bucket.first = new_iter;
    }
    current_bucket.second++;
    if(load_factor() >= max_load_factor){
        rehash(hash_table.size() * 2);
        auto new_bucket_index = hash(element) % hash_table.size();
        auto new_pos = get_element_by_chain(new_bucket_index, element);
        return std::make_pair(true, const_iterator(new_pos));
    }
    return std::make_pair(true,const_iterator(current_bucket.first));
}

template<class key,class hasher>
class unordered_set<key,hasher>::const_iterator unordered_set<key,hasher>::find(const key& element) const{
    auto bucket_index = hash(element) % hash_table.size();
    auto iter = get_element_by_chain(bucket_index, element);
    if(iter != data.end())
        return const_iterator(iter);
    return c_end();
}

template<class key,class hasher>
bool unordered_set<key,hasher>::erase(const key& element){
    if(hash_table.empty())
        return false;
    auto bucket_index = hash(element) % hash_table.size();
    auto iter = get_element_by_chain(bucket_index, element);
    if(iter == data.end())
        return false;
    auto& chain_info = hash_table[bucket_index];
    if(iter == chain_info.first){
        auto next = std::next(iter);
        chain_info.first = next;
    }
    data.erase(iter);
    chain_info.second--;
    return true;
}

template<class key,class hasher>
bool unordered_set<key,hasher>::erase(const const_iterator iter){
    if(iter == c_end() || hash_table.empty())
        return false;
    
    auto element = *iter;
    return erase(element);
}

template<class key,class hasher>
double unordered_set<key,hasher>::load_factor() const{
    return data.size() / hash_table.size();
}

template<class key,class hasher>
bool unordered_set<key,hasher>::empty() const{
    return data.size() == 0;
}

template<class key,class hasher>
size_t unordered_set<key,hasher>::size() const{
    return data.size();
}

template<class key,class hasher>
class unordered_set<key,hasher>::const_iterator unordered_set<key,hasher>::c_begin() const{
    return const_iterator(data.begin());
}

template<class key,class hasher>
class unordered_set<key,hasher>::const_iterator unordered_set<key,hasher>::c_end() const{
    return const_iterator(data.end());
}

template<class key,class hasher>
void unordered_set<key,hasher>::rehash(size_t new_capacity){
    std::vector<key> old_data(new_capacity);
    for(auto it = data.begin();it!=data.end();it++)
        old_data.push_back(*it);
    data.clear();
    hash_table.clear();
    hash_table.resize(new_capacity,std::pair<iterator,size_t>(data.end(),0));
    for(int i = 0;i<old_data.size();i++)
        insert(old_data[i]);
}

template<class key,class hasher>
void unordered_set<key,hasher>::print() const{
    for(auto it = data.begin();it!=data.end();it++)
        std::cout<<*it<<" ";
}
