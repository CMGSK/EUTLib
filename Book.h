#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private: 

    int id_;
    std::string title_;
    std::string author_;
    std::string genre_;
    std::string ISBN_;
    int year_;
    int available_;

public:

    Book();
    Book(std::string title, std::string ISBN, int available):
    id_(-1), title_(title), author_("null"), genre_("null"), ISBN_(ISBN), year_(0), available_(available)
    {};
    Book(int id, std::string title, std::string ISBN):
    id_(id), title_(title), ISBN_(ISBN) 
    {};

    void setId(int n);
    void setTitle(std::string s);
    void setAuthor(std::string s);
    void setGenre(std::string s);
    void setISBN(std::string s);
    void setYear(int n);
    void setAvailable(int n);
    void dropAvailable() {available_--;};
    void addAvailable() {available_++;};
    int getId() const {return id_;};
    std::string getTitle() const {return title_;};
    std::string getAuthor() const {return author_;};
    std::string getGenre() const {return genre_;};
    std::string getISBN() const {return ISBN_;};
    int getYear() const {return year_;};
    int getAvailable() const {return available_;};

};

#endif