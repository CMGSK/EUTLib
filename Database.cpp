#include <iostream>
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

//Selects
void Database::idQuery (Book &target, int id){
    std::ostringstream os;
    os << "SELECT * from Books WHERE id = " << id;

    if(sqlite3_prepare_v2(cx_, sql.c_str(), -1, &ptr, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    if (sqlite3_step(ptr) == SQLITE_ROW){
        target.setId(sqlite3_column_int(ptr, 0));
        target.setTitle(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
        target.setAuthor(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
        target.setGenre(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
        target.setYear(sqlite3_column_int(ptr, 4));
        target.setAvailable(sqlite3_column_int(ptr, 5));
    } else {
        std::cout << sqlite3_errmsg(cx_) << std::endl << "No books with such ID" << std::endl;
    }
}

void Database::idQuery (Transaction &target, int id){
    std::ostringstream os;
    os << "SELECT * from Transactions WHERE id = " << id;

    if(sqlite3_prepare_v2(cx_, sql.c_str(), -1, &ptr, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    if (sqlite3_step(ptr) == SQLITE_ROW){
        target.setId(sqlite3_column_int(ptr, 0));
        target.setBookId(sqlite3_column_int(ptr, 1));
        target.setMemberId(sqlite3_column_int(ptr, 2));
        target.setTransactionDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
        target.setDueDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
        target.setReturningDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
        target.setIsReturned(sqlite3_column_int(ptr, 6));
    } else {
        std::cout << sqlite3_errmsg(cx_) << std::endl << "No transactions with such ID" << std::endl;
    }
}

void Database::idQuery (Member &target, int id){
    std::ostringstream os;
    os << "SELECT * from Members WHERE id = " << id;

    if(sqlite3_prepare_v2(cx_, sql.c_str(), -1, &ptr, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    if (sqlite3_step(ptr) == SQLITE_ROW){
        target.setId(sqlite3_column_int(ptr, 0));
        target.setName(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
        target.setAddress(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
        target.setEmail(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
        target.setPhone(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
        target.setRestriction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
        target.setActive(sqlite3_column_int(ptr, 6));
    } else {
        std::cout << sqlite3_errmsg(cx_) << std::endl << "No members with such ID" << std::endl;
    }
}

void Database::idQuery (Sanction &target, int id){
    std::ostringstream os;
    os << "SELECT * from Sanctions WHERE id = " << id;

    if(sqlite3_prepare_v2(cx_, sql.c_str(), -1, &ptr, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    if (sqlite3_step(ptr) == SQLITE_ROW){
        target.setId(sqlite3_column_int(ptr, 0));
        target.setMemberId(sqlite3_column_int(ptr, 1));
        target.setEndOfSanction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
        target.setIsActive(sqlite3_column_int(ptr, 3));
    } else {
        std::cout << sqlite3_errmsg(cx_) << std::endl << "No sanctions with such ID" << std::endl;
    }
}


Book Database::insertOrUpdate(const Book &book){
    Book result;
    //Update only logically editable and non-unique fields
    if (book.getId() != -1) { //TODO: Set all non initialized ID to -1
        try{
            std::ostringstream os;
            os << "UPDATE Books SET ";
            if (book.getTitle() != "")
                os << "title = '" << book.getTitle().c_str() << "',";
            if (book.getAuthor() != "")
                os << "author = '" << book.getAuthor().c_str() << "',";
            if (book.getGenre() != "")
                os << "genre = '" << book.getGenre().c_str() << "',";
            if (book.getYear())
                os << "year = '" << book.getYear() << "' ";
            os << "WHERE id = " << book.getId();
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
    //Insertion
    try{
        std::ostringstream os;
        os << "INSERT INTO Books (title, author, genre, ISBN, year, available) VALUES (" 
        << "'" << book.getTitle().c_str() << "',"
        << "'" << book.getAuthor().c_str() << "',"
        << "'" << book.getGenre().c_str() << "',"
        << "'" << book.getISBN().c_str() << "',"
        << "'" << book.getYear() << "',"
        << "'" << book.getAvailable() << "');";
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
    if (transaction.getId() != -1) { //TODO: Set all non initialized ID to -1
        try{
            std::ostringstream os;
            os << "UPDATE Books SET ";
            if (transaction.getBookId() != -1)
                os << "book_id = '" << transaction.getBookId() << "',";
            if (transaction.getMemberId() != -1)
                os << "member_id = '" << transaction.getMemberId() << "',";
            if (transaction.getTransactionDate() != "")
                os << "transaction_date = '" << transaction.getTransactionDate().c_str() << "',";
            if (transaction.getDueDate() != "")
                os << "due_date = '" << transaction.getDueDate() << "', ";
            if (transaction.getReturningDate() != "")
                os << "returning_date = '" << transaction.getReturningDate() << "' ";
            os << "WHERE id = " << transaction.getId();
            if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
                std::cout << os.str() << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
            } else {
                // result = *new Book(sqlite3_last_insert_rowid(cx_),
                //     transaction.getTitle(),
                //     transaction.getISBN());
            }
        } catch (...) { return result; }
        return result;
    }
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
                sanction.getMemberId(),
                sanction.getEndOfSanction());
        }
    } catch (...) { return result; }
    return result;
}