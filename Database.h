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

    void query(std::list<void*> &target, std::string from, std::string filter, std::string value, bool partialMatch, bool isTotal);

    Book insertOrUpdate(const Book &book);
    Transaction insertOrUpdate(const Transaction &transaction);
    Member insertOrUpdate(const Member &member);
    Sanction insertOrUpdate(const Sanction &sanction);

    bool deleteSingle(std::string table, int id);
    bool deleteBulk(std::string table, int id[], int arrSize);

};

#endif