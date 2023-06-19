/*CSCI Final Project || Library Management System
This program is a simple library management system that allows users to search, borrow, and return books. It reads a list of books from a text file and stores them in a library class. Users can interact with the program through a menu-driven interface, which offers options to search for a book by title, author, or ISBN, borrow a book, return a book, and check the number of days left until a borrowed book is due.

The search functionality supports case-insensitive and partial matches. When a user borrows a book, the program sets the due date to 14 days from the borrowing date and associates the book with the user. Users can then return the book or check the remaining days until the book is due back.
 [UPDATE]: menu driven
 Date: 4/25/2023
 Author: Charles Bedecki
 Details:
 - added a menu driven functionality to the basic program. Menu options include: Search for a book, Borrow a book, Return a book, Check days left, and Exit.
 (May need to add a function to write user names to a seperate txt file for when the user is prompted to enter name to borrow a book. also track the days left for each checked out book per user in the txt file?)

 [UPDATE]: worked on input validation
 Date: 5/8/2023
 Author: Charles Bedecki
 Details:
 - ISBN validation: ensure that input is a string of numbers or dashes
 - User's name: validate that input is a non-empty string || validate non-character input
*/
#ifndef library_h
#define library_h
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

struct Book
{
    string title;
    string author;
    string ISBN;
    bool is_available;
    time_t due_date;
    string user;
};

class Library
{
    vector<Book> books;

public:
    // Add a book to the library
    void add_book(const Book &book)
    {
        books.push_back(book);
    }

    // Search for books by title, author, or ISBN (case-insensitive and partial matches)
    vector<Book> search_books(const string &query)
    {
        vector<Book> results;
        string query_lower = query;
        transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);

        for (const auto &book : books)
        {
            string title_lower = book.title;
            string author_lower = book.author;
            string ISBN_lower = book.ISBN;

            transform(title_lower.begin(), title_lower.end(), title_lower.begin(), ::tolower);
            transform(author_lower.begin(), author_lower.end(), author_lower.begin(), ::tolower);
            transform(ISBN_lower.begin(), ISBN_lower.end(), ISBN_lower.begin(), ::tolower);

            if (title_lower.find(query_lower) != string::npos ||
                author_lower.find(query_lower) != string::npos ||
                ISBN_lower.find(query_lower) != string::npos)
            {
                results.push_back(book);
            }
        }
        return results;
    }

    // Borrow a book by ISBN, and set the due date 14 days from now
    bool borrow_book(const string &ISBN, const string &user)
    {
        for (auto &book : books)
        {
            if (book.ISBN == ISBN && book.is_available)
            {
                book.is_available = false;
                book.due_date = time(nullptr) + 14 * 24 * 60 * 60; // 14 days from now
                book.user = user;                                  // Store the user who borrowed the book
                return true;
            }
        }
        return false;
    }

    // Return a book by ISBN
    bool return_book(const string &ISBN)
    {
        for (auto &book : books)
        {
            if (book.ISBN == ISBN && !book.is_available)
            {
                book.is_available = true;
                book.user = ""; // Clear the user field
                return true;
            }
        }
        return false;
    }

    // Check how many days are left for a borrowed book by ISBN
    int days_left(const string &ISBN)
    {
        for (const auto &book : books)
        {
            if (book.ISBN == ISBN && !book.is_available)
            {
                int days_remaining = (book.due_date - time(nullptr)) / (24 * 60 * 60);
                return days_remaining >= 0 ? days_remaining : 0;
            }
        }
        return -1;
    }
};

// Load books from the given txt file
void load_books_from_file(const string &filename, Library &library)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            size_t semicolon1 = line.find(';');
            size_t semicolon2 = line.find(';', semicolon1 + 1);
            string title = line.substr(0, semicolon1);
            string author = line.substr(semicolon1 + 1, semicolon2 - semicolon1 - 1);
            string ISBN = line.substr(semicolon2 + 1);
            library.add_book({title, author, ISBN, true});
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }
}

// Display the main menu options
void display_menu()
{
    cout << "\nLibrary Management System" << endl;
    cout << "1. Search for a book" << endl;
    cout << "2. Borrow a book" << endl;
    cout << "3. Return a book" << endl;
    cout << "4. Check days left" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

// Search for a book by title, author, or ISBN
void search_for_book(Library &library)
{
    string query;
    cout << "Enter a book title, author, or ISBN to search: ";
    getline(cin, query);

    auto results = library.search_books(query);

    if (results.empty())
    {
        cout << "No books found." << endl;
    }
    else
    {
        for (const auto &book : results)
        {
            cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN
                 << ", Availability: " << (book.is_available ? "Available" : "Not available") << endl;
        }
    }
}

// Check how many days are left on a borrowed book by ISBN
void check_days_left(Library &library)
{
    cout << "Enter the ISBN of the book you want to check the remaining days: ";
    string ISBN;
    getline(cin, ISBN);

    int days_left = library.days_left(ISBN);
    if (days_left >= 0)
    {
        cout << "You have " << days_left << " day(s) left to return the book." << endl;
    }
    else
    {
        cout << "Book not found or not checked out." << endl;
    }
}

// Borrow a book by ISBN
void borrow_book(Library &library)
{
    cout << "Enter the ISBN of the book you want to borrow: ";
    string ISBN;
    getline(cin, ISBN);

    cout << "Enter your name: ";
    string user;
    getline(cin, user);

    if (library.borrow_book(ISBN, user))
    {
        cout << "Book successfully borrowed." << endl;
    }
    else
    {
        cout << "Book is not available or does not exist." << endl;
    }
}

// Return a book by ISBN
void return_book(Library &library)
{
    cout << "Enter the ISBN of the book you want to return: ";
    string ISBN;
    getline(cin, ISBN);

    if (library.return_book(ISBN))
    {
        cout << "Book successfully returned." << endl;
    }
    else
    {
        cout << "Book not found or not checked out." << endl;
    }
}
#endif