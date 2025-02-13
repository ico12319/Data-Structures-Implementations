#pragma once
#include <iostream>

template<class key,class hash = std::hash<key>>
class unordered_set{
private:
    struct data{
        std::optional<key> info;
        bool tombstone;
    };
    
    std::vector<data> table;
    size_t size;
    double max_load_factor = 0.75;
    hash hasher;
    size_t probe_step;
    
    bool contains_element_at_index(size_t index) const;
    void rehash(size_t new_capacity);
    double load_factor() const{
        return static_cast<double>(size) / table.size();
    }
    
    
    class const_iterator{
    private:
        size_t index;
        const unordered_set& context;
        friend class unordered_set;
    public:
        const_iterator(size_t index,const unordered_set& context) : index(index),context(context){}
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
        const key& operator*() const{
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
    unordered_set();
    void insert(const key& element);
    void erase(const key& element);
    const_iterator find(const key& element) const;
    const_iterator c_begin() const;
    const_iterator c_end() const;
    bool empty() const;
    size_t get_size() const;
    void print() const;
};

template<class key,class hash>
bool unordered_set<key,hash>::contains_element_at_index(size_t index) const{
    return table[index].info.has_value() && !table[index].tombstone;
}

template<class key,class hash>
void unordered_set<key,hash>::insert(const key& element){
    if(load_factor() >= max_load_factor)
        rehash(table.size() * 2);
    auto index = hasher(element) % table.size();
    while(contains_element_at_index(index)){
        if(*table[index].info == element && !table[index].tombstone)
            return;
        (index+=probe_step) %= table.size();
    }
    table[index].info = element;
    table[index].tombstone = false;
    size++;
}

template<class key,class hash>
class unordered_set<key,hash>::const_iterator unordered_set<key,hash>::find(const key& element) const{
    auto index = hasher(element) % table.size();
    while(table[index].info.has_value()){
        if(*table[index].info == element && !table[index].tombstone)
            return const_iterator(index, *this);
        (index+=probe_step) %= table.size();
    }
    return c_end();
}

template<class key,class hash>
void unordered_set<key,hash>::erase(const key& element){
    auto index = hasher(element) % table.size();
    while(table[index].info.has_value()){
        if(*table[index].info == element && !table[index].tombstone){
            table[index].tombstone = true;
            size--;
            break;
        }
        (index+=probe_step) %= table.size();
    }
}

template<class key,class hash>
unordered_set<key,hash>::unordered_set() : max_load_factor(0.75),size(0),probe_step(1){
    table.resize(8);
}
template<class key,class hash>
class unordered_set<key,hash>::const_iterator unordered_set<key,hash>::c_begin() const{
    if(size == 0)
        return c_end();
    for(int i = 0;i<table.size();i++){
        if(contains_element_at_index(i))
            return const_iterator(i, *this);
    }
    return c_end();
}

template<class key,class hash>
class unordered_set<key,hash>::const_iterator unordered_set<key,hash>::c_end() const{
    return const_iterator(-1, *this);
}

template<class key,class hash>
bool unordered_set<key,hash>::empty() const{
    return size == 0;
}

template<class key,class hash>
size_t unordered_set<key,hash>::get_size() const{
    return size;
}

template<class key,class hash>
void unordered_set<key,hash>::rehash(size_t new_capacity){
    std::vector<data> old_data = table;
    table.clear();
    table.resize(new_capacity);
    for(auto& d : table){
        d.info.reset();
        d.tombstone = false;
    }
    size = 0;
    for(auto& d : old_data){
        if(d.info.has_value() && !d.tombstone)
            insert(*d.info);
    }
}

template<class key,class hash>
void unordered_set<key,hash>::print() const{
    for(int i = 0;i<table.size();i++){
        if(contains_element_at_index(i))
            std::cout<<*table[i].info<<std::endl;
    }
}
