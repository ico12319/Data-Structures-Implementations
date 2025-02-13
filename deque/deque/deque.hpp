#pragma once
#include <iostream>

namespace CONSTANTS {
constexpr size_t INITIAL_CAPACITY = 8;
constexpr size_t GROWTH_FACTOR = 2;
}

template<class value,class allocator = std::allocator<value>>
class deque{
private:
    value* data;
    size_t size;
    allocator allocat;
    size_t head;
    size_t tail;
    size_t capacity;
    
    void resize(size_t new_capacity);
    void move(deque&& other);
    void copy(const deque& other);
    void destroy();
    
    void move_index(size_t& index,bool forward);
    
    class iterator{
    private:
        deque& d;
        int index;
        friend class deque;
        
    public:
        iterator(deque& d,int index) : d(d),index(index){}
        value& operator*(){
            return d[index];
        }
        iterator& operator++(){
            ++index;
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            ++(*this);
            return copy;
        }
        iterator& operator--(){
            --index;
            return *this;
        }
        iterator operator--(int){
            iterator copy(*this);
            --(*this);
            return copy;
        }
        iterator operator+(int offset){
            return iterator(d, index + offset);
        }
        iterator operator-(int offset){
            return iterator(d, index - offset);
        }
        bool operator==(const iterator& other) const{
            return index == other.index;
        }
        bool operator!=(const iterator& other) const{
            return !(*this == other);
        }
    };
    
    class const_iterator{
    private:
        int index;
        const deque& d;
        friend class deque;
        
    public:
        const_iterator(const deque& d,int index) : d(d),index(index){}
        const_iterator& operator++(){
            ++index;
            return *this;
        }
        const_iterator operator++(int){
            const_iterator copy(*this);
            ++(*this);
            return copy;
        }
        const_iterator& operator--(){
            --index;
            return *this;
        }
        const_iterator operator--(int){
            const_iterator copy(*this);
            --(*this);
            return copy;
        }
        const value& operator*() const{
            return d[index];
        }
        const_iterator operator+(int offset) const{
            return const_iterator(d, index + offset);
        }
        const_iterator operator-(int offset) const{
            return const_iterator(d, index - offset);
        }
        bool operator==(const const_iterator& other) const{
            return index == other.index;
        }
        bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }
    };
    
public:
    deque();
    deque(const deque& other);
    deque(deque&& other);
    ~deque();
    deque& operator=(const deque& other);
    deque& operator=(deque&& other);
    
    void push_back(const value& val);
    void push_back(value&& other);
    
    void push_front(const value& val);
    void push_front(value&& other);
    
    void pop_back();
    void pop_front();
    
    const value& back() const;
    const value& front() const;
    
    value& back();
    value& front();
    
    const value& operator[](size_t index) const;
    value& operator[](size_t index);
    
    bool empty() const;
    size_t get_size() const;
    
    iterator begin();
    iterator end();
    const_iterator c_begin() const;
    const_iterator c_end() const;
};

template<class value,class allocator>
deque<value,allocator>::deque() : size(0),capacity(CONSTANTS::INITIAL_CAPACITY),head(0),tail(0){
    data = allocat.allocate(capacity);
}

