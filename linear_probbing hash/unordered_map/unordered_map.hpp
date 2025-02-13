#pragma once
#include <iostream>

template<class key,class value,class hash = std::hash<key>>
class unordered_map{
private:
    struct data{
        std::optional<std::pair<key,value>> info;
        bool tombstone;
    };
    
    std::vector<data> table;
    size_t size;
    double max_load_factor;
    hash hasher;
    size_t probe_step;
    
    bool contains_element_at_index(size_t index) const;
    void rehash(size_t new_capacity);
    double load_factor() const;
    
    class const_iterator{
    private:
        friend class unordered_map;
        const unordered_map& context;
        size_t index;
        
    public:
        const_iterator(size_t index,const unordered_map& context) : index(index),context(context){}
        const_iterator& operator++(){
            index++;
            while(index < context.table.size() && !context.contains_element_at_index(index))
                index++;
            if(index > context.table.size())
                index = -1;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const value& operator*() const{
            return *context.table[index].info;
        }
        bool operator==(const const_iterator& other) const{
            return index == other.index;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };

public:
    unordered_map(size_t probe_step = 1);
    void insert(const key& _key,const value& val);
    void erase(const key& _key);
    const_iterator find(const key& _key) const;
    const_iterator c_begin() const;
    const_iterator c_end() const;
    bool empty() const;
    size_t get_size() const;
    void print() const;
};

template<class key,class value,class hash>
bool unordered_map<key,value,hash>::contains_element_at_index(size_t index) const{
    return table[index].info.has_value() && !table[index].tombstone;
}

template<class key,class value,class hash>
void unordered_map<key,value,hash>::insert(const key& _key,const value& _val){
    auto index = hasher(_key) % table.size();
    while(index < table.size() && contains_element_at_index(index)){
        if((*table[index].info).first == _key)
            return;
        (index += probe_step) %= table.size();
    }
    table[index].info = std::make_pair(_key,_val);
    table[index].tombstone = false;
    size++;
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::find(const key& _key) const{
    auto index = hasher(_key) % table.size();
    while(table[index].info.has_value()){
        if((*table[index].info).first == _key && !table[index].tombstone)
            return const_iterator(index,*this);
        (index += probe_step) %= table.size();
    }
    return c_end();
}

template<class key,class value,class hash>
void unordered_map<key,value,hash>::erase(const key& _key){
    auto index = hasher(_key) % table.size();
    while(index < table.size() && table[index].info.has_value()){
        if((*table[index].info).first == _key && !table[index].tombstone){
            table[index].tombstone = true;
            size--;
            break;
        }
        (index += probe_step) %= table.size();
    }
}

template<class key,class value,class hash>
unordered_map<key,value,hash>::unordered_map(size_t probe_step) : size(0),max_load_factor(0.75),probe_step(probe_step){
    table.resize(8);
}

template<class key,class value,class hash>
void unordered_map<key,value,hash>::rehash(size_t new_capacity){
    std::vector<data> old_data = table;
    table.clear();
    table.resize(new_capacity);
    for(auto& d : table){
        d.info.reset();
        d.tombstone = false;
    }
    size = 0;
    for(auto& d : old_data)
        insert((*d.info).first,(*d.info).second);
}

template<class key,class value,class hash>
double unordered_map<key,value,hash>::load_factor() const{
    return static_cast<double>(size) / table.size();
}

template<class key,class value,class hash>
bool unordered_map<key,value,hash>::empty() const{
    return size == 0;
}

template<class key,class value,class hash>
size_t unordered_map<key,value,hash>::get_size() const{
    return size;
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::c_begin() const{
    if(empty())
        return c_end();
    for(int i = 0;i<table.size();i++){
        if(contains_element_at_index(i))
            return const_iterator(i, *this);
    }
    return c_end();
}

template<class key,class value,class hash>
class unordered_map<key,value,hash>::const_iterator unordered_map<key,value,hash>::c_end() const{
    return const_iterator(-1,*this);
}

template<class key,class value,class hash>
void unordered_map<key,value,hash>::print() const{
    for(int i = 0;i<table.size();i++){
        if(contains_element_at_index(i))
            std::cout<<(*table[i].info).first<<" "<<(*table[i].info).second<<std::endl;
    }
}
