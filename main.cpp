/*CSCI Final Project || Library Management System
This program is a simple library management system that allows users to search, borrow, and return books. It reads a list of books from a text file and stores them in a library class. Users can interact with the program through a menu-driven interface, which offers options to search for a book by title, author, or ISBN, borrow a book, return a book, and check the number of days left until a borrowed book is due.

The search functionality supports case-insensitive and partial matches. When a user borrows a book, the program sets the due date to 14 days from the borrowing date and associates the book with the user. Users can then return the book or check the remaining days until the book is due back.
 [UPDATE]: menu driven
 Date: 4/25/2023
 Author: Charles Bedecki
 Details:
 - added a menu driven functionality to the basic program. Menu options include: Search for a book, Borrow a book, Return a book, Check days left, and Exit.
 (May need to add a function to write user names to a seperate txt file for when the user is prompted to enter name to borrow a book. also track the days left for each checked out book per user in the txt file?)
*/
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "library.h"

using namespace std;

int main()
{
    Library library;
    load_books_from_file("books.txt", library);

    while (true)
    {
        display_menu();
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            search_for_book(library);
        }
        else if (choice == 2)
        {
            borrow_book(library);
        }
        else if (choice == 3)
        {
            return_book(library);
        }
        else if (choice == 4)
        {
            check_days_left(library);
        }
        else if (choice == 5)
        {
            cout << "Exiting the program..." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}