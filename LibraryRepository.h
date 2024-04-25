#ifndef LIBRARYREPOSITORY_H
#define LIBRARYREPOSITORY_H

#include <memory>
#include <string>
#include <sstream>

class LibraryRepository {

private:
    int id_;

public:

    LibraryRepository(): id_(-1){};
    LibraryRepository(int id): id_(id){};

    int getId() const {return id_;};
    void setId(int id){id_ = id;};

    virtual std::string toString(){ return nullptr; };

};

class Sanction : public LibraryRepository {
private: 

    int memberId_;
    std::string endOfSanction_;
    bool isActive_;

public:

    Sanction() {}
    Sanction(int memberId, std::string endOfSanction):
    LibraryRepository(), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};
    Sanction(int id, int memberId, std::string endOfSanction):
    LibraryRepository(id), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};

    void setMemberId(int n) {memberId_ = n;};
    void setEndOfSanction(std::string s) { endOfSanction_ = s;};
    void setIsActive(bool b) {isActive_ = b;};
    void markAsFinished(){isActive_ = false;};

    int getMemberId() const {return memberId_;};
    std::string getEndOfSanction()  const {return endOfSanction_;};
    bool getIsActive() const {return isActive_;};

    std::string toString() override {
       std::ostringstream os;
       os << "Sanction id: " << this->getId() << '\n'
       << "On member: " << memberId_ << '\n'
       << "----------" << '\n'
       << '\t' << "End of sanction: " << endOfSanction_ << '\n'
       << '\t' << "Is active: " << isActive_ << std::endl;
       return os.str();
    };
};

class Member : public LibraryRepository {
private: 

    std::string name_;
    std::string address_;
    std::string email_;
    std::string phone_;
    std::string restrictedUntil_;
    bool active_;

public:

    Member() {}
    Member(std::string name, std::string address, std::string email, std::string phone):
    LibraryRepository(), name_(name), address_(address), email_(email), phone_(phone)
    {};
    Member(int id, std::string name):
    LibraryRepository(id), name_(name) 
    {};

    void setName(std::string s) {name_ = s;};
    void setAddress(std::string s) {address_ = s;};
    void setEmail(std::string s) {email_ = s;};
    void setPhone(std::string s) {phone_ =s;};
    void setRestriction(std::string s) {restrictedUntil_ = s;};
    void setActive(bool b) {active_ = b;};
    std::string getName() const {return name_;};
    std::string getAddress() const {return address_;};
    std::string getEmail() const {return email_;};
    std::string getPhone () const {return phone_;};
    std::string getRestrictedUntil() const {return restrictedUntil_;};
    bool getActive() const {return active_;};

    std::string toString() override {
       std::ostringstream os;
       os << "Member id: " << this->getId() << '\n'
       << "--------" << '\n'
       << '\t' << "Name: " << name_ << '\n'
       << '\t' << "Address: " << address_ << '\n'
       << '\t' << "Email: " << email_ << '\n'
       << '\t' << "Phone: " << phone_ << '\n'
       << '\t' << "Current restriction: " << restrictedUntil_ << '\n'
       << '\t' << "Active user: " << active_ << std::endl;
       return os.str();
    };

};

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

    std::string toString() override {
       std::ostringstream os;
       os << "Book id: " << this->getId() << '\n'
       << "--------" << '\n'
       << '\t' << "ISBN: " << ISBN_ << '\n'
       << '\t' << "Title: " << title_ << '\n'
       << '\t' << "Author: " << author_ << '\n'
       << '\t' << "Genre: " << genre_ << '\n'
       << '\t' << "Year: " << year_ << '\n'
       << '\t' << "Availavility: x" << available_ << std::endl;
       return os.str();
    };

};

class Transaction : public LibraryRepository {
private: 

    int bookId_;
    int memberId_;
    std::string transactionDate_;
    std::string dueDate_;
    std::string returningDate_;
    bool isReturned_;

public:

    Transaction() {}
    Transaction(int bookId, int memberId, std::string transactionDate, std::string dueDate, std::string returningDate):
    LibraryRepository(), bookId_(bookId), memberId_(memberId), transactionDate_(transactionDate), dueDate_(dueDate), returningDate_(returningDate), isReturned_(false)
    {};
    Transaction(int id, int bookId, int memberId):
    LibraryRepository(id), bookId_(bookId), memberId_(memberId)
    {};
    

    void markAsReturned() {isReturned_ = true;};
    int getBookId() const {return bookId_;};
    int getMemberId() const {return memberId_;};
    std::string getTransactionDate() const {return transactionDate_;};
    std::string getDueDate () const {return dueDate_;};
    std::string getReturningDate() const {return returningDate_;};
    int getIsReturned() const {return isReturned_;};

    void setBookId(int n) { bookId_ = n; };
    void setMemberId(int n) { memberId_ = n; };
    void setTransactionDate(std::string s) { transactionDate_ = s; };
    void setDueDate(std::string s) { dueDate_ = s; };
    void setReturningDate(std::string s) { returningDate_ = s; };
    void setIsReturned(bool b) { isReturned_ = b; };

    std::string toString() override {
       std::ostringstream os;
       os << "Member id: " << this->getId() << '\n'
       << " Relation Member-Book -> " << bookId_ << " - " << memberId_ << '\n'
       << " --------------------" << '\n'
       << '\t' << "Transaction date: " << transactionDate_ << '\n'
       << '\t' << "Due date: " << dueDate_ << '\n'
       << '\t' << "Returning date: " << returningDate_ << '\n'
       << '\t' << "Has been returned: " << isReturned_ << std::endl;
       return os.str();
    };
};

#endif