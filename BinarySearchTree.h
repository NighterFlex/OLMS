#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "Book.h"
#include <iostream>
using namespace std;

struct BSTNode {
    Book book;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Book b) : book(b), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    BSTNode* root;

    BSTNode* minValueNode(BSTNode* node) {
        BSTNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(Book book) {
        BSTNode* newNode = new BSTNode(book);
        if (root == nullptr) {
            root = newNode;
            return;
        }

        BSTNode* temp = root;
        while (true) {
            if (newNode->book.title == temp->book.title) {
                return;  // Duplicate titles are not allowed.
            }
            if (newNode->book.title < temp->book.title) {
                if (temp->left == nullptr) {
                    temp->left = newNode;
                    return;
                }
                temp = temp->left;
            }
            else {
                if (temp->right == nullptr) {
                    temp->right = newNode;
                    return;
                }
                temp = temp->right;
            }
        }
    }

    BSTNode* search(const string& title) {
        BSTNode* temp = root;
        while (temp != nullptr) {
            if (title == temp->book.title) {
                return temp;
            }
            else if (title < temp->book.title) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return nullptr;
    }

    void deleteBook(const string& title) {
        BSTNode* parent = nullptr;
        BSTNode* temp = root;

        // Find the node to delete and its parent.
        while (temp != nullptr && temp->book.title != title) {
            parent = temp;
            if (title < temp->book.title) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }

        if (temp == nullptr) {
            return;
        }

        // Case 1: Node has no children or only one child.
        if (temp->left == nullptr) {
            BSTNode* child = temp->right;
            if (parent == nullptr) {
                root = child;
            }
            else if (parent->left == temp) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }
            delete temp;
        }
        else if (temp->right == nullptr) {
            BSTNode* child = temp->left;
            if (parent == nullptr) {
                root = child;
            }
            else if (parent->left == temp) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }
            delete temp;
        }
        // Case 2: Node has two children.
        else {
            BSTNode* successor = minValueNode(temp->right);
            temp->book = successor->book;
            string successorTitle = successor->book.title;
            deleteBook(successorTitle);
        }

        cout << "Book deleted from BST." << endl;
    }
};

#endif