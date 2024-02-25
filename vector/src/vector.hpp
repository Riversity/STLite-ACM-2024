#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
auto resize_func = [](size_t s){return s / 2 * 3 + 1;};
template<typename T>
class vector 
{
private:
	size_t size, capacity;
	T* data;

public:
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		vector* origin;
		int pos;
	public:
		iterator(vector* _origin = nullptr, int _pos = 0)
						: origin(_origin), pos(_pos) {}
		~iterator() = default;
		iterator(iterator &other) {
			origin = other.origin;
			pos = other.pos;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const 
		{
			return iterator(origin, pos + n);
		}
		iterator operator-(const int &n) const 
		{
			return iterator(origin, pos - n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			if(origin != rhs.origin) throw invalid_iterator();
			else return pos - rhs.pos;
		}
		iterator& operator+=(const int &n) 
		{
			pos += n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			pos -= n;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp(origin, pos + 1);
			return tmp;
		}
		iterator& operator++() {
			++pos;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp(origin, pos - 1);
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return origin->data[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {}
		bool operator==(const const_iterator &rhs) const {}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {}
		bool operator!=(const const_iterator &rhs) const {}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/

	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {}
	vector(const vector &other) {}
	/**
	 * TODO Destructor
	 */
	~vector() {}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	void expand() {} // Move the whole chunk to a bigger residence
	T & at(const size_t &pos) {}
	const T & at(const size_t &pos) const {}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {}
	const T & operator[](const size_t &pos) const {}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {}
	/**
	 * returns the number of elements
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	void clear() {}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {}
};


}

#endif
