#ifndef _MINHEAP_HPP
#define _MINHEAP_HPP

#include <iostream>

#include "vector.hpp"

template <typename T>
void swap(T* a,T* b){
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class T>
class MinHeap{
public:
    typedef T value_type;
    typedef value_type &reference_type;
    typedef const value_type  &const_reference_type;
    typedef value_type *pointer_type;
    typedef const value_type  *const_pointer_type;
private:
    int size;
    Vector<T> vec;
    int parent(int x) { return (int)((x-1)/2); }
    int left(int x) { return (x*2) + 1; }
    int right(int x) { return (x*2) + 2; }
public:
    MinHeap();
    void insert(value_type x);
    bool is_empty()const;
    int get_size()const;
    value_type get_top()const; //Top is index 0.
    void remove_top();
    void print_list()const;
    Vector<T> get_vector()const;
    template <class U>
    friend std::ostream& operator<< (std::ostream& out, const MinHeap<U>& mh);

    ~MinHeap();
};

template <class T>
void heap_sort(Vector<T>* vec){
    int cur, size = vec->size(), end, l, r;
    while(size > 1){
        // std::cout<<"Hp"<<size<<":"<<*(*vec)[0]<<"  ";
        end = size-1;
        swap(&(*vec)[0], &(*vec)[end]);
        size--;
        end = size-1;
        cur = 0;
        while((1+2*cur) <= (end)){
            l = (1+2*cur);
            r = (2+2*cur);
            if((*((*vec)[l]) < *((*vec)[r])) || (r > end)){
                if(*((*vec)[l]) < *((*vec)[cur])){
                    swap(&(*vec)[cur], &(*vec)[l]);
                    cur = l;
                }else{
                    break;
                }
            }else{
                if(*((*vec)[r]) < *((*vec)[cur])){
                    swap(&(*vec)[cur], &(*vec)[r]);
                    cur = r;
                }else{
                    break;
                }
            }
        }
    }
}

template <class T>
Vector<T> MinHeap<T>::get_vector()const{
    return this->vec;
}

template <class T>
std::ostream& operator<< (std::ostream& out, const MinHeap<T>& mh){
    Vector<T> vec = mh.get_vector();
    heap_sort(&vec);
    for(int i=0; i<vec.size();i++){
        out <<" ";
        out<< *(vec[i]);
    }
    return out;
}

template <class T>
MinHeap<T>::MinHeap(): size(0){}

template <class T>
void MinHeap<T>::insert(value_type x){
    int cur = this->size;
    //std::cout<<"p:"<<parent(cur)<<" "<<cur<<"\n"; //DEBUG
    vec.push_back(x);
    while(cur > 0){
        if(*(vec[parent(cur)]) > *(x)){
            vec[cur] = vec[parent(cur)];
            cur = parent(cur);
        }else{
            break;
        }
    }
    vec[cur] = x; 
    this->size++;
}

template <class T>
bool MinHeap<T>::is_empty()const{
    return this->get_size() == 0;
}
template <class T>
int MinHeap<T>::get_size()const{
    return this->size;
}

template <class T>
T MinHeap<T>::get_top()const{
    if(this->get_size()>0) {
        return this->vec[0];
    }else{ 
        return NULL;
    }
}

template <class T>
void MinHeap<T>::remove_top(){
    int size = this->get_size(), last, cur, l, r;
    if(size==0)
        return;
    if(size==1){
        this->size--;
        this->vec.pop_back();
    }else{
        cur = 0;
        last = size-1;
        this->vec[0] = this->vec[last];
        last = size-2; //because pop-back the last part and size not changed
        while(left(cur) <= last){
            l = left(cur);
            r = right(cur);
            if(r > last){
                if(*(this->vec[l]) < *(this->vec[cur])){
                    swap(&this->vec[cur], &this->vec[l]);
                }
                break;
            }else if(*(this->vec[l]) < *(this->vec[r])){
                if(*(this->vec[l]) < *(this->vec[cur])){
                    swap(&this->vec[cur], &this->vec[l]);
                    cur = l;
                }else{
                    break;
                }
            }else{
                if(*(this->vec[r]) < *(this->vec[cur])){
                    swap(&this->vec[cur], &this->vec[r]);
                    cur = r;
                }else{
                    break;
                }
            }
        }
        this->size--;
        this->vec.pop_back();
    }
}

template <class T>
void MinHeap<T>::print_list()const{
    for(int i=0;i<size;i++){
        std::cout<<" "<<*(vec[i]);
    }
    std::cout<<"\n";
}


template <class T>
MinHeap<T>::~MinHeap(){
}

#endif