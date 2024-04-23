#include <exception>
#include <iostream>
#include <iterator>
#include <sqlite3.h>
#include <sstream>
#include "Database.h"
#include "Book.h"
#include "Member.h"
#include "Sanction.h"
#include "Transaction.h"

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


Book Database::insertOrUpdate(const Book &book){
    Book result;
    //Insertion
    try{
        std::ostringstream os;
        os << "INSERT INTO Books (title, author, genre, ISBN, year, available) VALUES (" 
        << "'" << book.getTitle().c_str() << "',"
        << "'" << book.getAuthor().c_str() << "',"
        << "'" << book.getGenre().c_str() << "',"
        << "'" << book.getISBN().c_str() << "',"
        << "'" << book.getYear() << ","
        << "'" << book.getAvailable() << ");";
        if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
            std::cout << os.str() << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        } else {
            result = *new Book(sqlite3_last_insert_rowid(cx_),
                book.getTitle(),
                book.getISBN());
        }
    } catch (...) { return result; }
    return result;
}

Transaction Database::insertOrUpdate(const Transaction &transaction){
    Transaction result;
    //Insertion
    try{
        std::ostringstream os;
        os << "INSERT INTO Transactions (book_id, member_id, transaction_date, due_date, returning_date, is_returned) VALUES (" 
        << "'" << transaction.getBookId() << "',"
        << "'" << transaction.getMemberId() << "',"
        << "'" << transaction.getTransactionDate().c_str() << "',"
        << "'" << transaction.getDueDate().c_str() << "',"
        << "'" << transaction.getReturningDate().c_str() << "');";
        if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
            std::cout << os.str() << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        } else {
            result = *new Transaction(sqlite3_last_insert_rowid(cx_),
                transaction.getBookId(),
                transaction.getMemberId());
        }
    } catch (...) { return result; }
    return result;
}

Member Database::insertOrUpdate(const Member &member){
    Member result;
    //Insertion
    try{
        std::ostringstream os;
        os << "INSERT INTO Members (name, address, email, phone, active, restricted_until) VALUES (" 
        << "'" << member.getName().c_str() << "',"
        << "'" << member.getAddress().c_str() << "',"
        << "'" << member.getEmail().c_str() << "',"
        << "'" << member.getPhone().c_str() << "',"
        << "'" << member.getRestrictedUntil().c_str() << "');";
        if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
            std::cout << os.str() << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        } else {
            result = *new Member(sqlite3_last_insert_rowid(cx_),
                member.getName());
        }
    } catch (...) { return result; }
    return result;
}

Sanction Database::insertOrUpdate(const Sanction &sanction){
    Sanction result;
    //Insertion
    try{
        std::ostringstream os;
        os << "INSERT INTO Sanctions (member_id, end_of_sanction, is_active) VALUES (" 
        << "'" << sanction.getMemberId() << "',"
        << "'" << sanction.getEndOfSanction().c_str() << "',"
        << "'" << sanction.getIsActive() << "');";
        if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
            std::cout << os.str() << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        } else {
            result = *new Sanction(sqlite3_last_insert_rowid(cx_),
                sanction.getName());
        }
    } catch (...) { return result; }
    return result;
}