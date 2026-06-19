#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <cmath>

enum TokenType
{
    OPERATOR,
    OPERAND
};

struct Token
{
    TokenType tType;
    union
    {
        double oprnd;
        char oprtr;
    };

    Token(double val)
    {
        oprnd = val;
        tType = OPERAND;
    }

    Token(char c)
    {
        oprtr = c;
        tType = OPERATOR;
    }
};

std::vector<Token> getExprTokenVec(const std::string &expr)
{
    std::vector<Token> tokenVec;
    std::stringstream ss(expr);

    tokenVec.reserve(expr.length());

    
    while (ss >> std::ws)
    {
        char next = ss.peek();

        bool isUnary = (next == '+' || next == '-') && (tokenVec.empty() || (tokenVec.back().tType == OPERATOR && tokenVec.back().oprtr != ')'));

        if (std::isdigit(next) || next == '.' || isUnary)
        {
            double oprnd;
            if (ss >> oprnd)
            {
                tokenVec.push_back(Token(oprnd));
            }
        }
        else if (next == '+' || next == '-' || next == '*' || next == '/' || next == '^' || next == '(' || next == ')')
        {
            char oprtr;
            ss >> oprtr;
            tokenVec.push_back(Token(oprtr));
        }
        else
        {
            char discard;
            ss >> discard;
        }
    }
    return tokenVec;
}

inline unsigned int precedence(char c)
{
    switch (c)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}

inline bool popAndEval(std::stack<char> &oprtrStack, std::stack<double> &oprndStack)
{
    if (oprndStack.size() < 2 || oprtrStack.empty())
    {
        std::cerr << "Malformed expression structure" << std::endl;
        return false;
    }

    double b = oprndStack.top();
    oprndStack.pop();
    double a = oprndStack.top();
    oprndStack.pop();
    char oprtr = oprtrStack.top();
    oprtrStack.pop();

    switch (oprtr)
    {
    case '+':
        oprndStack.push(a + b);
        break;
    case '-':
        oprndStack.push(a - b);
        break;
    case '*':
        oprndStack.push(a * b);
        break;
    case '/':
        oprndStack.push(a / b);
        break;
    case '^':
        oprndStack.push(std::pow(a, b));
        break;
    default:
        break;
    }
    return true;
}

double eval(std::vector<Token> &tokenVec)
{
    std::stack<char> oprtrStck;
    std::stack<double> oprndStck;

    for (auto it = tokenVec.begin(); it != tokenVec.end(); ++it)
    {
        Token &t = *it;
        if (t.tType == OPERAND)
        {
            oprndStck.push(t.oprnd);
            // std::cout << t.oprnd << " ";
        }
        else if (t.tType == OPERATOR)
        {
            auto prec = precedence(t.oprtr);

            if (t.oprtr == '(')
                oprtrStck.push(t.oprtr);
            else if (t.oprtr == ')')
            {
                while (!oprtrStck.empty() && oprtrStck.top() != '(')
                {
                    if (!popAndEval(oprtrStck, oprndStck))
                        return 80883;
                }
                // remove (
                if (oprtrStck.empty())
                {
                    std::cerr << "Unmatched paranthesis" << std::endl;
                    return 80883;
                }
                oprtrStck.pop();
            }
            else
            {
                while (!oprtrStck.empty() &&
                       ((precedence(oprtrStck.top()) == prec && t.oprtr != '^') ||
                        (precedence(oprtrStck.top()) > prec)))
                {
                    if (!popAndEval(oprtrStck, oprndStck))
                        return 80883;
                }
                oprtrStck.push(t.oprtr);
            }
        }
    }

    while (!oprtrStck.empty())
    {
        if (oprtrStck.top() == '(')
        {
            std::cerr << "Unmatched paranthesis" << std::endl;
            return 80883;
        }
        if (!popAndEval(oprtrStck, oprndStck))
            return 80883;
    }

    if (oprndStck.size() != 1)
    {
        std::cerr << "Malformed expression: unmatched operands" << std::endl;
        return 80883;
    }

    double result = oprndStck.top();
    return result;
}

int main()
{
    std::string expression;
    do
    {
        std::cout << ">> ";
        std::getline(std::cin, expression);

        auto tokenVec = getExprTokenVec(expression);

        if (tokenVec.size() == 0)
            continue;

        auto result = eval(tokenVec);
        std::cout << result << std::endl;

    } while (expression != "\\q");

    return EXIT_SUCCESS;
}