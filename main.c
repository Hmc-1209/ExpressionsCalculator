#include<stdio.h>
#include<ctype.h>
#include<math.h>

#define LT 1000000 // Maximum chars available.

// Stack for temporary storing operators when converting equation to postfix.
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

void printOperator(void);

int topOperator(void);

void queue(int element);

void printQueue(void);

void calPush(double num);

void calPop(void);

double calTop(void);

void calPrint(void);


// Generating stacks and queue.
Operators op;
Queue q;
Calculate ca;


// Check if the char is number or operator.
int inRange(char c) {
    int e = (int) c;
    if ((e >= 48 && e <= 57) || (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^') || (c == '%') ||
        (c == '(') || (c == ')')) return 1;
    return 0;
}


// Converting equation to postfix.
void postfix(char *equations, int l) {

    int num = -1;
    for (int j = 0; j < l; j++) {
        // If the element is digit, save it in the int "num".
        if (isdigit(equations[j])) {
            if (num == -1) { // If this is the first num :
                num = (int) equations[j] - 48;
            } else {  // If this is not the first num :
                num = num * 10 + ((int) equations[j] - 48);
            }
        } else if (inRange(equations[j])) {
            if (num != -1) queue(num); // Queuing the number into the queue.
            num = -1; // Reset num.
            // Checking the level of the operator.
            // [+, -, (] -> [-1, -2, -13]
            int opr = 0;
            if (equations[j] == '+') opr = -1;
            else if (equations[j] == '-') opr = -2;
            else if (equations[j] == '*') opr = -4;
            else if (equations[j] == '/') opr = -5;
            else if (equations[j] == '%') opr = -6;
            else if (equations[j] == '^') opr = -7;
            else if (equations[j] == '(') opr = -13;
            else if (equations[j] == ')') { // Right parenthesis has special behavior.
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
    if (equations[l - 1] != ')') queue(num); // Queuing the last number if exists.
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


// Checking if the equation is legal.
int exception_handling(char *equations, int l) {
    int parentheses = 0;
    for (int i = 0; i < l; i++) {
        if (equations[i] == '(') parentheses++;
        else if (equations[i] == ')') {
            if (parentheses >= 0) parentheses--;
            else return 1;
        }
    }
    if (parentheses != 0) return 1;
    return 0;
}


int main() {

    // Array equations for storing all chars.
    // Pointer equation_pos for sending the position of the start pos of current equation.
    // Array equations_length for saving numbers of chars in single equation.
    // int "i" is just a reusable variable.
    char equations[LT];
    char *equation_pos = &equations[0];
    int equations_length[LT / 3];
    int length = 0, T = 0, i = 0;

    // Entering equations
    while (scanf("%c", &equations[i]) != ' ') {
        if (equations[i] == ' ') { // When entering space and return the line, end the input.
            equations_length[T++] = length;
            break;
        }
        if (equations[i] == '\n') { // When entering return, save the current equation detail.
            equations_length[T++] = length;
            length = 0;
        } else length++;
        i++;
    }

    // --------------------- Calculating ---------------------
    i = 0;
    while (equations_length[i] != '\0') {
        op.top = -1;    // Resetting the operators stack.
        q.front = 0;    // Resetting the queue.
        q.rear = -1;
        ca.top = -1;    // Resetting the calculator stack.

        // If the equation is legal, do postfix and calculate.
        if (exception_handling(equation_pos, equations_length[i]) != 1) {
            postfix(equation_pos, equations_length[i]);
//            printQueue();
            calculate();
            // Else, told the user this equation has some problem.
        } else printf("The equation has unbalanced parentheses.");

        equation_pos += equations_length[i] + 1; // Plus one for '\n' operator.
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

void printOperator(void) {  // Print
    for (int i = 0; i < op.top + 1; i++) printf("%d ", op.values[i]);
    printf("\n");
}

int topOperator(void) {
    return op.values[op.top];
}

// --------------------- Queue functions ---------------------
void queue(int element) {
    q.rear += 1;
    q.values[q.rear] = element;
}

void dequeue(void) {
    q.front += 1;
}

void printQueue(void) {
    for (int i = q.front; i < q.rear + 1; i++) {
        if (q.values[i] >= 0) {
            printf("%d ", q.values[i]);
        } else {
            switch (q.values[i]) {
                case -1:
                    printf("+ ");
                    break;
                case -2:
                    printf("- ");
                    break;
                case -4:
                    printf("* ");
                    break;
                case -5:
                    printf("/ ");
                    break;
                case -6:
                    printf("%% ");
                    break;
                case -7:
                    printf("^ ");
                    break;
            }
        }
    }
    printf("\n");
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

void calPrint(void) {
    for (int i = 0; i <= ca.top; i++) {
        printf("%.13f ", ca.values[i]);
    }
    printf("\n");
}