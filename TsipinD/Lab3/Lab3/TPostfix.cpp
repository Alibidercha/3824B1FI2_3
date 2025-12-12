#include "TPostfix.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cmath>


TPostfix::TPostfix(const std::string& expr) : infix(expr) {}

const std::string& TPostfix::GetInfix() const {return infix;} 

const std::string& TPostfix::GetPostfix() const { return postfix;} 

bool TPostfix::IsLetter(char c) const {return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');} 

bool TPostfix::IsDigit(char c) const {return (c >= '0' && c <= '9');}

bool TPostfix::IsOperator(char c) const { return c == '+' || c == '-' || c == '*' || c == '/';}

int TPostfix::Priority(const std::string& op) const {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "~")              return 3; // '~' - унарный минус 
    if (op == "ln")             return 4;
    return 0;
}

void TPostfix::CheckExpression() const {
    TStack<char> st(1000);
    bool prevWasOperator = true;
    bool prevWasFunction = false;

    for (size_t i = 0; i < infix.size(); ++i) {
        char c = infix[i];

        if (c == ' ')
            continue;


        if (c == '(') {
            st.Push('(');

            if (!prevWasOperator && !prevWasFunction)
                throw std::runtime_error("Missed the operator before the '(' at position " + std::to_string(i));

            prevWasOperator = true;
            prevWasFunction = false;
            continue;
        }


        if (c == ')') {
            if (st.IsEmpty())
                throw std::runtime_error("Extra closing parenthesis at position " + std::to_string(i));

            st.Pop();

            if (prevWasOperator)
                throw std::runtime_error("Empty brackets at position " + std::to_string(i));

            prevWasOperator = false;
            prevWasFunction = false;
            continue;
        }


        if (IsOperator(c)) {
            if (c == '-' && prevWasOperator) {
                // унарный минус Ч пропускаем
            }
            else {
                if (prevWasOperator)
                    throw std::runtime_error("Two operators in a row at position " + std::to_string(i));
            }

            prevWasOperator = true;
            prevWasFunction = false;
            continue;
        }

        if (IsDigit(c) || c == '.') {

            size_t j = i;
            bool hasPoint = false; // проверка на c == '.' позже в ифах и циклах

            // запрет .1   |   1. - разрешен
            if (c == '.') {
            //    if (j + 1 >= infix.size() || !IsDigit(infix[j + 1]))    // разрешить .1
                    throw std::runtime_error("A number cannot start from a dot at position " + std::to_string(i));
            }

            while (j < infix.size() && (IsDigit(infix[j]) || infix[j] == '.')) {
                if (infix[j] == '.') {
                    if (hasPoint)
                        throw std::runtime_error("Two dots in the number at position " + std::to_string(j));
                    hasPoint = true;
                }
                j++;
            }
            if (j < infix.size() && (IsLetter(infix[j]) || infix[j] == '_')) {
                throw std::runtime_error(
                    "A variable cannot start immediately after a number at position " + std::to_string(j)
                );
            }

            i = j - 1;
            prevWasOperator = false;
            prevWasFunction = false;
            continue;
        }


        if (IsLetter(c) || c == '_') {

            std::string name;
            size_t j = i;

            // первый символ: буква или _
            name.push_back(infix[j]);
            j++;

            // всЄ остальное: буквы, цифры, _
            while (j < infix.size() &&
                (IsLetter(infix[j]) || IsDigit(infix[j]) || infix[j] == '_')) {
                name.push_back(infix[j]);
                j++;
            }

            if (name == "ln") {
                prevWasFunction = true;
            }
            else {
                prevWasFunction = false;
            }

            i = j - 1;
            prevWasOperator = false;
            continue;
        }

        throw std::runtime_error(
            "Invalid character '" + std::string(1, c) +
            "' at position " + std::to_string(i)
        );
    }

    if (!st.IsEmpty())
        throw std::runtime_error("The bracket is not closed");

    if (prevWasOperator)
        throw std::runtime_error("An expression cannot end with an operator");
}

void TPostfix::ToPostfix() {CheckExpression(); ToPostfixInternal();}

void TPostfix::AddToken(const std::string& tok) {
    if (!postfix.empty())
        postfix += ' ';
    postfix += tok;
}

