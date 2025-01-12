#include "LinkedList.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "Queue.h"
#include "Stack.h"
#include "User.h"
#include "Librarian.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Library {
private:
    LinkedList linkedList;
    HashTable hashTable;
    BinarySearchTree bst;
    Queue issueQueue;
    Stack undoStack;

    User users[max_size];
    int userCount;

public:
    Library() : userCount(0) {
        loadUsers();
        loadBooks();
    }

    ~Library() {
        cout << "Saving books and users..." << endl;
        saveUsers();
        saveBooks();
    }

    // File handling for users
    void loadUsers() {
        ifstream file("users.txt");
        if (!file) {
            cout << "No existing user data found. Starting with an empty user list." << endl;
            return;
        }

        string name, id, role, issuedBook;
        userCount = 0;
        while (file >> name >> id >> role) {
            if (file.peek() != '\n' && file.peek() != EOF) {
                file >> issuedBook;
            }
            else {
                issuedBook = "";
            }

            users[userCount] = User(name, id, role);
            users[userCount].issuedBook = issuedBook;
            userCount++;
        }

        file.close();
    }

    void saveUsers() {
        ofstream file("users.txt");
        if (!file) {
            cout << "Error: Unable to save users to file!" << endl;
            return;
        }

        for (int i = 0; i < userCount; i++) {
            file << users[i].name << " " << users[i].id << " " << users[i].role << " " << users[i].issuedBook << endl;
        }

        file.close();
        cout << "Users saved to file." << endl;
    }

    // File handling for books
    void loadBooks() {
        ifstream file("books.txt");
        if (!file) return;

        string title, author, ISBN;
        bool availability;
        while (file >> title >> author >> ISBN >> availability) {
            Book newBook(title, author, ISBN);
            newBook.availability = availability;
            addBook(newBook);
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        if (!file) {
            cout << "Error: Unable to create or open file: books.txt" << endl;
            return;
        }

        LinkedListNode* temp = linkedList.getHead();
        while (temp != nullptr) {
            file << temp->book.title << " " << temp->book.author << " " << temp->book.ISBN << " " << temp->book.availability << endl;
            temp = temp->next;
        }

        file.close();
    }

    // User management
    void addUser(string name, string id, string role) {
        if (userCount >= max_size) {
            cout << "User limit reached!" << endl;
            return;
        }

        users[userCount] = User(name, id, role);
        users[userCount].issuedBook = "";
        userCount++;
        cout << "User added: " << name << endl;

        saveUsers();
    }

    void displayUsers() {
        if (userCount == 0) {
            cout << "No users found!" << endl;
            return;
        }

        cout << "User Count: " << userCount << endl;

        for (int i = 0; i < userCount; i++) {
            cout << users[i].name << " " << users[i].id << " " << users[i].role;
            if (!users[i].issuedBook.empty()) {
                cout << " " << users[i].issuedBook;
            }
            cout << endl;
        }
    }

    void updateUser(string id, string newName, string newRole) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                users[i].name = newName;
                users[i].role = newRole;
                cout << "User updated: " << users[i].name << endl;
                return;
            }
        }
        cout << "User not found with ID: " << id << endl;
    }

    void deleteUser(string id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                cout << "User deleted: " << users[i].name << endl;
                for (int j = i; j < userCount - 1; j++) {
                    users[j] = users[j + 1];
                }
                userCount--;
                saveUsers();
                return;
            }
        }
        cout << "User not found with ID: " << id << endl;
    }

    User* findUserById(string id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                return &users[i];
            }
        }
        return nullptr;
    }

    // Issue a book to a user
    void issueBookToUser(string ISBN, string userId) {
        HashTableNode* book = hashTable.searchByISBN(ISBN);
        User* user = findUserById(userId);

        if (!book || !user) {
            cout << "Book or user not found!" << endl;
            return;
        }

        if (!book->book.availability) {
            cout << "Book is already issued!" << endl;
            return;
        }

        if (!user->issuedBook.empty()) {
            cout << "User already has a book issued!" << endl;
            return;
        }

        book->book.availability = false;
        user->issuedBook = ISBN;
        issueQueue.enqueue(new QueueNode(book->book));
        cout << "Book issued: " << book->book.title << " (ISBN: " << ISBN << ") to user: " << user->name << endl;

        saveUsers();
    }

    void searchByISBN(const string& ISBN) {
        HashTableNode* foundBook = hashTable.searchByISBN(ISBN);
        if (foundBook != nullptr) {
            cout << "Book found! ISBN: " << foundBook->book.ISBN << ", Title: " << foundBook->book.title
                << ", Author: " << foundBook->book.author << ", Availability: "
                << (foundBook->book.availability ? "Available" : "Issued") << endl;
        }
        else {
            cout << "Book not found with ISBN: " << ISBN << endl;
        }
    }

    // Return a book from a user
    void returnBookFromUser(string userId) {
        User* user = findUserById(userId);
        if (!user || user->issuedBook.empty()) {
            cout << "User has no book to return!" << endl;
            return;
        }

        HashTableNode* book = hashTable.searchByISBN(user->issuedBook);
        if (!book) {
            cout << "Book not found!" << endl;
            return;
        }

        book->book.availability = true;
        user->issuedBook = "";
        cout << "Book returned by user: " << user->name << endl;
    }

    // Add a book to the library
    void addBook(Book book) {
        linkedList.insert(book);
        bst.insert(book);
        hashTable.insert(book);

        Action* addAction = new Action(ADD, book);
        undoStack.push(addAction);

        cout << "Book added: " << book.title << endl;

        saveBooks();
    }

    // Delete a book from the library
    void deleteBook(const string& title) {

        cout << "Attempting to delete book with title: " << title << endl;

        LinkedListNode* bookNode = linkedList.getHead();
        while (bookNode && bookNode->book.title != title) {
            bookNode = bookNode->next;
        }

        if (bookNode) {
            string ISBN = bookNode->book.ISBN;
            linkedList.deleteBook(title);
            bst.deleteBook(title);
            hashTable.deleteBook(ISBN);

            Action* deleteAction = new Action(DELETE, bookNode->book);
            undoStack.push(deleteAction);

            cout << "Book deleted: " << title << endl;

            saveBooks();
        }
        else {
            cout << "Book not found: " << title << endl;
        }
    }

    // Display all books in the library
    void displayBooks() {
        linkedList.display();
    }

    // Display all issued books
    void displayIssuedBooks() {
        issueQueue.display();
    }

    // Undo the last action
    void undo() {
        if (undoStack.isEmpty()) {
            cout << "No actions to undo." << endl;
            return;
        }

        Action* lastAction = undoStack.pop();
        if (lastAction->type == ADD) {
            deleteBook(lastAction->book.title);
            cout << "Undo: Book '" << lastAction->book.title << "' removed." << endl;
        }
        else if (lastAction->type == DELETE) {
            addBook(lastAction->book);
            cout << "Undo: Book '" << lastAction->book.title << "' added back." << endl;
        }

        delete lastAction;
    }

    // Librarian login
    bool librarianLogin() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream file("librarians.txt");
        if (!file) {
            cout << "No librarians found. Please sign up." << endl;
            return false;
        }

        string storedUsername, storedPassword;
        while (file >> storedUsername >> storedPassword) {
            if (storedUsername == username && storedPassword == password) {
                cout << "Login successful! Welcome, " << username << "." << endl;
                file.close();
                return true;
            }
        }

        cout << "Invalid username or password." << endl;
        file.close();
        return false;
    }

    // Librarian signup
    void librarianSignup() {
        string username, password;
        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password: ";
        cin >> password;

        ofstream file("librarians.txt", ios::app);
        file << username << " " << password << endl;
        file.close();

        cout << "Signup successful! You can now log in." << endl;
    }

    void menu() {
        int choice;
        string title, author, ISBN, name, id, role;

        // Librarian login/signup
        do {
            cout << "\n\n\t\t\t\t\t\tLibrary Management System\n";
            cout << "1. Login\n";
            cout << "2. Signup\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                if (librarianLogin()) {
                    // Main menu after successful login
                    do {
                        cout << "\n\nMain Menu\n";
                        cout << "1. User Operations\n";
                        cout << "2. Book Operations\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                        case 1:
                            // User operations menu
                            do {
                                cout << "\n\nUser Operations\n";
                                cout << "1. Add User\n";
                                cout << "2. Update User\n";
                                cout << "3. Delete User\n";
                                cout << "4. Display All Users\n";
                                cout << "5. Issue Book to User\n";
                                cout << "6. Return Book from User\n";
                                cout << "7. Back to Main Menu\n";
                                cout << "Enter your choice: ";
                                cin >> choice;

                                switch (choice) {
                                case 1:
                                    cout << "Enter user name: ";
                                    cin.ignore();
                                    getline(cin, name);
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    cout << "Enter user role (student/faculty): ";
                                    cin >> role;
                                    addUser(name, id, role);
                                    break;

                                case 2:
                                    cout << "Enter user ID to update: ";
                                    cin >> id;
                                    cout << "Enter new name: ";
                                    cin.ignore();
                                    getline(cin, name);
                                    cout << "Enter new role (student/faculty): ";
                                    cin >> role;
                                    updateUser(id, name, role);
                                    break;

                                case 3:
                                    cout << "Enter user ID to delete: ";
                                    cin >> id;
                                    deleteUser(id);
                                    break;

                                case 4:
                                    displayUsers();
                                    break;

                                case 5:
                                    cout << "Enter ISBN of the book to issue: ";
                                    cin >> ISBN;
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    issueBookToUser(ISBN, id);
                                    break;

                                case 6:
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    returnBookFromUser(id);
                                    break;

                                case 7:
                                    cout << "Returning to main menu..." << endl;
                                    break;

                                default:
                                    cout << "Invalid choice! Please try again." << endl;
                                }

                                cout << "\nPress Enter to continue...";
                                cin.ignore();
                                cin.get();

                                system("CLS");

                            } while (choice != 7);
                            break;

                        case 2:
                            // Book operations menu
                            do {
                                cout << "\n\nBook Operations\n";
                                cout << "1. Add Book\n";
                                cout << "2. Delete Book\n";
                                cout << "3. Display All Books\n";
                                cout << "4. Display Issued Books\n";
                                cout << "5. Search Book by ISBN\n";
                                cout << "6. Search Book by Title\n";
                                cout << "7. Undo Last Action\n";
                                cout << "8. Back to Main Menu\n";
                                cout << "Enter your choice: ";
                                cin >> choice;

                                switch (choice) {
                                case 1:
                                    cout << "Enter book title: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    cout << "Enter author name: ";
                                    getline(cin, author);
                                    cout << "Enter ISBN: ";
                                    cin >> ISBN;
                                    addBook(Book(title, author, ISBN));
                                    break;

                                case 2:
                                    cout << "Enter book title to delete: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    deleteBook(title);
                                    break;

                                case 3:
                                    displayBooks();
                                    break;

                                case 4:
                                    displayIssuedBooks();
                                    break;

                                case 5:
                                    cout << "Enter ISBN of the book to search: ";
                                    cin >> ISBN;
                                    searchByISBN(ISBN);
                                    break;

                                case 6:
                                    cout << "Enter book title to search: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    if (bst.search(title)) {
                                        cout << "Book found!" << endl;
                                    }
                                    else {
                                        cout << "Book not found!" << endl;
                                    }
                                    break;

                                case 7:
                                    undo();
                                    break;

                                case 8:
                                    cout << "Returning to main menu..." << endl;
                                    break;

                                default:
                                    cout << "Invalid choice! Please try again." << endl;
                                }

                                cout << "\nPress Enter to continue...";
                                cin.ignore();
                                cin.get();

                                system("CLS");

                            } while (choice != 8);
                            break;

                        case 3:
                            cout << "Logging out..." << endl;
                            break;

                        default:
                            cout << "Invalid choice! Please try again." << endl;
                        }

                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();

                        system("CLS");

                    } while (choice != 3);
                }
                break;

            case 2:
                librarianSignup();
                break;

            case 3:
                cout << "Exiting the system. Thank you!" << endl;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();

            system("CLS");

        } while (choice != 3);
    }
};


int main() {
    Library lib;
    lib.menu();
    return 0;
}