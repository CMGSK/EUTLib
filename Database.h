#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "sqlite3.h"
#include "Book.h"

class Database {
private:
    bool ok_;
    char* err_;
    sqlite3* cx_;
public:
    Database();
    ~Database(){ if(ok_) sqlite3_close(cx_);};
    void printHelp();
    bool insertOrUpdate(const Book &book);
    // bool insertOrUpdate();
    // bool insertOrUpdate();
    // bool insertOrUpdate();
    void query(std::string target, std::string field, std::string filter);
    bool deleteEntry(const Book &book);
    // bool delete();
    // bool delete();
    // bool delete();

};

#endif