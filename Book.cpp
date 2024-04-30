#include <iostream>
#include <string>

#include "Commands.h"
#include "Book.h"


Book::Book(std::string title, std::string author, std::string genre, std::string ISBN, int year, int available, int id){
    setId(id);
    title_ = title == "" ? "null" : title;
    author_ = author == "" ? "null" : author;
    genre_ = genre == "" ? "null" : genre;
    ISBN_ = ISBN == "" ? "null" : ISBN;
    year_ = year; 
    available_ = available; 
}

Book::Book(bool isUpdate){
    std::string input = "";
    if (isUpdate){
        std::cout << "Insert the ID.\n> ";
        std::getline(std::cin, input);
        setId(input != "" ? stoi(input) : -1);
        input = "";
    }
    std::cout << "Insert a title for the book.\n> ";
    std::getline(std::cin, input);
    title_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert an author for the book. \n> ";
    std::getline(std::cin, input);
    author_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a genre for the book. \n> ";
    std::getline(std::cin, input);
    genre_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert an ISBN for the book. \n> ";
    std::getline(std::cin, input);
    ISBN_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a year for the book. \n> ";
    std::getline(std::cin, input);
    ISBN_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert the ammount of books. \n> ";
    std::getline(std::cin, input);
    available_ = input != "" ? stoi(input) : 0;
}

std::string Book::toString() {
    std::ostringstream os;
    os << "Book id: " << this->getId() << '\n'
    << "--------" << '\n'
    << '\t' << "ISBN: " << ISBN_ << '\n'
    << '\t' << "Title: " << title_ << '\n'
    << '\t' << "Author: " << author_ << '\n'
    << '\t' << "Genre: " << genre_ << '\n'
    << '\t' << "Year: " << year_ << '\n'
    << '\t' << "Availability: x" << available_ << std::endl;
    return os.str();
};

std::string Book::getUpdateQry() const {
    std::ostringstream os;
    os << "UPDATE Books SET ";
    if (title_ != "")
        os << "title = '" << title_ << "'";
    if (author_ != "")
        os << ",author = '" << author_ << "'";
    if (genre_ != "")
        os << ",genre = '" << genre_ << "'";
    if (year_)
        os << ",year = '" << year_ << "' ";
    os << "WHERE id = " << getId();
    return os.str();
}

std::string Book::getInsertQry() const{
    std::ostringstream os;
    os << "INSERT INTO Books (title, author, genre, ISBN, year, available) VALUES (" 
    << "'" << title_ << "',"
    << "'" << author_ << "',"
    << "'" << genre_ << "',"
    << "'" << ISBN_ << "',"
    << "'" << year_ << "',"
    << "'" << available_ << "');";
    return os.str();
}
