#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

struct Book {
    string title;
    string author;
    string ISBN;
    bool availability;

    Book() : title(""), author(""), ISBN(""), availability(true) {}

    Book(string t, string a, string i) : title(t), author(a), ISBN(i), availability(true) {}
};

#endif