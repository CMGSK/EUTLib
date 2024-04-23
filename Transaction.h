#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private: 

    int id_;
    int bookId_;
    int memberId_;
    std::string transactionDate_;
    std::string dueDate_;
    std::string returningDate_;
    bool isReturned_;

public:

    Transaction();
    Transaction(int bookId, int memberId, std::string transactionDate, std::string dueDate, std::string returningDate):
    bookId_(bookId), memberId_(memberId), transactionDate_(transactionDate), dueDate_(dueDate), returningDate_(returningDate), isReturned_(false)
    {};
    Transaction(int id, int bookId, int memberId):
    id_(id), bookId_(bookId), memberId_(memberId)
    {};
    

    void markAsReturned();
    int getId() const {return id_;};
    int getBookId() const {return bookId_;};
    int getMemberId() const {return memberId_;};
    std::string getTransactionDate() const {return transactionDate_;};
    std::string getDueDate () const {return dueDate_;};
    std::string getReturningDate() const {return returningDate_;};
    int getIsReturned() const {return isReturned_;};

};

#endif