#include <string.h>

#include <sstream>
class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
 private:
  long Num;

 public:
  NumExpression(long n) : Num(n) {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << Num;
    return ss.str();
  };
};

class PlusExpression : public Expression {
 private:
  Expression *lhs, *rhs;

 public:
  PlusExpression(Expression * l, Expression * r) : lhs(l), rhs(r){};
  virtual std::string toString() const {
    std::stringstream ss;
    ss << '(' << lhs->toString() << ' ' << '+' << ' ' << rhs->toString() << ')';
    return ss.str();
  }
  ~PlusExpression() {
    delete rhs;
    delete lhs;
  }
};
