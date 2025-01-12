#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Book.h"
#include <iostream>
using namespace std;

struct HashTableNode {
    Book book;
    HashTableNode* next;

    HashTableNode(Book b) : book(b), next(nullptr) {}
};

class HashTable {
private:
    static const int TABLE_SIZE = 10;
    HashTableNode* table[TABLE_SIZE];

    int hashISBN(const string& ISBN) {
        long int hashValue = 0;
        for (char c : ISBN) {
            hashValue += c;
        }
        return hashValue % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void insert(Book book) {
        int index = hashISBN(book.ISBN);
        HashTableNode* newNode = new HashTableNode(book);
        newNode->next = table[index];
        table[index] = newNode;
    }

    HashTableNode* searchByISBN(const string& ISBN) {
        int index = hashISBN(ISBN);
        HashTableNode* current = table[index];

        while (current != nullptr) {
            if (current->book.ISBN == ISBN) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    void deleteBook(const string& ISBN) {
        int index = hashISBN(ISBN);
        HashTableNode* current = table[index];
        HashTableNode* previous = nullptr;

        while (current != nullptr) {
            if (current->book.ISBN == ISBN) {
                if (previous == nullptr) {
                    table[index] = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;    // Free memory of the deleted node
                current = nullptr; // Avoid dangling pointer
                cout << "Book with ISBN " << ISBN << " deleted from hash table." << endl;
                return;
            }
            previous = current;
            current = current->next;
        }
        cout << "Book with ISBN " << ISBN << " not found in hash table." << endl;
    }

    void display() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashTableNode* current = table[i];
            while (current != nullptr) {
                cout << "ISBN: " << current->book.ISBN << ", Title: " << current->book.title
                    << ", Author: " << current->book.author << endl;
                current = current->next;
            }
        }
    }
};

#endif
