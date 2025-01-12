#pragma once
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct User {
    string name;
    string id;
    string role; // "student" or "faculty"
    string issuedBook; // Track which book is issued to the user

    User() : name(""), id(""), role(""), issuedBook("") {}

    User(string n, string i, string r) : name(n), id(i), role(r), issuedBook("") {}
};

#endif