void TPostfix::ToPostfixInternal() {
    postfix.clear();
    TStack<std::string> opStack(1000);

    bool prevWasOperator = true;

    for (size_t i = 0; i < infix.size(); ) {
        char c = infix[i];

        if (c == ' ') {
            ++i;
            continue;
        }

        // число
        if (IsDigit(c)) {
            std::string number;
            while (i < infix.size() && (IsDigit(infix[i]) || infix[i] == '.')) {
                number.push_back(infix[i]);
                ++i;
            }
            AddToken(number);
            prevWasOperator = false;
            continue;
        }

        // переменна€ или функци€ ln
        if (IsLetter(c) || c == '_') {
            std::string name;
            while (i < infix.size() && (IsLetter(infix[i]) || IsDigit(infix[i]) || infix[i] == '_')) { // проверка на первый символ infix[i] = число была ранее,
                name.push_back(infix[i]);                                                              // только дальнейшие индекса могут оказатьс€ числом, но не первый
                ++i;
            }

            if (name == "ln") {
                opStack.Push("ln");
                prevWasOperator = true;
            }
            else {
                AddToken(name);
                prevWasOperator = false;
            }
            continue;
        }

        // открывающа€ скобка
        if (c == '(') {
            opStack.Push("(");
            prevWasOperator = true;
            ++i;
            continue;
        }

        // закрывающа€ скобка
        if (c == ')') {
            while (!opStack.IsEmpty() && opStack.Top() != "(") { //выгрузка операторов, последующа€ проверка соответстви€ открыти€ и закрыти€ скобок
                AddToken(opStack.Pop());
            }
            if (opStack.IsEmpty())
                throw std::runtime_error("Bracket mismatch"); // в OpStack должна остатьс€ "(" после выгрузки 

            opStack.Pop(); // убрать "("

            if (!opStack.IsEmpty() && opStack.Top() == "ln") { // проверка ln( ... ) 
                AddToken(opStack.Pop());
            }

            prevWasOperator = false;
            ++i;
            continue;
        }

        // оператор
        if (IsOperator(c)) {
            std::string op(1, c);

            if (c == '-' && prevWasOperator) {
                op = "~";
            }

            while (!opStack.IsEmpty() &&
                opStack.Top() != "(" &&
                Priority(opStack.Top()) >= Priority(op)) {
                AddToken(opStack.Pop());
            }

            opStack.Push(op);
            prevWasOperator = true;
            ++i;
            continue;
        }

        throw std::runtime_error("Unexpected character during parsing");
    }

    while (!opStack.IsEmpty()) {
        std::string op = opStack.Pop();
        if (op == "(" || op == ")")
            throw std::runtime_error("Extra brackets");
        AddToken(op);
    }
}

double TPostfix::Calculate() {
    if (postfix.empty())
        ToPostfix();

    TStack<double> st(1000);
    std::map<std::string, double> vars;

    size_t i = 0;

    while (i < postfix.size()) {

        if (postfix[i] == ' ') {  // пропуск пробелов
            ++i;
            continue;
        }

        std::string tok;
        while (i < postfix.size() && postfix[i] != ' ') {
            tok.push_back(postfix[i]);
            ++i;
        }

        // далее обработка токена
 
        bool isNumber = true;
        bool hasPoint = false;

        for (char c : tok) {
            if (c == '.') {
                if (hasPoint) {
                    isNumber = false;
                    break;
                }
                hasPoint = true;
            }
            else if (!IsDigit(c)) {
                isNumber = false;
                break;
            }
        }

        if (isNumber && !tok.empty()) {
            st.Push(std::stod(tok));
            continue;
        }


        if (tok == "~") {           // унарный минус
            double a = st.Pop();
            st.Push(-a);
            continue;
        }

        if (tok == "ln") {         // функци€ ln
            double a = st.Pop();
            st.Push(std::log(a));
            continue;
        }

        // бинарные операторы
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
            double b = st.Pop();
            double a = st.Pop();

            if (tok == "+") st.Push(a + b);
            else if (tok == "-") st.Push(a - b);
            else if (tok == "*") st.Push(a * b);
            else if (tok == "/" && b == 0)
                throw std::runtime_error("Division by zero");
            else if (tok == "/") st.Push(a / b);

            continue;
        }

        // переменна€
        if (vars.find(tok) == vars.end()) {
            std::cout << "Enter the value of the variable " << tok << ": ";
            double val;
            std::cin >> val;
            vars[tok] = val;
        }

        st.Push(vars[tok]);
    }

    //проверка результата

    if (st.IsEmpty())
        throw std::runtime_error("Calculation error: stack is empty");

    double result = st.Pop();

    if (!st.IsEmpty())
        throw std::runtime_error("Calculation error: extra items in the stack");

    return result;
}