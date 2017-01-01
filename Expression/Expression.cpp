#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "Expression.h"

double NumberNode::eval(void)
{
    return num;
}

void NumberNode::prefixVisit(void)
{
    std::cout << num;
}

void NumberNode::postfixVisit(void)
{
    std::cout << num;
}

void OperationNode::prefixVisit(void)
{
    std::cout << op << ' ';
    lch->prefixVisit();
    std::cout << ' ';
    rch->prefixVisit();
}

void OperationNode::postfixVisit(void)
{
    lch->postfixVisit();
    std::cout << ' ';
    rch->postfixVisit();
    std::cout << ' ' << op;
}

double OperationNode::eval(void)
{
    switch (op) {
    case '+':
        return lch->eval() + rch->eval();
    case '-':
        return lch->eval() - rch->eval();
    case '*':
        return lch->eval() * rch->eval();
    case '/':
        return lch->eval() / rch->eval();
    case '^':
        return std::pow(lch->eval(), rch->eval());
    default:
        return NAN;
    }
}

void Parser::next(void)
{
    while (std::isblank(*curpos)) { ++curpos; }
    if (*curpos) {
        if (std::isdigit(*curpos) || *curpos == '.') {
            char * tmp;
            curnum = strtod(curpos, &tmp);
            curpos = tmp;
            status = NUMBER;
        } else if (std::strchr("+-*/^()", *curpos)) {
            curop = *curpos;
            ++curpos;
            status = OPERATOR;
        } else {
            status = UNKNOWN;
            curnum = NAN;
        }
    } else {
        status = END;
    }
}

std::unique_ptr<ExpressionNode> Parser::get(void)
{
    return std::unique_ptr<ExpressionNode>(binary(BasicPre));
}

ExpressionNode* Parser::binary(int pre)
{
    if (pre >= MaxPre) {
        return unary();
    } else {
        auto left = binary(pre + 1);
        if (status == END) {
            return left;
        }
        auto op = curop;
        if (assocOf(op) == Left) {
            next();
            return new OperationNode(left, op, binary(pre));
        } else {
            while (precedenceOf(op) == pre && status != END && status != UNKNOWN) {
                next();
                auto right = binary(pre + 1);
                left = new OperationNode(left, op, right);
                op = curop;
            }
            return left;
        }
    }
}

Parser::OperatorAssociativity Parser::assocOf(char op)
{
    switch (op) {
    case '^':
        return Left;
    default:
        return Right;
    }
}

int Parser::precedenceOf(char op)
{
    switch (op) {
    case '+':
    case '-':
        return BasicPre;
    case '*':
    case '/':
        return BasicPre + 1;
    case '^':
        return BasicPre + 2;
    default:
        return MaxPre;
    }
}

ExpressionNode* Parser::unary()
{
    ExpressionNode* res = nullptr;
    if (status == OPERATOR && curop == '(') {
        next();
        res = binary(BasicPre);
        next();
    } else {
        if (status == OPERATOR && curop == '-') {
            next();
            auto tmp = unary();
            res = new NumberNode(-(tmp->eval()));
            delete tmp;
        } else if (status == OPERATOR && curop == '+') {
            next();
            res = unary();
        } else if (status == NUMBER || status == UNKNOWN) {
            res = new NumberNode(curnum);
            next();
        }
    }
    return res;
}

ExpressionTestCase::ExpressionTestCase(const char* expr, double answer) : e(expr)
{
    ++total;
    right = std::abs((Parser(expr).get())->eval() - answer) < 0.00001;
    std::cout << e << " ...";
    if (right) {
        ++righted;
        std::cout << "Right!\n";
    } else {
        std::cout << "Wrong...\n";
    }
}

unsigned ExpressionTestCase::righted = 0;
unsigned ExpressionTestCase::total = 0;

int main(void)
{
#ifdef HAVETEST
    ExpressionTestCase("1+1+1", 3);
    ExpressionTestCase("(((((((1)))))))-0", 1);
    ExpressionTestCase("1-2-3", -4);
    ExpressionTestCase("2^30", 1073741824);
    ExpressionTestCase("1.1*1.3", 1.43);
    ExpressionTestCase("-5--1", -4);
    ExpressionTestCase("1+2*3^5", 487);
    ExpressionTestCase("3^-2", 0.111111111);
    ExpressionTestCase("901*-22", -19822);
    ExpressionTestCase("5+3*(7-8/2)+6", 20);
    std::cout << "Check Result: "
              << ExpressionTestCase::righted << '/'
              << ExpressionTestCase::total << '\n';
#else
    std::string tmp;
    for (;;) {
        std::cout << "请输入一行表达式：\n";
        std::getline(std::cin, tmp);
        Parser p(tmp);
        if (std::cin.eof()) {
            break;
        }
        auto m = p.get();
        std::cout << "前缀形式是：";
        m->prefixVisit();
        std::cout << '\n';
        std::cout << "中缀形式是：";
        m->postfixVisit();
        std::cout << '\n';
        std::cout << "计算结果是：" << m->eval() << "\n\n";
    }
#endif
    return 0;
}
