#pragma once
#include <iostream>

namespace CONSTANTS{
constexpr size_t GROWHT_FACTOR = 2;
constexpr size_t INITIAL_CAPACITY = 8;
}

template<class value,class allocator = std::allocator<value>>
class vector{
private:
    value* data = nullptr;
    size_t size;
    size_t capacity;
    allocator allocat;
    
    void copy(const vector& other);
    void move(vector&& other);
    void destroy();
    
    class iterator{
    private:
        value* mem_pointer;
        
    public:
        iterator(value* mem_pointer) : mem_pointer(mem_pointer){};
        iterator(value* mem_pointer, size_t push) : mem_pointer({mem_pointer + push}){}
        iterator& operator++(){
            if(mem_pointer)
                mem_pointer++;
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            ++(*this);
            return copy;
        }
        
        iterator& operator--(){
            if(mem_pointer)
                mem_pointer--;
            return *this;
        }
        iterator operator--(int){
            iterator copy(*this);
            --(*this);
            return copy;
        }
        iterator operator+(size_t offset){
            if(mem_pointer)
                return {mem_pointer + offset};
        }
        
        int operator+(iterator& it){
            return mem_pointer + it.mem_pointer;
        }
        
        iterator operator-(size_t offset){
            if(mem_pointer)
                return {mem_pointer - offset};
        }
        
        int operator-(iterator& it){
            return mem_pointer - it.mem_pointer;
        }
        
        value* operator->(){
            return mem_pointer;
        }
        value& operator*(){
            return *mem_pointer;
        }
        
        bool operator==(const iterator& other) const{
            return mem_pointer == other.mem_pointer;
        }
        bool operator!=(const iterator& other) const{
            return !(*this == other);
        }
    };
    
    class const_iterator{
    private:
        const value* mem_pointer;
        
