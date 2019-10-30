<html>
<head>
<title>../include/vector.h</title>
<link rel="stylesheet" type="text/css" href="cppdoc.css" />
</head>
<body>
<pre class=code>
<scan class=comment>/*
 * File: vector.h
 * --------------
 * This file exports the Vector class, which provides an efficient, safe,
 * convenient replacement for the array type in C++.
 */
</scan>
#ifndef _vector_h
#define _vector_h

#include &lt;iterator&gt;
#include &lt;iostream&gt;
#include &lt;sstream&gt;
#include &lt;string&gt;
#include "strlib.h"

<a name="Class:Vector<ValueType>"><scan class=comment>/*
 * Class: Vector&lt;ValueType&gt;
 * ------------------------
 * This class stores an ordered list of values similar to an array.  It
 * supports traditional array selection using square brackets, but also
 * supports inserting and deleting elements.  It is similar in function to
 * the STL vector type, but is simpler both to use and to implement.
 */
</scan>
template &lt;typename ValueType&gt;
class Vector {

public:

<a name="Constructor:Vector"><scan class=comment>/*
 * Constructor: Vector
 * Usage: Vector&lt;ValueType&gt; vec;
 *        Vector&lt;ValueType&gt; vec(n, value);
 * ---------------------------------------
 * Initializes a new vector.  The default constructor creates an empty
 * vector.  The second form creates an array with n elements, each of which
 * is initialized to value; if value is missing, the elements are
 * initialized to the default value for the type.
 */
</scan>
   Vector();
   explicit Vector(int n, ValueType value = ValueType());

<a name="Destructor:~Vector"><scan class=comment>/*
 * Destructor: ~Vector
 * -------------------
 * Frees any heap storage allocated by this vector.
 */
</scan>
   virtual ~Vector();

<a name="Method:size"><scan class=comment>/*
 * Method: size
 * Usage: int nElems = vec.size();
 * -------------------------------
 * Returns the number of elements in this vector.
 */
</scan>
   int size() const;

<a name="Method:isEmpty"><scan class=comment>/*
 * Method: isEmpty
 * Usage: if (vec.isEmpty()) ...
 * -----------------------------
 * Returns true if this vector contains no elements.
 */
</scan>
   bool isEmpty() const;

<a name="Method:clear"><scan class=comment>/*
 * Method: clear
 * Usage: vec.clear();
 * -------------------
 * Removes all elements from this vector.
 */
</scan>
   void clear();

<a name="Method:get"><scan class=comment>/*
 * Method: get
 * Usage: ValueType val = vec.get(index);
 * --------------------------------------
 * Returns the element at the specified index in this vector.  This method
 * signals an error if the index is not in the array range.
 */
</scan>
   const ValueType &amp; get(int index) const;

<a name="Method:set"><scan class=comment>/*
 * Method: set
 * Usage: vec.set(index, value);
 * -----------------------------
 * Replaces the element at the specified index in this vector with a new
 * value.  The previous value at that index is overwritten.  This method
 * signals an error if the index is not in the array range.
 */
</scan>
   void set(int index, const ValueType &amp; value);

<a name="Method:insert"><scan class=comment>/*
 * Method: insert
 * Usage: vec.insert(0, value);
 * ----------------------------
 * Inserts the element into this vector before the specified index.  All
 * subsequent elements are shifted one position to the right.  This method
 * signals an error if the index is outside the range from 0 up to and
 * including the length of the vector.
 */
</scan>
   void insert(int index, ValueType value);

<a name="Method:remove"><scan class=comment>/*
 * Method: remove
 * Usage: vec.remove(index);
 * -------------------------
 * Removes the element at the specified index from this vector.  All
 * subsequent elements are shifted one position to the left.  This method
 * signals an error if the index is outside the array range.
 */
</scan>
   void remove(int index);

<a name="Method:add"><scan class=comment>/*
 * Method: add
 * Usage: vec.add(value);
 * ----------------------
 * Adds a new value to the end of this vector.  To ensure compatibility
 * with the vector class in the Standard Template Library, this method is
 * also called push_back.
 */
</scan>
   void add(ValueType value);
   void push_back(ValueType value);

<a name="Operator:[]"><scan class=comment>/*
 * Operator: []
 * Usage: vec[index]
 * -----------------
 * Overloads [] to select elements from this vector.  This extension
 * enables the use of traditional array notation to get or set individual
 * elements.  This method signals an error if the index is outside the
 * array range.  The file supports two versions of this operator, one for
 * const vectors and one for mutable vectors.
 */
</scan>
   ValueType &amp; operator[](int index);
   const ValueType &amp; operator[](int index) const;

<a name="Operator:+"><scan class=comment>/*
 * Operator: +
 * Usage: v1 + v2
 * --------------
 * Concatenates two vectors.
 */
</scan>
   Vector operator+(const Vector &amp; v2) const;

<a name="Operator:+="><scan class=comment>/*
 * Operator: +=
 * Usage: v1 += v2;
 *        v1 += value;
 * -------------------
 * Adds all of the elements from v2 (or the single specified value) to v1. 
 * As a convenience, the Vector package also overloads the comma operator
 * so that it is possible to initialize a vector like this:
 *
 *    Vector&lt;int&gt; digits;
 *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
 */
</scan>
   Vector &amp; operator+=(const Vector &amp; v2);
   Vector &amp; operator+=(const ValueType &amp; value);

<a name="Method:toString"><scan class=comment>/*
 * Method: toString
 * Usage: string str = vec.toString();
 * -----------------------------------
 * Converts the vector to a printable string representation.
 */
</scan>
   std::string toString();

<a name="Method:mapAll"><scan class=comment>/*
 * Method: mapAll
 * Usage: vec.mapAll(fn);
 * ----------------------
 * Calls the specified function on each element of the vector in ascending
 * index order.
 */
</scan>
   void mapAll(void (*fn)(ValueType)) const;
   void mapAll(void (*fn)(const ValueType &amp;)) const;

