
#include <bits/stdc++.h>
using namespace std;

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *front;
	Node *rear;
} Queue;

Queue *createQueue() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

void enQueue(Queue *queue, int data) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;

  if (queue->front == NULL) {
    queue->front = node;
    queue->rear = node;
  } else {
    queue->rear->next = node;
    queue->rear = node;
  }
}

int deQueue(Queue *queue) {
  if (queue->front == NULL) {
    return -1;
  }

  int data = queue->front->data;
  Node *node = queue->front;
  queue->front = queue->front->next;

  if (queue->front == NULL) {
    queue->rear = NULL;
  }

  free(node);
  return data;
}

int getSize(Queue *queue) {
  int size = 0;
  Node *node = queue->front;
  while (node != NULL) {
    size++;
    node = node->next;
  }
  return size;
}

int peekFront(Queue *queue) {
  if (queue->front == NULL) {
    return -1;
  }
  return queue->front->data;
}

int peekRear(Queue *queue) {
  if (queue->rear == NULL) {
    return -1;
  }
  return queue->rear->data;
}

void traverseQueue(Queue *queue) {
  Node *node = queue->front;
  while (node != NULL) {
    printf("%d ", node->data);
    node = node->next;
  }
  printf("\n");
}

int isEmpty(Queue *queue) {
  return queue->front == NULL;
}

void destroyQueue(Queue *queue) {
  while (queue->front != NULL) {
    int data = deQueue(queue);
    // free(queue->front);
  }
  free(queue);
}

void printHelp(void)
{
    cout << endl;
    cout << "Now send different CMD to test QUEUE functionality repeatedly, until you input 'exit'" << endl;
    cout << endl;
    cout << "enque <int>        to push one integer into queue" << endl;
    cout << "deque              to pop one integer from QUEUE" << endl;
    cout << "traverse           to traverse QUEUE" << endl;
    cout << "size           	to get the size of QUEUE" << endl;
    cout << "front          	to get the front of QUEUE" << endl;
    cout << "rear           	to get the rear of QUEUE" << endl;
    cout << "empty           	to check if it's empty of QUEUE" << endl;
    cout << "help               to print the information again" << endl;
    cout << "exit               to exit test" << endl;
    cout << endl;
}

int main(void)
{
	Queue *queue = createQueue();
    cout << "Queue is created" << endl;
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

        if (cmd.substr(0, strlen("enque")) == string("enque"))
        {
			char str1[9];
			int qdata;
			sscanf(cmd.c_str(), "%s %d", str1, &qdata);
			enQueue(queue, qdata);
            continue;
        }

        if (cmd.substr(0, strlen("deque")) == string("deque"))
        {
			cout << cmd << " result " << deQueue(queue) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("empty")) == string("empty"))
        {
			cout << cmd << " result " << isEmpty(queue) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("rear")) == string("rear"))
        {
			cout << cmd << " result " << peekRear(queue) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("front")) == string("front"))
        {
			cout << cmd << " result " << peekFront(queue) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("size")) == string("size"))
        {
			cout << cmd << " result " << getSize(queue) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("traverse")) == string("traverse"))
        {
			cout << cmd << endl;
			traverseQueue(queue);
            continue;
        }

        cout << "Unsupported CMD, please check it carefully." << endl;
        printHelp();
    }

  	destroyQueue(queue);
    cout << "Queue is destroied\n";
}
