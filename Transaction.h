#include <string>
#include <iostream>

#include "LibraryRepository.h"

class Transaction : public LibraryRepository {
private: 

    int bookId_;
    int memberId_;
    std::string transactionDate_;
    std::string dueDate_;
    std::string returningDate_;
    bool isReturned_;

public:

    Transaction() {};
    Transaction(bool isUpdate);
    Transaction(int bookId, int memberId, std::string transactionDate, std::string dueDate, std::string returningDate, bool isReturned, int id = 0);
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

    std::string toString() override;

    std::string getUpdateQry() const;

    std::string getInsertQry() const;
};