   template &lt;typename FunctorType&gt;
   void mapAll(FunctorType fn) const;

<scan class=comment>/*
 * Additional Vector operations
 * ----------------------------
 * In addition to the methods listed in this interface, the Vector class
 * supports the following operations:
 *
 *   - Stream I/O using the &lt;&lt; and &gt;&gt; operators
 *   - Deep copying for the copy constructor and assignment operator
 *   - Iteration using the range-based for statement or STL iterators
 *
 * The iteration forms process the Vector in index order.
 */
</scan>
<scan class=comment>/* Private section */</scan>

<scan class=comment>/**********************************************************************/</scan>
<scan class=comment>/* Note: Everything below this point in the file is logically part    */</scan>
<scan class=comment>/* of the implementation and should not be of interest to clients.    */</scan>
<scan class=comment>/**********************************************************************/</scan>

private:

<a name="Implementation notes:Vector data structure"><scan class=comment>/*
 * Implementation notes: Vector data structure
 * -------------------------------------------
 * The elements of the Vector are stored in a dynamic array of the
 * specified element type.  If the space in the array is ever exhausted,
 * the implementation doubles the array capacity.
 */
</scan>
<scan class=comment>/* Instance variables */</scan>

   ValueType *elements;        <scan class=comment>/* A dynamic array of the elements   */</scan>
   int capacity;               <scan class=comment>/* The allocated size of the array   */</scan>
   int count;                  <scan class=comment>/* The number of elements in use     */</scan>

<scan class=comment>/* Private methods */</scan>

