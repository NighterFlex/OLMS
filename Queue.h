#ifndef QUEUE_H
#define QUEUE_H

#include "Book.h"
#include <iostream>
using namespace std;

const int max_size = 10;

struct QueueNode {
    Book book;
    QueueNode* next;

    QueueNode(Book b) : book(b), next(nullptr) {}
};

class Queue {
private:
    int front;
    int rear;
    QueueNode* arr[max_size];

public:
    Queue() : front(-1), rear(-1) {}

    bool isEmpty() {
        return front == -1 && rear == -1;
    }

    bool isFull() {
        return rear == max_size - 1;
    }

    void enqueue(QueueNode* val) {
        if (isFull()) {
            cout << "Issuing queue is full! :(" << endl;
            return;
        }
        if (isEmpty()) {
            rear = 0;
            front = 0;
        }
        else {
            rear++;
        }
        arr[rear] = val;
    }

    QueueNode* dequeue() {
        if (isEmpty()) {
            cout << "Issuing queue is empty! :D" << endl;
            return nullptr;
        }
        QueueNode* x = arr[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front++;
        }
        return x;
    }

    int count() {
        return rear - front + 1;
    }

    void display() {
        if (isEmpty()) {
            cout << "No books in the issuing queue." << endl;
            return;
        }
        for (int i = front; i <= rear; i++) {
            cout << "Title: " << arr[i]->book.title << ", ISBN: " << arr[i]->book.ISBN << endl;
        }
    }
};

#endif