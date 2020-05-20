#ifndef _vector_h
#define _vector_h

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <type_traits>
#include <functional>


/*!
\class MyVector myvector.h
\brief Growable, range-checked array
This class stores an ordered list of values similar to an array.
 * It supports traditional array selection using square brackets, but
 * also supports inserting and deleting elements.  It is similar in
 * function to the STL `std::vector` type, but is simpler both
 * to use and to implement.
*/
template <typename T>

class MyVector {

public:
    /*! \brief first of the three constructors, this is the default one */
    MyVector() = default;
    /*! \overload */
    explicit MyVector(int n, T value = T());
    /*! \brief separate from the pack */
    MyVector(std::initializer_list<T> list);
    virtual ~MyVector() = default;

    void add(const T& value);
    MyVector<T>& addAll(const MyVector<T>& v);
    T& front();
    const T& front() const;
    void mapAll( std::function<void (const T&)> fn) const;
    int size() const;
    void shuffle();

    T& operator [](int index);
    const T& operator [](int index) const;
    MyVector operator +(const T& elem) const;
    MyVector& operator +=(const T& value);

    /** number of vectors alive */
    static int _totalCount;

     /** which waay vector pointes */
    enum VectorDirection { North, South, East, West};

private:
    using ContainerType = typename std::conditional<std::is_same<T, bool>::value,
                                                    std::deque<bool>,
                                                    std::vector<T>>::type;
    ContainerType _elements;
    void checkIndex(int index, int min, int max, const char* prefix) const;
};

template <typename T>
void shuffle(MyVector<T>& v);

template <typename T>
std::ostream& operator <<(std::ostream& os, const MyVector<T>& vec);




static inline void error(const std::string &) {}

template <typename T>
MyVector<T>::MyVector(int n, T value) {
    if (n < 0) error("Cannot create a MyVector with a negative number of elements.");
    _elements.assign(n, value);
}

template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> list)
        : _elements(list) {
}

/*
 * Implementation notes: MyVector methods
 * ------------------------------------
 * The basic MyVector methods are straightforward and should require
 * no detailed documentation.
 */



template <typename T>
/*! add all of v to this \a v
\bigoh O(N\sup{2})*/
MyVector<T>& MyVector<T>::addAll(const MyVector<T>& v) {
    for (const T& value : v) {
        add(value);
    }
    return *this;   // BUGFIX 2014/04/27
}


template <typename T>
/*! front
\bigoh O(1)*/
T& MyVector<T>::front() {
    return const_cast<T&>(static_cast<const MyVector &>(*this).front());
}

template <typename T>
/*! front
\bigoh O(1)*/
const T& MyVector<T>::front() const {
    return _elements.front();
}



template <typename T>
/*! map \a fn over all elements */
void MyVector<T>::mapAll( std::function<void (const T&)> fn) const {
    for (const auto& elem: _elements) {
        fn(elem);
    }
}


template <typename T>
/*! access element at \a index
\bigoh O(1)*/
T& MyVector<T>::operator [](int index) {
    return const_cast<T&>(static_cast<const MyVector &>(*this)[index]);
}
template <typename T>
/*! access element at \a index
* \sample v[index]
*/
const T& MyVector<T>::operator [](int index) const {
    checkIndex(index, 0, size()-1, "operator []");
    return _elements[index];
}


template <typename T>
/*! create new vector which is sum of this and extra element
\a elem
\tparam T type of element stored in this vector
\param elem to add to vector
\bigoh O(1)
* \sample v + elem
*/
MyVector<T> MyVector<T>::operator +(const T& elem) const {
    MyVector<T> result = *this;
    return result += elem;
}

/*!
  append \a value to end of this vector
  * \sample v += elem

  \bigoh O(1)
*/
template <typename T>
MyVector<T>& MyVector<T>::operator +=(const T& value) {
    add(value);
    return *this;
}

template <typename T>
/*!
  \internal
*/
void MyVector<T>::checkIndex(int index, int min, int max, const char* prefix) const {
    if (index < min || index > max) {
        std::ostringstream out;
        out << "MyVector::" << prefix << ": index of " << index
            << " is outside of valid range ";
        out << "[";
        if (min < max) {
            out << min << ".." << max;
        } else if (min == max) {
            out << min;
        } // else min > max, no range, empty vector
        out << "]";
        error(out.str());
    }
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename T>
/*!
  \brief write to ostream

  The insertion and extraction operators use the template facilities in
    strlib.h to read and write generic values in a way that treats strings
    especially.
    writes contents of \a vec to stream \a os
    returns \a os
    \relates MyVector
    \sample cout << v
*/
std::ostream& operator <<(std::ostream& os, const MyVector<T>& v) {
    (void)v;
    return os;
}


template <typename T>
/*!
    \brief shuffle baby shuffle (member function)
    \bigoh O(N)
    */
void MyVector<T>::shuffle() {
    for (int i = 0; i < size() - 1; i++) {
        std::swap(_elements[i], _elements[randomInteger(i, size() - 1)]);
    }
}

template <typename T>
/*!
  \brief shuffle (free function)
  \relates MyVector
*/
void shuffle(MyVector<T>& v) {
    v.shuffle();
}

template <typename T>
/*! \brief add an element to end of vector */
void MyVector<T>::add(const T& ) {
    ; //insert(size(), value);
}

template <typename T>
int MyVector<T>::size() const {
    return _elements.size();
}

/*! \fn void MyVector<T>::size() const

    This is where I tried to add it after
    */

/*! \fn template <typename T> int MyVector<T>::size() const
  return number of elements (this one works for QT tool tip)
  */


/*! \fn int MyVector::size() const
  return number of elements
  */


#endif // _vector_h
