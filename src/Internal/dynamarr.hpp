#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include "stm32f10x.h"
#include <stdlib.h>

/*
 * Class DynamicArray
 * A dynamic array class.
 * T - The type of elements in this array
 * IncreaseAmount - The amount of elements by which to increase the size every time the array is filled
 */
template <typename T, uint16_t IncreaseAmount = 1>
class DynamicArray {
public:
	// Creates a new DynamicArray with a default length of 0
	DynamicArray() : contents((T*)malloc(0)), len(0), maxLen(0) {}
	// Creates a new DynamicArray with a starting maximum length
	DynamicArray(uint16_t initialCapacity) : len(0), maxLen(initialCapacity) {
		// Make sure the length is multipled by the size of T
		contents = (T*)malloc(sizeof(T) * initialCapacity);
	}
	// Copy constructor
	DynamicArray(const DynamicArray &other) : len(other.len), maxLen(other.maxLen) {
		contents = (T*)malloc(sizeof(T) * maxLen);

		for (uint16_t i = 0; i < len; i++) {
			contents[i] = other.contents[i];
		}
	}
	typedef T* iterator;
	typedef const T* const_iterator;
	// Iterator constructor
	DynamicArray(const_iterator start, const_iterator fin) : len(fin - start), maxLen(fin - start) {
		contents = (T*)malloc(sizeof(T) * maxLen);

		for (iterator i = begin(); i != end(); i++) {
			// Go through every elem and initialize its value
			*i = *(start++);
		}
	}
	// Array constructor from an array and size
	DynamicArray(const T *arr, uint16_t len) : len(len), maxLen(len) {
		contents = (T*)malloc(sizeof(T) * maxLen);

		for (uint16_t i = 0; i < len; i++) {
			contents[i] = arr[i];
		}
	}
	~DynamicArray() {
		free(contents);
	}

	// Retrieves the length of this DynamicArray
	uint16_t length() const {
		return len;
	}
	// Retrieves the maximum length of this DynamicArray (the length it can be before having to reallocate)
	uint16_t maxLength() const {
		return maxLen;
	}
	// Resizes the maximum length
	bool resize(uint16_t newSize) {
		uint16_t oldSize = maxLen;
		// Ignore if the new size is less than the length
		if (newSize < len) {
			return true;
		}
		// Otherwise reallocate memory
		maxLen = newSize;
		// Make sure the length is multipled by the size of T
		void *tmp = realloc(contents, sizeof(T) * newSize);
		// Oh crap we ran out of memory
		if (!tmp) {
			// Reset max len
			maxLen = oldSize;
			return false;
		}
		contents = (T*)tmp;
		return true;
	}
	// Reallocates the contents so that all extra space is freed
	void minimize() {
		resize(len);
	}
	
	bool add(const T &elem) {
		len++;
		// If the new length is more than what we can store then reallocate
		if (len > maxLen) {
			// Minus one because len is already increased
			if (!resize(len + IncreaseAmount - 1)) {
				return false;
			}
		}
		contents[len - 1] = elem;
		return true;
	}
	bool insert(const T &elem, uint16_t where) {
		len++;
		if (len > maxLen) {
			if (!resize(len + IncreaseAmount - 1)) {
				return false;
			}
		}
		// Iterate backwards to move the elements
		// This way we don't have to keep a buffer
		for (uint16_t i = len - 1; i > where; i--) {
			contents[i] = contents[i - 1];
		}
		contents[where] = elem;
		return true;
	}
	void removeAt(uint16_t where, uint16_t howMany = 1) {
		// Ignore if out of bounds
		if (where + howMany - 1 >= len) {
			return;
		}
		len -= howMany;
		// Shift all elements after the index back
		for (uint16_t i = where - 1 + howMany; i < len; i++) {
			contents[i] = contents[i + howMany];
		}
	}
	T pop() {
		// Simply decrement the length, no need to waste time clearing out the memory
		if(len > 0) {
			len--;
		}
		return contents[len];
	}
	void empty() {
		len = 0;
	}
	template <uint16_t Increase>
	bool merge(const DynamicArray<T, Increase> *other) {
		// Expand memory and stuff
		len += other->length();
		if (len > maxLen) {
			uint16_t old = maxLen;
			maxLen = len;
			void *tmp = realloc(contents, sizeof(T) * maxLen);
			if (!tmp) {
				len -= other->length();
				maxLen = old;
				return false;
			}
			contents = (T*)tmp;
		}
		// Iterate and copy elements
		auto itThis = begin() + len - other->length();
		for (auto itOther = other->begin(); itThis != end() && itOther != other->end(); itThis++, itOther++) {
			*itThis = *itOther;
		}
		return true;
	}

	T* asArray() {
		return contents;
	}
	const T* asArray() const {
		return contents;
	}

	// WARNING: Does not check for out of bounds!
	const T& operator[](uint16_t i) const {
		return contents[i];
	}
	T& operator[](uint16_t i) {
		return contents[i];
	}
	DynamicArray& operator+=(const T &elem) {
		add(elem);
		return *this;
	}

	iterator begin() {
		return &contents[0];
	}
	const_iterator begin() const {
		return &contents[0];
	}
	iterator end() {
		return &contents[len];
	}
	const_iterator end() const {
		return &contents[len];
	}

protected:
	T *contents;
	uint16_t len;
	uint16_t maxLen;
};

#endif
