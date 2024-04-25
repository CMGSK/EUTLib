#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "LibraryRepository.h"
#include <string>
#include <sstream>

class Transaction : public LibraryRepository {
private: 

    int bookId_;
    int memberId_;
    std::string transactionDate_;
    std::string dueDate_;
    std::string returningDate_;
    bool isReturned_;

public:

    Transaction();
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

    void setBookId(int n);
    void setMemberId(int n);
    void setTransactionDate(std::string s);
    void setDueDate(std::string s);
    void setReturningDate(std::string s);
    void setIsReturned(bool b);

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