#ifndef _LIST_HPP
#define _LIST_HPP

#include <iostream>

template <class T>
class NodeList{
public:
    typedef T value_type;
    typedef value_type &reference_type;
    typedef const value_type &const_reference_type;
    typedef value_type *pointer_type;
    typedef const value_type *const_pointer_type;
private:
    value_type val;
    NodeList* next;
public:
    NodeList(): next(NULL){}
    NodeList(value_type val): val(val), next(NULL){}
    value_type get_data(){
        return this->val;
    }
    NodeList* get_next(){
        return this->next;
    }
    void set_next(NodeList* node){
        this->next = node;
    }
    void set_data(value_type val){
        this->val = val;
    }
    ~NodeList(){}
};

template <class T>
class List{
private:
    typedef NodeList<T> node_type;
    typedef node_type *node_pointer;
public:
    typedef typename node_type::value_type value_type;
    typedef typename node_type::reference_type reference_type;
    typedef typename node_type::const_reference_type const_reference_type;
private:
    node_pointer head, tail;
public:
    List();

    void print_list();
    void push_front(value_type x);
    void push_back(value_type x);
    void Delete(value_type x);
    void clear();
    void reverse();

    ~List();
};


template <class T>
List<T>::List(): head(NULL), tail(NULL){
}

template <class T>
void List<T>::print_list(){
    node_pointer cur = head;
    while(cur != NULL){
        std::cout<<cur->get_data()<<' ';
        cur = cur->get_next();
    }
}

template <class T>
void List<T>::push_front(value_type x){
    if(this->tail == NULL){
        this->head = new node_type(x);
        this->tail = this->head;
    }else{
        node_pointer tmp = new node_type(x);
        tmp->set_next(head);
        this->head = tmp;
    }
}

template <class T>
void List<T>::push_back(value_type x){
    if(this->head == NULL){
        this->head = new node_type(x);
        this->tail = this->head;
    }else{
        node_pointer tmp = new node_type(x);
        this->tail->set_next(tmp);
        this->tail = this->tail->get_next();
    }
}

template <class T>
void List<T>::Delete(value_type x){
    node_pointer cur = head, prev;
    while(cur != NULL){
        if(cur->get_data() == x) {
            if(head == cur){
                if(tail == cur) tail == NULL;
                head = cur->get_next();
                delete cur;
                cur = head;
            }else{
                if(tail == cur) tail = prev;
                prev->set_next(cur->get_next());
                delete cur;
                cur = prev->get_next();        
            }
        }else{
            prev = cur;
            cur = cur->get_next();
        }
    }
}

template <class T>
void List<T>::clear(){
    while(head != NULL){
        node_pointer tmp = head;
        head = head->get_next();
        delete tmp;
    }
}

template <class T>
void List<T>::reverse(){
    node_pointer prev = NULL, cur = this->head, next = NULL;
    this->tail = this->head;
    while(cur != NULL){
        next = cur->get_next();
        cur->set_next(prev);
        prev = cur;
        cur = next;
    }
    this->head = prev;
}

template <class T>
List<T>::~List(){
    this->clear();
}

#endif