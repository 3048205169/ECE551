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

class OExpression : public Expression {
 private:
  Expression *lhs, *rhs;
  const char * op;

 public:
  OExpression(Expression * l, Expression * r, const char * o) : lhs(l), rhs(r), op(o){};
  virtual std::string toString() const {
    std::stringstream ss;
    ss << '(' << lhs->toString() << ' ' << op << ' ' << rhs->toString() << ')';
    return ss.str();
  }
  ~OExpression() {
    delete rhs;
    delete lhs;
  }
};

class PlusExpression : public OExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : OExpression(lhs, rhs, "+") {}
};

class MinusExpression : public OExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : OExpression(lhs, rhs, "-") {}
};

class TimesExpression : public OExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : OExpression(lhs, rhs, "*") {}
};

class DivExpression : public OExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OExpression(lhs, rhs, " / ") {}
};
