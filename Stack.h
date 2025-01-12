#ifndef STACK_H
#define STACK_H

#include "Book.h"
#include <iostream>
using namespace std;

enum ActionType { ADD, DELETE };

struct Action {
    ActionType type;
    Book book;

    Action(ActionType t, Book b) : type(t), book(b) {}
};

struct StackNode {
    Action* action;
    StackNode* next;

    StackNode(Action* a) : action(a), next(nullptr) {}
};

class Stack {
private:
    StackNode* top;

public:
    Stack() : top(nullptr) {}

    bool isEmpty() {
        return top == nullptr;
    }

    void push(Action* action) {
        StackNode* newNode = new StackNode(action);
        newNode->next = top;
        top = newNode;
    }

    Action* pop() {
        if (isEmpty()) {
            cout << "Stack Underflow" << endl;
            return nullptr;
        }
        StackNode* temp = top;
        Action* action = temp->action;
        top = top->next;
        delete temp;
        return action;
    }

    void display() {
        StackNode* temp = top;
        while (temp != nullptr) {
            cout << (temp->action->type == ADD ? "Add " : "Delete ")
                << "Book - Title: " << temp->action->book.title
                << ", ISBN: " << temp->action->book.ISBN << endl;
            temp = temp->next;
        }
    }
};

#endif
