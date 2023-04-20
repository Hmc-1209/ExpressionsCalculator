#include<stdio.h>
#include<ctype.h>
#include<math.h>

#define LT 1000000 // Maximum chars available.

// Stack for temporary storing operators when converting expression to postfix.
typedef struct Operators {
    int values[LT];
    int top;
} Operators;
// Queue for storing all postfix numbers and operators.
typedef struct Queue {
    int values[LT];
    int front;
    int rear;
} Queue;
// Stack for temporary storing numbers when calculating answers.
typedef struct Calculate {
    double values[LT];
    int top;
} Calculate;


// Defining functions of stacks and queue.
void pushOperator(int operator);

void popOperator(void);

void queue(int element);

void calPush(double num);

void calPop(void);

double calTop(void);


// Generating stacks and queue.
Operators op;
Queue q;
Calculate ca;


// Check if the char is number or operator.
int inRange(char c) {
    int e = (int) c;
    if ((e >= 48 && e <= 57) || (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^') || (c == '%') ||
        (c == '(') || (c == ')'))
        return 1;
    return 0;
}


// Converting expression to postfix.

void postfix(char *expressions, int l) {

    int num = -1;
    for (int j = 0; j < l; j++) {
        // If the element is digit, save it in the int "num".
        if (isdigit(expressions[j])) {
            if (num == -1) { // If this is the first num :
                num = (int) expressions[j] - 48;
            } else {  // If this is not the first num :
                num = num * 10 + ((int) expressions[j] - 48);
            }
        } else if (inRange(expressions[j])) {
            if (num != -1) queue(num); // Queuing the number into the queue.
            num = -1; // Reset num.
            // Checking the level of the operator.
            // [+, -, (] -> [-1, -2, -13]
            int opr = 0;
            if (expressions[j] == '+') opr = -1;
            else if (expressions[j] == '-') opr = -2;
            else if (expressions[j] == '*') opr = -4;
            else if (expressions[j] == '/') opr = -5;
            else if (expressions[j] == '%') opr = -6;
            else if (expressions[j] == '^') opr = -7;
            else if (expressions[j] == '(') opr = -13;
            else if (expressions[j] == ')') { // Right parenthesis has special behavior.
                opr = -14;
                while (op.values[op.top] != -13) {
                    queue(op.values[op.top]);
                    popOperator();
                }
                popOperator();
            }
            // While there are still element(s) at same level inside the stack :
            if (opr != -14) {
                while (((op.values[op.top] / 4 == opr / 4) || (op.values[op.top] / 4 < opr / 4)) && (op.top != -1) &&
                       (op.values[op.top] != -13) && (opr != -13)) {
                    queue(op.values[op.top]); // Queue it.
                    popOperator();  // Pop it from the stack.
                }
                pushOperator(opr); // If opr not ")", then push it into the stack.
            }
        }
    }
    if (expressions[l - 1] != ')') queue(num); // Queuing the last number if exists.
    while (op.top != -1) {
        queue(op.values[op.top]);
        popOperator();
    }
}


// Calculate the postfix function and print out the answer.
void calculate() {
    for (int i = 0; i <= q.rear; i++) {
        if (q.values[i] >= 0) calPush(q.values[i]);
        else {
            double a = calTop();
            calPop();
            double b = calTop();
            calPop();
            switch (q.values[i]) {
                case -1:
                    calPush(b + a);
                    break;
                case -2:
                    calPush(b - a);
                    break;
                case -4:
                    calPush(b * a);
                    break;
                case -5:
                    calPush(b / a);
                    if (a == 0) {   // Handling error if the denominator is 0 after some steps of calculating.
                        printf("The expression can't divide a number by zero.");
                        return;
                    }
                    break;
                case -6:
                    calPush(fmod(b, a));
                    break;
                case -7:
                    calPush(pow(b, a));
                    break;
            }
        }
    }
    printf("%.13f", calTop() / 1.0);
}

// Checking if the expression is legal.
int exception_handling(char *expressions, int l) {
    int parentheses = 0;
    for (int i = 0; i < l; i++) {
        if (expressions[i] == '(') parentheses++;
        else if (expressions[i] == ')') {
            if (parentheses >= 0) parentheses--;
            else return 1;
        } else if (expressions[i] == '/' && expressions[i + 1] == '0') return 2;
    }
//    if (parentheses != 0) return 1;
    return (parentheses != 0) ? 1 : 0;
}


int main() {

    // Array expressions for storing all chars.
    // Pointer expression_pos for sending the position of the start pos of current expression.
    // Array expressions_length for saving numbers of chars in single expression.
    // int "i" is just a reusable variable.
    char expressions[LT];
    char *expression_pos = &expressions[0];
    int expressions_length[LT / 3];
    int length = 0, T = 0, i = 0;

    // Entering expressions
    while (scanf("%c", &expressions[i]) != ' ') {
        if (expressions[i] == ' ') { // When entering space and return the line, end the input.
            expressions_length[T++] = length;
            break;
        }
        if (expressions[i] == '\n') { // When entering return, save the current expression detail.
            expressions_length[T++] = length;
            length = 0;
        } else length++;
        i++;
    }

    // --------------------- Calculating ---------------------
    i = 0;
    while (expressions_length[i] != '\0') {
        op.top = -1;    // Resetting the operators stack.
        q.front = 0;    // Resetting the queue.
        q.rear = -1;
        ca.top = -1;    // Resetting the calculator stack.

        // If the expression is illegal, told the user this expression has some problem.
        switch (exception_handling(expression_pos, expressions_length[i])) {
            case 1:
                printf("The expression has unbalanced parentheses.");
                break;
            case 2:
                printf("The expression can't divide a number by zero.");
                break;
                // Else, do postfix and calculate the answer.
            default:
                postfix(expression_pos, expressions_length[i]);
                calculate();
                break;
        }

        // Move the pointer to the start of the next expression.
        expression_pos += expressions_length[i] + 1; // Plus one for '\n' operator.
        printf("\n");

        i++;
    }
    return 0;
}

// --------------------- Operator functions ---------------------
void pushOperator(int operator) {   // Pushing an operator in the storage stack.
    op.top += 1;
    op.values[op.top] = operator;
}

void popOperator(void) {    // Popping an operator from the storage stack.
    op.top -= 1;
}

// --------------------- Queue functions ---------------------
void queue(int element) {
    q.rear += 1;
    q.values[q.rear] = element;
}

// --------------------- Calculator functions ---------------------
void calPush(double num) {
    ca.top += 1;
    ca.values[ca.top] = num;
}

void calPop() {
    ca.top -= 1;
}

double calTop(void) {
    return ca.values[ca.top];
}
