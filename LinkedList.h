#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Book.h"
#include <iostream>
using namespace std;

struct LinkedListNode {
    Book book;
    LinkedListNode* next;

    LinkedListNode(Book b) : book(b), next(nullptr) {}
};

class LinkedList {
private:
    LinkedListNode* head;

public:
    LinkedList() : head(nullptr) {}

    void insert(Book book) {
        LinkedListNode* newNode = new LinkedListNode(book);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            LinkedListNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void deleteBook(const string& title) {
        if (head == nullptr) {
            cout << "Linked list is empty. No book to delete." << endl;
            return;
        }

        // Case 1: Delete the head node
        if (head->book.title == title) {
            LinkedListNode* temp = head;
            head = head->next;  // Update head to the next node
            delete temp;        // Free memory of the deleted node
            temp = nullptr;     // Avoid dangling pointer
            cout << "Book deleted from Linked list: " << title << endl;
            return;
        }

        // Case 2: Delete a node in the middle or at the end
        LinkedListNode* temp = head;
        while (temp->next != nullptr && temp->next->book.title != title) {
            temp = temp->next;
        }

        if (temp->next == nullptr) {
            cout << "Book not found in Linked list: " << title << endl;
            return;
        }

        // Book found, delete it
        LinkedListNode* bookToDelete = temp->next;
        temp->next = temp->next->next;  // Bypass the node to be deleted
        delete bookToDelete;            // Free memory of the deleted node
        bookToDelete = nullptr;         // Avoid dangling pointer
        cout << "Book deleted from Linked list: " << title << endl;
    }

    void display() {
        LinkedListNode* temp = head;
        while (temp) {
            cout << "Title: " << temp->book.title << ", Author: " << temp->book.author
                << ", ISBN: " << temp->book.ISBN << ", Availability: "
                << (temp->book.availability ? "Available" : "Issued") << endl;
            temp = temp->next;
        }
        cout << "No Books to display." << endl;
    }

    LinkedListNode* getHead() {
        return head;
    }
};

#endif
