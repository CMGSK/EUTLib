#include "LibraryRepository.h"

class Book : public LibraryRepository{
private: 

    std::string title_;
    std::string author_;
    std::string genre_;
    std::string ISBN_;
    int year_;
    int available_;

public:

    Book() {}
    Book(bool fromStdi);
    Book(std::string title, std::string ISBN, std::string genre, std::string author, int year, int available, int id = 0);
    Book(std::string title, std::string ISBN, int available):
    LibraryRepository(), title_(title), author_("null"), genre_("null"), ISBN_(ISBN), year_(0), available_(available)
    {};
    Book(int id, std::string title, std::string ISBN):
    LibraryRepository(id), title_(title), ISBN_(ISBN) 
    {};

    void setTitle(std::string s) {title_ = s;};
    void setAuthor(std::string s) {author_ = s;};
    void setGenre(std::string s) {genre_ = s;};
    void setISBN(std::string s) {ISBN_ = s;};
    void setYear(int n) {year_ = n;};
    void setAvailable(int n) {available_ = n;};
    void dropAvailable() {available_--;};
    void addAvailable() {available_++;};

    std::string getTitle() const {return title_;};
    std::string getAuthor() const {return author_;};
    std::string getGenre() const {return genre_;};
    std::string getISBN() const {return ISBN_;};
    int getYear() const {return year_;};
    int getAvailable() const {return available_;};

    std::string toString() override;
    std::string getUpdateQry() const;
    std::string getInsertQry() const;
    static void execSelect(bool isTotal);

};

