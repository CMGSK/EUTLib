#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <list>
#include "Member.h"
#include "Sanction.h"
#include "Transaction.h"
#include "Book.h"
#include "sqlite3.h"

class Database {
private:

    bool ok_;
    char* err_;
    sqlite3* cx_;
    sqlite3_stmt* ptr;

public:

    Database();
    ~Database(){ if(ok_) sqlite3_close(cx_);};
    
    void printHelp();

    void idQuery(Book &target, int id);
    void idQuery(Transaction &target, int id);
    void idQuery(Member &target, int id);
    void idQuery(Sanction &target, int id);
    void query(std::list<Book> &target, std::string field, std::string filter, bool partialMatch);
    void query(std::list<Transaction> &target, std::string field, std::string filter, bool partialMatch);
    void query(std::list<Member> &target, std::string field, std::string filter, bool partialMatch);
    void query(std::list<Sanction> &target, std::string field, std::string filter, bool partialMatch);

    Book insertOrUpdate(const Book &book);
    Transaction insertOrUpdate(const Transaction &transaction);
    Member insertOrUpdate(const Member &member);
    Sanction insertOrUpdate(const Sanction &sanction);
    bool deleteFromDB(const Book &book);
    bool deleteFromDB(const Transaction &transaction);
    bool deleteFromDB(const Member &memeber);
    bool deleteFromDB(const Sanction &sanction);

};

#endif