template<class value,class allocator>
void deque<value,allocator>::push_back(const value& val){
    if(size >= capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    std::allocator_traits<allocator>::cosntruct(allocat,&data[tail],val);
    move_index(tail, true);
    size++;
}


template<class value,class allocator>
void deque<value,allocator>::move_index(size_t& index,bool forward){
    if(capacity == 0)
        throw std::invalid_argument("Empty!");
    
    if(forward)
        (++index) %= capacity;
    else
        index = (index == 0) ? capacity - 1 : index - 1;
}

template<class value,class allocator>
void deque<value,allocator>::push_back(value&& val){
    if(size >= capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    std::allocator_traits<allocator>::construct(allocat,&data[tail],std::move(val));
    move_index(tail, true);
    size++;
}

template<class value,class allocator>
void deque<value,allocator>::push_front(const value& val){
    if(size >= capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    move_index(head, false);
    std::allocator_traits<allocator>::construct(allocat,&data[head],val);
    size++;
}

template<class value,class allocator>
void deque<value,allocator>::push_front(value&& val){
    if(size >= capacity)
        resize(capacity * CONSTANTS::GROWTH_FACTOR);
    move_index(head, false);
    std::allocator_traits<allocator>::construct(allocat,&data[head],val);
    size++;
}

template<class value,class allocator>
const value& deque<value,allocator>::back() const{
    return tail == 0 ? data[capacity - 1] : data[tail - 1];
}

template<class value,class allocator>
value& deque<value,allocator>::back(){
    return tail == 0 ? data[capacity - 1] : data[tail - 1];
}


template<class value,class allocator>
const value& deque<value,allocator>::front() const{
    return data[head];
}

template<class value,class allocator>
value& deque<value,allocator>::front(){
    return data[head];
}


template<class value,class allocator>
const value& deque<value,allocator>::operator[](size_t index) const{
    return data[(head + index) % capacity];
}

template<class value,class allocator>
value& deque<value,allocator>::operator[](size_t index){
    return data[(head + index) % capacity];
}

template<class value,class allocator>
void deque<value,allocator>::move(deque&& other){
    this->head = other.head;
    this->tail = other.tail;
    this->size = other.size;
    this->capacity = other.capacity;
    this->data = other.data;
    
    other.data = nullptr;
    other.head = other.tail = other.size = other.capacity = 0;
}

template<class value,class allocator>
void deque<value,allocator>::copy(const deque& other){
    this->data = allocat.allocate(other.capacity);
    for(int i = 0;i<other.size;i++)
        push_back(other[i]);
    this->size = other.size;
    this->capacity = other.capacity;
    this->tail = other.tail;
    this->head = other.head;
}

template<class value,class allocator>
void deque<value,allocator>::destroy(){
    if(head == tail && !empty()){
        std::allocator_traits<allocator>::destroy(allocat,&data[head]);
        move_index(head, true);
    }
    while(head != tail){
        std::allocator_traits<allocator>::destroy(allocat,&data[head]);
        move_index(head, true);
    }
    allocat.deallocate(data,capacity);
}

template<class value,class allocator>
deque<value,allocator>::deque(const deque& other){
    copy(other);
}

template<class value,class allocator>
deque<value,allocator>::deque(deque&& other){
    move(std::move(other));
}

template<class value,class allocator>
deque<value,allocator>& deque<value,allocator>::operator=(const deque& other){
    if(this != &other){
        destroy();
        copy(other);
    }
    return *this;
}

template<class value,class allocator>
deque<value,allocator>& deque<value,allocator>::operator=(deque&& other){
    if(this != &other){
        destroy();
        move(std::move(other));
    }
    return *this;
}

template<class value,class allocator>
deque<value,allocator>::~deque(){
    destroy();
}

template<class value,class allocator>
void deque<value,allocator>::pop_back(){
    if(empty())
        throw std::invalid_argument("Empty!");
    move_index(tail, false);
    std::allocator_traits<allocator>::destroy(allocat,&data[tail]);
    size--;
}

template<class value,class allocator>
void deque<value,allocator>::pop_front(){
    if(empty())
        throw std::invalid_argument("Empty!");
    std::allocator_traits<allocator>::destroy(allocat,&data[head]);
    move_index(head, true);
    size--;
}

template<class value,class allocator>
void deque<value,allocator>::resize(size_t new_capacity){
    if(new_capacity <= 0)
        return;
    value* new_data = allocat.allocate(new_capacity);
    auto included_elements = std::min(size,new_capacity);
    for(int i = 0;i<included_elements;i++){
        std::allocator_traits<allocator>::construct(allocat,&new_data[i],std::move(operator[](i)));
        std::allocator_traits<allocator>::destroy(allocat,&data[(head + i) % capacity]);
    }
    
    allocat.deallocate(data,capacity);
    data = new_data;
    capacity = new_capacity;
}

template<class value,class allocator>
class deque<value,allocator>::iterator deque<value,allocator>::begin(){
    return iterator(*this, 0);
}

template<class value,class allocator>
class deque<value,allocator>::iterator deque<value,allocator>::end(){
    return iterator(*this, get_size());
}

template<class value,class allocator>
class deque<value,allocator>::const_iterator deque<value,allocator>::c_begin() const{
    return const_iterator(*this, 0);
}

template<class value,class allocator>
class deque<value,allocator>::const_iterator deque<value,allocator>::c_end() const{
    return const_iterator(*this, get_size());
}

template<class value,class allocator>
bool deque<value,allocator>::empty() const{
    return get_size() == 0;
}

template<class value,class allocator>
size_t deque<value,allocator>::get_size() const{
    return size;
}


