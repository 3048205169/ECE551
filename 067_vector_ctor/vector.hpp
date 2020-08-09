/* write your class interface in this file
   write function definitions only if they are very short
 */
#include "cmath"
#include "stdio.h"
#include "stdlib.h"
class Vector2D {
 private:
  double x;
  double y;

 public:
  Vector2D() {
    x = 0;
    y = 0;
  }

  Vector2D(double x_in, double y_in) {
    x = x_in;
    y = y_in;
  }
  void initVector(double init_x, double init_y) {
    x = init_x;
    y = init_y;
  }
  double getMagnitude() const { return std::sqrt(x * x + y * y); }
  Vector2D operator+(const Vector2D & rhs) const {
    Vector2D a;
    a.x = x + rhs.x;
    a.y = y + rhs.y;
    return a;
  }
  Vector2D & operator+=(const Vector2D & rhs) {
    Vector2D & p = *this;
    p.x = rhs.x + p.x;
    p.y = rhs.y + p.y;
    return p;
  }
  double dot(const Vector2D & rhs) const {
    double dotr;
    dotr = x * rhs.x + y * rhs.y;
    return dotr;
  }
  void print() const { printf("<%.2f, %.2f>", x, y); }
};
