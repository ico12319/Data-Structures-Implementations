#pragma once
#include <iostream>
#include <list>

template<class key,class value,class hash = std::hash<key>>
class unordered_map{
private:
    std::list<std::pair<key,value>> data;
    using iterator = std::list<std::pair<key,value>>::iterator;
    std::vector<std::pair<iterator,size_t>> hash_table;
    hash hasher;
    double max_load_factor = 0.75;
    
    void rehash(size_t new_capacity);
    double load_factor() const;
    
    class const_iterator{
    private:
        friend class unordered_map;
        std::list<std::pair<key,value>>::const_iterator it;
        
    public:
        const_iterator(std::list<std::pair<key,value>>::const_iterator it) : it(it){}
        const_iterator& operator++(){
            ++it;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const value* operator->() const{
            return &(*it);
        }
        const value& operator*() const{
            return *it;
        }
        bool operator==(const const_iterator& other) const{
            return it == other.it;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
    iterator get_element_by_chain(size_t bucket_index,const key& element);
    
public:
    unordered_map(size_t initial_capacity = 8);
    std::pair<bool,const_iterator> insert(const key& element,const value& val);
    bool erase(const key& element);
    bool erase(const const_iterator it);
    const_iterator find(const key& element) const;
    
    bool empty() const;
    size_t size() const;
    
    const_iterator c_begin() const;
    const_iterator c_end() const;
    void print() const{
        for(auto it = data.begin();it!=data.end();it++)
            std::cout<<it->first<<" "<<it->second<<std::endl;
    }
};

template<class key,class value,class hash>
unordered_map<key,value,hash>::unordered_map(size_t initial_capacity) : max_load_factor(0.75){
    hash_table.resize(initial_capacity,std::pair<iterator,size_t>(data.end(),0));
}

template<class key,class value,class hash>
typename unordered_map<key,value,hash>::iterator unordered_map<key,value,hash>::get_element_by_chain(size_t bucket_index,const key& element){
    auto& chain_info = hash_table[bucket_index];
    auto chain_size = chain_info.second;
    if(chain_size == 0)
        return data.end();
    auto iter = chain_info.first;
    for(int i = 0;i<chain_size;i++){
        if(iter->first == element)
            return iter;
        iter++;
    }
    return data.end();
}

template<class key,class value,class hash>
std::pair<bool,typename unordered_map<key,value,hash>::const_iterator> unordered_map<key,value,hash>::insert(const key& element,const value& val){
    auto bucket_index = hasher(element) % hash_table.size();
    auto& chain_info = hash_table[bucket_index];
    auto find_iter = get_element_by_chain(bucket_index, element);
    if(find_iter != data.end())
        return std::make_pair(false,const_iterator(find_iter));
    if(chain_info.second == 0){
        data.push_front(std::make_pair(element,val));
        chain_info.first = data.begin();
    }
    else{
        auto new_iter = data.insert(chain_info.first, std::make_pair(element,val));
        chain_info.first = new_iter;
    }
    chain_info.second++;
    
    if(load_factor() >= max_load_factor){
        rehash(hash_table.size() * 2);
        auto new_bucket_index = hasher(element) % hash_table.size();
        auto new_pos = get_element_by_chain(new_bucket_index, element);
        return std::make_pair(true,const_iterator(new_pos));
    }
    return std::make_pair(true,const_iterator(chain_info.first));
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::find(const key& element) const{
    if(hash_table.empty())
        return false;
    auto bucket_index = hasher(element) % hash_table.size();
    auto iter = get_element_by_chain(bucket_index,element);
    if(iter == data.end())
        return false;
    return true;
}

template<class key,class value,class hash>
bool unordered_map<key,value,hash>::erase(const key& element){
    if(hash_table.empty())
        return false;
    auto bucket_index = hasher(element) % hash_table.size();
    auto& chain_info = hash_table[bucket_index];
    auto found_iter = get_element_by_chain(bucket_index,element);
    if(found_iter == data.end())
        return false;
    if(chain_info.first == found_iter){
        auto next = std::next(found_iter);
        chain_info.first = next;
    }
    data.erase(found_iter);
    chain_info.second--;
    return true;
}

template<class key,class value,class hash>
double unordered_map<key,value,hash>::load_factor() const{
    return static_cast<double>(data.size()) / hash_table.size();
}

template<class key,class value,class hash>
void unordered_map<key,value,hash>::rehash(size_t new_capacity){
    std::vector<std::pair<key,value>> old_data(new_capacity);
    for(auto it = data.begin();it!=data.end();it++)
        old_data.push_back(*it);
    
    data.clear();
    hash_table.clear();
    for(int i = 0;i<old_data.size();i++)
        insert(old_data[i].first,old_data[i].second);
}

template<class key,class value,class hash>
bool unordered_map<key,value,hash>::erase(const const_iterator it){
    if(it == c_end() || hash_table.empty())
        return false;
    return erase(it->first);
}

template<class key,class value,class hash>
bool unordered_map<key,value,hash>::empty() const{
    return data.empty();
}

template<class key,class value,class hash>
size_t unordered_map<key,value,hash>::size() const{
    return data.size();
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::c_begin() const{
    return const_iterator(data.begin());
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::c_end() const{
    return const_iterator(data.end());
}
