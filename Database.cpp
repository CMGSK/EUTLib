#include <iostream>
#include <iterator>
#include <list>
#include <sqlite3.h>
#include <sstream>
#include <string>

#include "Database.h"
#include "Commands.h"
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

void Database::printHelp(){
    std::cout << "These are the available commands:" << std::endl;
    std::cout << "(Type 'quit' to exit from the program anytime)" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Command to insert:" << std::endl;
    std::cout << "\t ins [-flag]" << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "\t [-b] for books" << std::endl;
    std::cout << "\t [-m] for members" << std::endl;
    std::cout << "\t [-t] for transactions" << std::endl;
    std::cout << "\t [-s] for sanctions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Command to delete:" << std::endl;
    std::cout << "\t del [-flag]" << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "\t [-b] for books" << std::endl;
    std::cout << "\t [-m] for members" << std::endl;
    std::cout << "\t [-t] for transactions" << std::endl;
    std::cout << "\t [-s] for sanctions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Command to update:" << std::endl;
    std::cout << "\t mod [-flag]" << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "\t [-b] for books" << std::endl;
    std::cout << "\t [-m] for members" << std::endl;
    std::cout << "\t [-t] for transactions" << std::endl;
    std::cout << "\t [-s] for sanctions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Command to select:" << std::endl;
    std::cout << "\t sel [-flag]" << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "\t [-a] use it to avoid filter input (select all)" << std::endl;
    std::cout << "\t   ---------------------   " << std::endl;
    std::cout << "\t [-b] for books" << std::endl;
    std::cout << "\t [-m] for members" << std::endl;
    std::cout << "\t [-t] for transactions" << std::endl;
    std::cout << "\t [-s] for sanctions" << std::endl;
    std::cout << "\t [-s] for sanctions" << std::endl;
}

//Deletes
bool Database::deleteSingle(std::string table, int id){
    std::ostringstream os;
    os << "DELETE FROM " << table << " WHERE id = " << id;

    if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
        std::cout << os.str() << std::endl;
        std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        return false;
    } else return true;
}

bool Database::deleteBulk(std::string table, int id[], int arrSize){
    std::ostringstream os;
    os << "DELETE FROM " << table << " WHERE id IN ( ";
    for (int i=0; i<arrSize; i++){
        os << id[i] << (i == arrSize-1 ? ")" : ",");
    }

    if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
        std::cout << os.str() << std::endl;
        std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        return false;
    } else return true;
}

//Selects
void Database::query(std::list<void*> &target, std::string from, std::string filter, std::string value, bool partialMatch, bool isTotal){

    std::ostringstream os;
    if (!isTotal) {
        os << "SELECT * from " << from << " WHERE " << filter << (partialMatch ? " LIKE '%" : "='") << value << (partialMatch ? "%'" : "'");
    } else {
        os << "SELECT * FROM " << from;
    }

    if(sqlite3_prepare_v2(cx_, sql.c_str(), -1, &ptr, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    try{
        switch (from_case.at(from)) {
            case 0: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    void* temp;
                    Book* book = new Book();
                    book->setId(sqlite3_column_int(ptr, 0));
                    book->setTitle(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
                    book->setAuthor(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    book->setGenre(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    book->setYear(sqlite3_column_int(ptr, 4));
                    book->setAvailable(sqlite3_column_int(ptr, 5));
                    temp = book;
                    target.push_back(temp);
                }
                return;
            }
            case 1: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    void* temp;
                    Member* member = new Member();
                    member->setId(sqlite3_column_int(ptr, 0));
                    member->setName(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
                    member->setAddress(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    member->setEmail(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    member->setPhone(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
                    member->setRestriction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
                    member->setActive(sqlite3_column_int(ptr, 6));
                    temp = member;
                    target.push_back(temp);
                }
                return;
            }
            case 2: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    void* temp;
                    Transaction* transaction = new Transaction();
                    transaction->setId(sqlite3_column_int(ptr, 0));
                    transaction->setBookId(sqlite3_column_int(ptr, 1));
                    transaction->setMemberId(sqlite3_column_int(ptr, 2));
                    transaction->setTransactionDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    transaction->setDueDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
                    transaction->setReturningDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
                    transaction->setIsReturned(sqlite3_column_int(ptr, 6));
                    temp = transaction;
                    target.push_back(temp);
                }
                return;
            }
            case 3: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    void* temp;
                    Sanction* sanction = new Sanction();
                    sanction->setId(sqlite3_column_int(ptr, 0));
                    sanction->setMemberId(sqlite3_column_int(ptr, 1));
                    sanction->setEndOfSanction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    sanction->setIsActive(sqlite3_column_int(ptr, 3));
                    temp = sanction;
                    target.push_back(temp);
                }
                return;
            }
        }
    } catch (...) { 
        std::cout << "table does not exist" << std::endl;     
        return; 
    }
}

// Inserts or Updates
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
                //TODO: rewrite to use vectors then ask the fucking baldman how to cast void* into Obj
                int id = sqlite3_last_insert_rowid(cx_);
                std::list<void*> r;
                query(r, "books", "id", std::to_string(id), false, false);
                void* temp = r.begin();
                result = reinterpret_cast<Book*>(temp)
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