   void expandCapacity();
   void deepCopy(const Vector &amp; src);

<scan class=comment>/*
 * Hidden features
 * ---------------
 * The remainder of this file consists of the code required to support deep
 * copying and iteration.  Including these methods in the public interface
 * would make that interface more difficult to understand for the average
 * client.
 */
</scan>
public:

<scan class=comment>/*
 * Deep copying support
 * --------------------
 * This copy constructor and operator= are defined to make a deep copy,
 * making it possible to pass or return vectors by value and assign from
 * one vector to another.
 */
</scan>
   Vector(const Vector &amp; src);
   Vector &amp; operator=(const Vector &amp; src);

<a name="Operator:,"><scan class=comment>/*
 * Operator: ,
 * -----------
 * Adds an element to the vector passed as the left-hand operatand.  This
 * form makes it easier to initialize vectors in old versions of C++.
 */
</scan>
   Vector &amp; operator,(const ValueType &amp; value);

<scan class=comment>/*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input iterators
 * so that they work symmetrically with respect to the corresponding STL
 * classes.
 */
</scan>
   class iterator :
      public std::iterator&lt;std::random_access_iterator_tag, ValueType&gt; {

   private:
      const Vector *vp;
      int index;

   public:

      iterator() {
         this-&gt;vp = NULL;
      }

      iterator(const iterator &amp; it) {
         this-&gt;vp = it.vp;
         this-&gt;index = it.index;
      }

      iterator(const Vector *vp, int index) {
         this-&gt;vp = vp;
         this-&gt;index = index;
      }

      iterator &amp; operator++() {
         index++;
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      iterator &amp; operator--() {
         index--;
         return *this;
      }

      iterator operator--(int) {
         iterator copy(*this);
         operator--();
         return copy;
      }

      bool operator==(const iterator &amp; rhs) {
         return vp == rhs.vp &amp;&amp; index == rhs.index;
      }

      bool operator!=(const iterator &amp; rhs) {
         return !(*this == rhs);
      }

      bool operator&lt;(const iterator &amp; rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Iterators are in different vectors");
         return index &lt; rhs.index;
      }

      bool operator&lt;=(const iterator &amp; rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Iterators are in different vectors");
         return index &lt;= rhs.index;
      }

      bool operator&gt;(const iterator &amp; rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Iterators are in different vectors");
         return index &gt; rhs.index;
      }

      bool operator&gt;=(const iterator &amp; rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Iterators are in different vectors");
         return index &gt;= rhs.index;
      }

      iterator operator+(const int &amp; rhs) {
         return iterator(vp, index + rhs);
      }

      iterator operator+=(const int &amp; rhs) {
         index += rhs;
         return *this;
      }

      iterator operator-(const int &amp; rhs) {
         return iterator(vp, index - rhs);
      }

      iterator operator-=(const int &amp; rhs) {
         index -= rhs;
         return *this;
      }

      int operator-(const iterator &amp; rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Iterators are in different vectors");
         return index - rhs.index;
      }

      ValueType &amp; operator*() {
         return vp-&gt;elements[index];
      }

      ValueType *operator-&gt;() {
         return &amp;vp-&gt;elements[index];
      }

      ValueType &amp; operator[](int k) {
         return vp-&gt;elements[index + k];
      }

   };

   iterator begin() const {
      return iterator(this, 0);
   }

   iterator end() const {
      return iterator(this, count);
   }

};

<scan class=comment>/* Implementation section */</scan>

extern void error(std::string msg);

<a name="Implementation notes:Vector constructor and destructor"><scan class=comment>/*
 * Implementation notes: Vector constructor and destructor
 * -------------------------------------------------------
 * The constructor allocates storage for the dynamic array and initializes
 * the other fields of the object.  The destructor frees the memory used
 * for the array.
 */
</scan>
template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt;::Vector() {
   count = capacity = 0;
   elements = NULL;
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt;::Vector(int n, ValueType value) {
   count = capacity = n;
   elements = (n == 0) ? NULL : new ValueType[n];
   for (int i = 0; i &lt; n; i++) {
      elements[i] = value;
   }
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt;::~Vector() {
   if (elements != NULL) delete[] elements;
}

<a name="Implementation notes:Vector methods"><scan class=comment>/*
 * Implementation notes: Vector methods
 * ------------------------------------
 * The basic Vector methods are straightforward and should require no
 * detailed documentation.
 */
</scan>
template &lt;typename ValueType&gt;
int Vector&lt;ValueType&gt;::size() const {
   return count;
}

template &lt;typename ValueType&gt;
bool Vector&lt;ValueType&gt;::isEmpty() const {
   return count == 0;
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::clear() {
   if (elements != NULL) delete[] elements;
   count = capacity = 0;
   elements = NULL;
}

template &lt;typename ValueType&gt;
const ValueType &amp; Vector&lt;ValueType&gt;::get(int index) const {
   if (index &lt; 0 || index &gt;= count) error("get: index out of range");
   return elements[index];
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::set(int index, const ValueType &amp; value) {
   if (index &lt; 0 || index &gt;= count) error("set: index out of range");
   elements[index] = value;
}

<a name="Implementation notes:insert, remove, add"><scan class=comment>/*
 * Implementation notes: insert, remove, add
 * -----------------------------------------
 * These methods must shift the existing elements in the array to make room
 * for a new element or to close up the space left by a deleted one.
 */
</scan>
template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::insert(int index, ValueType value) {
   if (count == capacity) expandCapacity();
   if (index &lt; 0 || index &gt; count) {
      error("insert: index out of range");
   }
   for (int i = count; i &gt; index; i--) {
      elements[i] = elements[i - 1];
   }
   elements[index] = value;
   count++;
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::remove(int index) {
   if (index &lt; 0 || index &gt;= count) error("remove: index out of range");
   for (int i = index; i &lt; count - 1; i++) {
      elements[i] = elements[i + 1];
   }
   count--;
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::add(ValueType value) {
   insert(count, value);
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::push_back(ValueType value) {
   insert(count, value);
}

<a name="Implementation notes:Vector selection"><scan class=comment>/*
 * Implementation notes: Vector selection
 * --------------------------------------
 * The following code implements traditional array selection using square
 * brackets for the index.
 */
</scan>
template &lt;typename ValueType&gt;
ValueType &amp; Vector&lt;ValueType&gt;::operator[](int index) {
   if (index &lt; 0 || index &gt;= count) error("Selection index out of range");
   return elements[index];
}
template &lt;typename ValueType&gt;
const ValueType &amp; Vector&lt;ValueType&gt;::operator[](int index) const {
   if (index &lt; 0 || index &gt;= count) error("Selection index out of range");
   return elements[index];
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt; Vector&lt;ValueType&gt;::operator+(const Vector &amp; v2) const {
   Vector&lt;ValueType&gt; vec = *this;
   foreach (ValueType value in v2) {
      vec.add(value);
   }
   return vec;
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt; &amp; Vector&lt;ValueType&gt;::operator+=(const Vector &amp; v2) {
   foreach (ValueType value in v2) {
      *this += value;
   }
   return *this;
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt; &amp; Vector&lt;ValueType&gt;::operator+=(const ValueType &amp; value) {
   this-&gt;add(value);
   return *this;
}

template &lt;typename ValueType&gt;
std::string Vector&lt;ValueType&gt;::toString() {
   ostringstream os;
   os &lt;&lt; *this;
   return os.str();
}

<a name="Implementation notes:copy constructor and assignment operator"><scan class=comment>/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow a standard paradigm, as
 * described in the associated textbook.
 */
</scan>
template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt;::Vector(const Vector &amp; src) {
   deepCopy(src);
}

template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt; &amp; Vector&lt;ValueType&gt;::operator=(const Vector &amp; src) {
   if (this != &amp;src) {
      if (elements != NULL) delete[] elements;
      deepCopy(src);
   }
   return *this;
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::deepCopy(const Vector &amp; src) {
   count = capacity = src.count;
   elements = (capacity == 0) ? NULL : new ValueType[capacity];
   for (int i = 0; i &lt; count; i++) {
      elements[i] = src.elements[i];
   }
}

<a name="Implementation notes:The , operator"><scan class=comment>/*
 * Implementation notes: The , operator
 * ------------------------------------
 * The comma operator works adding the right operand to the vector and then
 * returning the vector by reference so that it is set for the next value
 * in the chain.
 */
</scan>
template &lt;typename ValueType&gt;
Vector&lt;ValueType&gt; &amp; Vector&lt;ValueType&gt;::operator,(const ValueType &amp; value) {
   this-&gt;add(value);
   return *this;
}

<a name="Implementation notes:mapAll"><scan class=comment>/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */
</scan>
template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::mapAll(void (*fn)(ValueType)) const {
   for (int i = 0; i &lt; count; i++) {
      fn(elements[i]);
   }
}

template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::mapAll(void (*fn)(const ValueType &amp;)) const {
   for (int i = 0; i &lt; count; i++) {
      fn(elements[i]);
   }
}

template &lt;typename ValueType&gt;
template &lt;typename FunctorType&gt;
void Vector&lt;ValueType&gt;::mapAll(FunctorType fn) const {
   for (int i = 0; i &lt; count; i++) {
      fn(elements[i]);
   }
}

<a name="Implementation notes:expandCapacity"><scan class=comment>/*
 * Implementation notes: expandCapacity
 * ------------------------------------
 * This function doubles the array capacity, copies the old elements into
 * the new array, and then frees the old one.
 */
</scan>
template &lt;typename ValueType&gt;
void Vector&lt;ValueType&gt;::expandCapacity() {
   capacity = max(1, capacity * 2);
   ValueType *array = new ValueType[capacity];
   for (int i = 0; i &lt; count; i++) {
      array[i] = elements[i];
   }
   if (elements != NULL) delete[] elements;
   elements = array;
}

<a name="Implementation notes:<< and >>"><scan class=comment>/*
 * Implementation notes: &lt;&lt; and &gt;&gt;
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
</scan>
template &lt;typename ValueType&gt;
std::ostream &amp; operator&lt;&lt;(std::ostream &amp; os, const Vector&lt;ValueType&gt; &amp; vec) {
   os &lt;&lt; "{";
   int len = vec.size();
   for (int i = 0; i &lt; len; i++) {
      if (i &gt; 0) os &lt;&lt; ", ";
      writeGenericValue(os, vec[i], true);
   }
   return os &lt;&lt; "}";
}

template &lt;typename ValueType&gt;
std::istream &amp; operator&gt;&gt;(std::istream &amp; is, Vector&lt;ValueType&gt; &amp; vec) {
   char ch;
   is &gt;&gt; ch;
   if (ch != '{') error("operator &gt;&gt;: Missing {");
   vec.clear();
   is &gt;&gt; ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         vec += value;
         is &gt;&gt; ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("operator &gt;&gt;: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

#endif
</pre>
</body>
</html>
