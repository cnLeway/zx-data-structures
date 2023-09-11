
#include <bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int data;
  struct Node *next;
} Node;

typedef struct Stack {
  Node *top;
  int size;
} Stack;

Stack *createStack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->top = NULL;
  stack->size = 0;
  return stack;
}

void push(Stack *stack, int data) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->data = data;
  node->next = stack->top;
  stack->top = node;
  stack->size++;
}

int pop(Stack *stack) {
  if (stack->top == NULL) {
    return -1;
  }

  int data = stack->top->data;
  Node *node = stack->top;
  stack->top = stack->top->next;
  stack->size--;
  free(node);
  return data;
}

int getSize(Stack *stack) {
  return stack->size;
}

int peekFront(Stack *stack) {
  if (stack->top == NULL) {
    return -1;
  }
  return stack->top->data;
}

int peekRear(Stack *stack) {
  if (stack->top == NULL) {
    return -1;
  }

  Node *node = stack->top;
  while (node->next != NULL) {
    node = node->next;
  }
  return node->data;
}

void traverseStack(Stack *stack) {
  Node *node = stack->top;
  while (node != NULL) {
    printf("%d ", node->data);
    node = node->next;
  }
  printf("\n");
}

int isEmpty(Stack *stack) {
  return stack->top == NULL;
}

void destroyStack(Stack *stack) {
  while (stack->top != NULL) {
    int data = pop(stack);
    // free(stack->top);
  }
  free(stack);
}

void printHelp(void)
{
    cout << endl;
    cout << "Now send different CMD to test STACK functionality repeatedly, until you input 'exit'" << endl;
    cout << endl;
    cout << "push <int>       	to push one integer into STACK" << endl;
    cout << "pop              	to pop one integer from STACK" << endl;
    cout << "traverse           to traverse STACK" << endl;
    cout << "size           	to get the size of STACK" << endl;
    cout << "front          	to get the front of STACK" << endl;
    cout << "rear           	to get the rear of STACK" << endl;
    cout << "empty           	to check if it's empty of STACK" << endl;
    cout << "help               to print the information again" << endl;
    cout << "exit               to exit test" << endl;
    cout << endl;
}

int main(void)
{
	Stack *stack = createStack();
    cout << "Stack is created" << endl;
    printHelp();

    string cmd;
    while (true)
    {
        getline(cin, cmd);
        if (cmd.substr(0, strlen("exit")) == string("exit"))
        {
            break;
        }

        if (cmd.substr(0, strlen("help")) == string("help"))
        {
            printHelp();
            continue;
        }

        if (cmd.substr(0, strlen("push")) == string("push"))
        {
			char str1[9];
			int qdata;
			sscanf(cmd.c_str(), "%s %d", str1, &qdata);
			push(stack, qdata);
            continue;
        }

        if (cmd.substr(0, strlen("pop")) == string("pop"))
        {
			cout << cmd << " result " << pop(stack) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("empty")) == string("empty"))
        {
			cout << cmd << " result " << isEmpty(stack) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("rear")) == string("rear"))
        {
			cout << cmd << " result " << peekRear(stack) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("front")) == string("front"))
        {
			cout << cmd << " result " << peekFront(stack) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("size")) == string("size"))
        {
			cout << cmd << " result " << getSize(stack) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("traverse")) == string("traverse"))
        {
			cout << cmd << endl;
			traverseStack(stack);
            continue;
        }

        cout << "Unsupported CMD, please check it carefully." << endl;
        printHelp();
    }

  	destroyStack(stack);
    cout << "Stack is destroied\n";
}
