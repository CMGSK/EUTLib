#include <exception>
#include <iostream>
#include <iterator>
#include <sqlite3.h>
#include <sstream>
#include "Database.h"
#include "Book.h"

static const std::string sql = "CREATE TABLE IF NOT EXISTS Books (" \
            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "title VARCHAR(80) NOT NULL," \
            "author VARCHAR(80)," \
            "genre VARCHAR(30)," \
            "ISBN VARCHAR(30) NOT NULL," \
            "year INT," \
            "available INT NOT NULL);" \
        "CREATE TABLE IF NOT EXISTS Members (" \
            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "name VARCHAR(80) NOT NULL," \
            "address VARCHAR(80) NOT NULL," \
            "email VARCHAR(80) NOT NULL," \
            "phone VARCHAR(30) NOT NULL," \
            "active BOOLEAN NOT NULL," \
            "restricted_until DATETIME NOT NULL);" \
        "CREATE TABLE IF NOT EXISTS Transactions (" \
            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "book_id INTEGER NOT NULL," \
            "member_id INTEGER NOT NULL," \
            "transaction_date DATETIME NOT NULL," \
            "due_date DATETIME NOT NULL," \
            "returning_date DATETIME NOT NULL," \
            "is_returned BOOLEAN NOT NULL);" \
        "CREATE TABLE IF NOT EXISTS Sanctions (" \
            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "member_id VARCHAR(80) NOT NULL," \
            "end_of_sanction VARCHAR(80) NOT NULL," \
            "is_active BOOLEAN NOT NULL);";


Database::Database(): err_(0), ok_(true){

    if (sqlite3_open("library.db", &cx_)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        ok_ = false;
        return;
    }
    std::cout << "Successful connection." << std::endl;


    if (sqlite3_exec(cx_, sql.c_str(), NULL, 0, &err_)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        ok_ = false;
    } else {
        std::cout << "Tables created." << std::endl;
    }

    
}


bool Database::insertOrUpdateBook(const Book &book){
    try{
        std::ostringstream os;
        os << "INSERT INTO Books (title, author, genre, ISBN, year, available) VALUES (" 
        << "'" << book.getTitle().c_str() << "',"
        << "'" << book.getAuthor().c_str() << "',"
        << "'" << book.getGenre().c_str() << "',"
        << "'" << book.getISBN().c_str() << "',"
        << book.getYear() << ","
        << book.getAvailable() << ");";
        if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
            std::cout << os.str() << std::endl;
            std::cout << "ha fallao tio mejora -->" << sqlite3_errmsg(cx_) << std::endl;
        }
    } catch (...) { return false; }
    return true;
}