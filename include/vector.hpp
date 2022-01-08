#ifndef _VECTOR_HPP
#define _VECTOR_HPP

template <typename T> 
class Vector{
public:
    typedef T value_type;
    typedef value_type &reference_type;
    typedef const value_type &const_reference_type;
    typedef value_type *pointer_type;
    typedef const value_type *const_pointer_type;
private:
	pointer_type vec;	// the array
	int cap;	// cap of array
	int last;	// size of array/last element index
public:
	Vector();
	Vector(int);

	template <typename U> 
	friend std::ostream& operator<< (std::ostream& out, const Vector<U>& v);

	int capacity() const;
	int size() const;

	inline reference_type operator[](const int pos)
	{
		return this->vec[pos];
	}
	inline const_reference_type operator[](const int pos) const
	{
		return this->vec[pos];
	}

	Vector<T>& operator= (const Vector<T>& vector);

	void pop_back();
	void push_back(value_type);
	// void insert(int, int count, value_type);
	// void insert(int, value_type);
	void reserve(int);
	void resize(int);
	
	~Vector();
};

template <typename T> 
std::ostream& operator<< (std::ostream& out, const Vector<T>& v){
    for(int i=0; i<v.size(); i++){
		if(i != 0) out<< ' ';
		out << v[i];
	}
	return out;
}

template <typename T> 
Vector<T>::Vector(){
	this->vec = new value_type[1];
	this->cap = 1;
	this->last = 0;
}

template <typename T> 
Vector<T>::Vector(int size){
	this->vec = new value_type[size+1];
	this->cap = size+1;
	for(int i=0; i<size; i++){
		this->vec[i] = NULL;
	}
	this->last = 0;
}

template <typename T> 
Vector<T>& Vector<T>::operator= (const Vector<T>& vector){
	int old_capacity = this->capacity();
	int new_size = vector.size();
	if (new_size > old_capacity) {
		if (old_capacity + (old_capacity / 2) > new_size)
			reserve(old_capacity+(old_capacity/2));
		else reserve(new_size);
	}
	resize(new_size);
	// *IMPORTANT*: this->size() is the new size.
	for (int i = 0; i<this->size(); i++) {
		this->vec[i] = vector[i];
	}
    return *this;
}

template <typename T> 
int Vector<T>::capacity() const {
	return this->cap;
}

template <typename T> 
int Vector<T>::size() const {
	return this->last;
}

template <typename T> 
void Vector<T>::pop_back() {
	if (this->size() > 0) this->vec[(--this->last)] = NULL;
}

template <typename T> 
void Vector<T>::push_back(value_type val) {
	int old_capacity = this->capacity();
	int new_capacity;
	if (this->size() == old_capacity) {
		if (old_capacity + 1 > old_capacity * 3) 
			reserve(old_capacity + 1);
		else reserve(old_capacity * 3);
		push_back(val);
	}
	else {
		this->vec[this->last] = val;
		this->last++;
	}
}

// template <typename T> 
// void Vector<T>::insert(int pos, int count, value_type val) {
// 	int old_capacity = this->capacity();
// 	int old_size = this->size();
// 	if (old_size + count > old_capacity) {
// 		if (old_capacity + (old_capacity / 2) > old_capacity +count)
// 			reserve(old_capacity+(old_capacity/2));
// 		else reserve(old_capacity+count);
// 	}
// 	resize(old_size + count);
// 	// *IMPORTANT*: this->size() is the new size.
// 	for (int i = pos+count; i<this->size(); i++) {
// 		this->vec[i] = this->vec[i - count];
// 	}
// 	for (int i = pos; i<pos+count; i++) {
// 		this->vec[i] = val;
// 	}
// }

// template <typename T> 
// void Vector<T>::insert(int pos, value_type val) {
// 	int old_capacity = this->capacity();
// 	int old_size = this->size();
// 	if (old_size + 1 > old_capacity) {
// 		if (old_capacity + (old_capacity / 2) > old_capacity +1)
// 			reserve(old_capacity+(old_capacity/2));
// 		else reserve(old_capacity+1);
// 	}
// 	resize(old_size + 1);
// 	// Note that this->last here is the new size.
// 	for (int i = pos+1; i < this->last; i++) {
// 		this->vec[i] = this->vec[i - 1];
// 	}
// 	for (int i = pos; i < pos+1; i++) {
// 		this->vec[i] = val;
// 	}
// }

template <typename T> 
void Vector<T>::reserve(int new_capacity) {
	int old_capacity = this->capacity();
	int old_size = this->size();
	if (new_capacity > old_capacity) {
		// Create a new array with the increased capacity.
		pointer_type new_vec = new value_type[new_capacity];
		// Copy the old array to the new array.
		for (int i = 0;i < old_size;i++) {
			new_vec[i] = vec[i];
		}
		// Delete the original and maintain the end and the last pointers.
		delete [] vec;
		vec = new_vec;
		this->cap = new_capacity;
		this->last = old_size;
	}
}

template <typename T> 
void Vector<T>::resize(int new_size) {
	int old_size = this->size();
	if (new_size < old_size) {
		for (int i = old_size;i > new_size;i--) {
			pop_back();
		}
	}
	else if (new_size > old_size) {
		if (new_size > this->capacity()) {
			reserve(new_size);
		}
		for (int i = old_size;i < new_size;i++) push_back(NULL);
	}
}

template <typename T> 
Vector<T>::~Vector() { // Destructor
	delete [] vec;
}

#endif