    public:
        const_iterator(const value* mem_pointer) : mem_pointer(mem_pointer){}
        const_iterator& operator++(){
            if(mem_pointer)
                mem_pointer++;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_iterator& operator--(){
            if(mem_pointer)
                mem_pointer--;
            return *this;
        }
        const_iterator operator--(int){
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        const_iterator operator+(size_t offset){
            return {mem_pointer + offset};
        }
        int operator+(const const_iterator& it){
            return mem_pointer + it.mem_pointer;
        }
        const_iterator operator-(size_t offset){
            return {mem_pointer - offset};
        }
        int operator-(const const_iterator& it){
            return mem_pointer - it.mem_pointer;
        }
        const value& operator*() const{
            return *mem_pointer;
        }
        const value* operator->() const{
            return mem_pointer;
        }
        bool operator==(const const_iterator& other) const{
            return mem_pointer == other.mem_pointer;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
    class reverse_iterator{
    private:
        value* mem_pointer;
        
    public:
        reverse_iterator(value* mem_pointer) : mem_pointer(mem_pointer){}
        reverse_iterator(value* mem_pointer,size_t offset) : mem_pointer({mem_pointer + offset}){}
        reverse_iterator& operator++(){
            if(mem_pointer)
                mem_pointer--;
            return *this;
        }
        reverse_iterator operator++(int){
            reverse_iterator copy(*this);
            ++(*this);
            return copy;
        }
        reverse_iterator& operator--(){
            if(mem_pointer)
                mem_pointer++;
            return *this;
        }
        reverse_iterator operator--(int){
            reverse_iterator copy(*this);
            --(*this);
            return copy;
        }
        
        reverse_iterator operator+(size_t offset){
            return {mem_pointer - offset};
        }
        reverse_iterator operator-(size_t offset){
            return {mem_pointer + offset};
        }
        value* operator->(){
            return mem_pointer;
        }
        value& operator*(){
            return *mem_pointer;
        }
        bool operator==(const reverse_iterator& other) const{
            return mem_pointer == other.mem_pointer;
        }
        bool operator!=(const reverse_iterator& other) const{
            return !(*this == other);
        }
    };
    
    class const_reverse_iterator{
    private:
       const value* mem_pointer;
        
    public:
        const_reverse_iterator(const value* mem_pointer) : mem_pointer(mem_pointer){}
        const_reverse_iterator(const value* mem_pointer,size_t offset) : mem_pointer({mem_pointer + offset}){};
        const_reverse_iterator& operator++(){
            if(mem_pointer)
                mem_pointer--;
            return *this;
        }
        const_reverse_iterator operator++(int){
            const_reverse_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_reverse_iterator& operator--(){
            if(mem_pointer)
                mem_pointer++;
            return *this;
        }
        const_reverse_iterator operator--(int){
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        const_reverse_iterator operator+(size_t offset) const{
            return {mem_pointer - offset};
        }
        const_reverse_iterator operator-(size_t offset) const{
            return {mem_pointer + offset};
        }
        const value* operator->() const{
            return mem_pointer;
        }
        const value& operator*() const{
            return *mem_pointer;
        }
        
        bool operator==(const const_reverse_iterator& other) const{
            return other.mem_pointer == mem_pointer;
        }
        bool operator!=(const const_reverse_iterator& other) const{
            return !(*this == other);
        }
        
        
    };
    
public:
    vector();
    vector(size_t capacity);
    vector(size_t capacity,const value& initial);
    vector(const vector& other);
    vector(vector&& other);
    vector& operator=(const vector& other);
    vector& operator=(vector&& other);
    ~vector();
    
    const value& operator[](size_t index) const;
    value& operator[](size_t index);
    const value& front() const;
    value& front();
    const value& back() const;
    value& back();
    
    void push_back(const value& val);
    void pop_back();
    void erase(const_iterator begin,const_iterator end);
    void erase(const_iterator postion);
    bool is_empty() const;
    size_t get_size() const;
    void print() const;
    iterator begin(){
        return iterator(data);
    }
    iterator end(){
        return iterator(&data[size]);
    }
    const_iterator c_begin() const{
        return const_iterator(data);
    }
    const_iterator c_end() const{
        return const_iterator(&data[size]);
    }
    reverse_iterator r_begin(){
        return reverse_iterator(&data[size - 1]);
    }
    reverse_iterator r_end(){
        return reverse_iterator(data - 1);
    }
    
    void resize(size_t n);
    void reserve(size_t new_capacity);
    void shrink_to_fit();
};

template<class value,class allocator>
vector<value,allocator>::vector() : data(nullptr),size(0),capacity(CONSTANTS::INITIAL_CAPACITY){
    reserve(CONSTANTS::INITIAL_CAPACITY);
}

template<class value,class allocator>
vector<value,allocator>::vector(const vector& other){
    copy(other);
}

template<class value,class allocator>
vector<value,allocator>::vector(vector&& other){
    move(std::move(other));
}

template<class value,class allocator>
vector<value,allocator>::vector(size_t initial_capacity,const value& initial){
    data = allocat.allocate(initial_capacity);
    for(int i = 0;i<initial_capacity;i++)
        std::allocator_traits<allocator>::construct(allocat,&data[i],initial);
    this->size = 0;
    this->capacity = initial_capacity;
}


template<class value,class allocator>
vector<value,allocator>::vector(size_t initial_capacity){
    data = allocat.allocate(initial_capacity);
    for(int i = 0;i<initial_capacity;i++)
        std::allocator_traits<allocator>::construct(allocat,&data[i]);
    this->size = 0;
    this->capacity = initial_capacity;
}

template<class value,class allocator>
vector<value,allocator>& vector<value,allocator>::operator=(const vector& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value,class allocator>
vector<value,allocator>& vector<value,allocator>::operator=(vector&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value,class allocator>
void vector<value,allocator>::copy(const vector& other){
    data = allocat.allocate(other.capacity);
    for(int i = 0;i<other.size;i++)
        std::allocator_traits<allocator>::construct(allocat,&data[i],other.data[i]);
    this->size = other.size;
    this->capacity = other.capacity;
}

template<class value,class allocator>
void vector<value,allocator>::move(vector&& other){
    this->data = other.data;
    this->size = other.size;
    this->capacity = other.capacity;
    this->allocat = other.allocat;
    
    other.data = nullptr;
    other.size = other.capacity = 0;
}

template<class value,class allocator>
void vector<value,allocator>::destroy(){
    for(int i = 0;i<size;i++)
        std::allocator_traits<allocator>::destroy(allocat,&data[i]);
    allocat.deallocate(data,capacity);
}

template<class value,class allocator>
vector<value,allocator>::~vector(){
    destroy();
}

template<class value,class allocator>
void vector<value,allocator>::push_back(const value& val){
    if(size == capacity){
        size_t new_capacity = (capacity == 0) ? 1 : CONSTANTS::GROWHT_FACTOR * capacity;
        reserve(new_capacity);
    }
    std::allocator_traits<allocator>::construct(allocat,&data[size++],val);
}

template<class value,class allocator>
void vector<value,allocator>::erase(const_iterator begin, const_iterator end){
    int deleted_count = end - begin;
    if(deleted_count <= 0)
        return;
    int begin_offset = begin - c_begin();
    int end_offset = end - c_begin();
    if(end != c_end()){
        for(int i = end_offset;i<size;i++)
            data[begin_offset++] = std::move(data[i]);
    }
    for(int i = size - deleted_count;i<size;i++)
        std::allocator_traits<allocator>::destroy(allocat,&data[i]);
    
    size-=deleted_count;
}

template<class value,class allocator>
void vector<value,allocator>::erase(const_iterator position){
    if(position == c_end())
        return;
    erase(position, position + 1);
}

template<class value,class allocator>
void vector<value,allocator>::resize(size_t n){
    if(n < size){
        for(int i = n;i<size;i++)
            std::allocator_traits<allocator>::destroy(allocat,&data[i]);
        size = n;
    }
    else{
        if(n <= capacity){
            for(int i = size;i<capacity;i++)
                std::allocator_traits<allocator>::construct(&data[i]);
            size = n;
        }
        else{
            value* new_data = allocat.allocate(n);
            for(int i = 0;i<size;i++)
                std::allocator_traits<allocator>::construct(allocat,&new_data[i],std::move(data[i]));
            for(int i = n;i<size;i++)
                std::allocator_traits<allocator>::construct(allocat,&new_data[i]);
            allocat.deallocate(data,capacity);
            data = new_data;
            capacity = n;
        }
    }
}

template<class value,class allocator>
void vector<value,allocator>::reserve(size_t new_capacity){
    value* new_data = allocat.allocate(new_capacity);
    for(int i = 0;i<size;i++)
        std::allocator_traits<allocator>::construct(allocat,&new_data[i],std::move(data[i]));
    
    allocat.deallocate(data,capacity);
    data = new_data;
    capacity = new_capacity;
}

template<class value,class allocator>
void vector<value,allocator>::shrink_to_fit(){
    if(size == capacity)
        return;
    value* new_data = allocat.allocate(size);
    for(int i = 0;i<size;i++)
        std::allocator_traits<allocator>::construct(allocat,&new_data[i],std::move(data[i]));
    allocat.deallocate(data,capacity);
    data = new_data;
    capacity = size;
}

template<class value,class allocator>
const value& vector<value,allocator>::front() const{
    if(is_empty())
        throw std::invalid_argument("invalid!");
    return data[0];
}


template<class value,class allocator>
const value& vector<value,allocator>::back() const{
    if(is_empty())
        throw std::invalid_argument("invalid!");
    return data[size - 1];
}

template<class value,class allocator>
value& vector<value,allocator>::front(){
    if(is_empty())
        throw std::invalid_argument("invalid!");
    return data[0];
}

template<class value,class allocator>
value& vector<value,allocator>::back(){
    if(is_empty())
        throw std::invalid_argument("invalid!");
    return data[size - 1];
}

template<class value,class allocator>
const value& vector<value,allocator>::operator[](size_t index) const{
    if(index >= size)
        throw std::invalid_argument("invalid index");
    return data[index];
}

template<class value,class allocator>
value& vector<value,allocator>::operator[](size_t index){
    if(index >= size)
        throw std::invalid_argument("invalid index");
    return data[index];
}

template<class value,class allocator>
void vector<value,allocator>::print() const{
    for(int i = 0;i<size;i++)
        std::cout<<data[i]<<" ";
}

template<class value,class allocator>
bool vector<value,allocator>::is_empty() const{
    return size == 0;
}

template<class value,class allocator>
size_t vector<value,allocator>::get_size() const{
    return size;
}

template<class value,class allocator>
void vector<value,allocator>::pop_back(){
    if(size == 0)
        return;
    erase(--c_end());
}


