#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <cmath>
#include <exception>

class ExpressionNode {
public:
    ExpressionNode() {}
    virtual ~ExpressionNode() {}
    virtual double eval(void) = 0;
    virtual void prefixVisit(void) = 0;
    virtual void postfixVisit(void) = 0;
};

class NumberNode : public ExpressionNode {
public:
    NumberNode(double d) : num(d) {}
    virtual ~NumberNode() {}
    virtual double eval(void) override;
    virtual void prefixVisit(void) override;
    virtual void postfixVisit(void) override;
private:
    double num;
};

class OperationNode : public ExpressionNode {
public:
    OperationNode(ExpressionNode* l, char o, ExpressionNode* r) :
    lch(l), op(o), rch(r) {}
    virtual ~OperationNode()
    {
        delete lch;
        delete rch;
    }
    virtual void prefixVisit(void) override;
    virtual void postfixVisit(void) override;
    virtual double eval(void) override;
private:
    char op;
    ExpressionNode* lch;
    ExpressionNode* rch;
};

class Parser {
public:
    Parser(const std::string& s) : curpos(s.c_str()), curop(-1), curnum(NAN) { next(); }
    ~Parser() {}
    std::unique_ptr<ExpressionNode> get(void);
private:
    int precedenceOf(char op);
    enum OperatorAssociativity { Left, Right };
    OperatorAssociativity assocOf(char op);
    void next(void);
    ExpressionNode* binary(int pre);
    ExpressionNode* unary(void);
private:
    static const int MaxPre = 4;
    static const int BasicPre = 1;
    double curnum;
    int curop;
    const char* curpos;
    enum {
        NUMBER, OPERATOR, UNKNOWN, END
    } status;
};

class ExpressionTestCase {
public:
    ExpressionTestCase(const char* expr, double answer);
    static unsigned righted;
    static unsigned total;
private:
    const char* e;
    bool right;
};

#endif // EXPRESSION_H