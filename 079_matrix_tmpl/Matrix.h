#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

//YOUR CODE GOES HERE!
template<typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  std::vector<std::vector<T> > rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows(0) {}
  Matrix(int r, int c) : numRows(r), numColumns(c), rows(r) {
    for (typename std::vector<std::vector<T> >::iterator it = rows.begin();
         it != rows.end();
         ++it) {
      it->resize(c, T());
    }
  }
  Matrix(const Matrix<T> & rhs) :
      numRows(rhs.numRows),
      numColumns(rhs.numColumns),
      rows(rhs.rows) {}
  Matrix<T> & operator=(const Matrix<T> & rhs) {
    numRows = rhs.numRows;
    numColumns = rhs.numColumns;
    rows = rhs.rows;
    return *this;
  }
  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }
  const std::vector<T> & operator[](int index) const {
    assert(0 <= index && index < numRows);
    return rows[index];
  }
  std::vector<T> & operator[](int index) {
    assert(0 <= index && index < numRows);
    return rows[index];
  }
  bool operator==(const Matrix<T> & rhs) const {
    return numRows == rhs.numRows && numColumns == rhs.numColumns && rows == rhs.rows;
  }

  Matrix<T> operator+(const Matrix<T> & rhs) const {
    assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
    Matrix<T> ans(numRows, numColumns);
    typename std::vector<std::vector<T> >::iterator a;
    typename std::vector<std::vector<T> >::const_iterator b = rows.begin(),
                                                          c = rhs.rows.begin();
    for (a = ans.rows.begin(); a != ans.rows.end(); ++a, ++b, ++c) {
      typename std::vector<T>::iterator aa;
      typename std::vector<T>::const_iterator bb = b->begin(), cc = c->begin();
      for (aa = a->begin(); aa != a->end(); ++aa, ++bb, ++cc) {
        *aa = *bb + *cc;
      }
    }
    return ans;
  }
  template<typename U>
  friend std::ostream & operator<<(std::ostream & s, const Matrix<U> & m);
};

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (typename std::vector<std::vector<T> >::const_iterator it = rhs.rows.begin();
       it != rhs.rows.end();
       ++it) {
    if (it != rhs.rows.begin()) {
      s << ",\n{";
    }
    else {
      s << "{";
    }
    for (typename std::vector<T>::const_iterator it2 = it->begin(); it2 != it->end();
         ++it2) {
      if (it2 != it->begin()) {
        s << ", ";
      }
      s << *it2;
    }
    s << "}";
  }
  s << " ]";
  return s;
}
#endif
