#ifndef DATABASE_H
#define DATABASE_H

#include <string>
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

public:

    Database();
    ~Database(){ if(ok_) sqlite3_close(cx_);};
    
    void printHelp();
    void query(std::string target, std::string field, std::string filter);

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