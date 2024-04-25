#include <iostream>
#include <iterator>
#include <vector>
#include <memory>
#include <sqlite3.h>
#include <sstream>
#include <string>

#include "Database.h"
#include "Commands.h"
#include "Book.h"
#include "LibraryRepository.h"
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

// Initialization
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

// Help
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

bool Database::deleteBulk(std::string table, std::vector<int> id){
    std::ostringstream os;
    os << "DELETE FROM " << table << " WHERE id IN ( ";
    for (int i=0; i<id.size(); i++){
        os << id[i] << (i == id.size()-1 ? ")" : ",");
    }

    if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
        std::cout << os.str() << std::endl;
        std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        return false;
    } else return true;
}

//Selects
void Database::query(std::vector<std::shared_ptr<LibraryRepository>> &target, std::string from, std::string filter, std::string value, bool partialMatch, bool isTotal){

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
                    std::shared_ptr<Book> book = std::make_shared<Book>();
                    book->setId(sqlite3_column_int(ptr, 0));
                    book->setTitle(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
                    book->setAuthor(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    book->setGenre(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    book->setYear(sqlite3_column_int(ptr, 4));
                    book->setAvailable(sqlite3_column_int(ptr, 5));
                    target.push_back(book);
                }
                return;
            }
            case 1: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    std::shared_ptr<Member> member = std::make_shared<Member>();
                    member->setId(sqlite3_column_int(ptr, 0));
                    member->setName(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 1)));
                    member->setAddress(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    member->setEmail(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    member->setPhone(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
                    member->setRestriction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
                    member->setActive(sqlite3_column_int(ptr, 6));
                    target.push_back(member);
                }
                return;
            }
            case 2: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    std::shared_ptr<Transaction> transaction = std::make_shared<Transaction>();
                    transaction->setId(sqlite3_column_int(ptr, 0));
                    transaction->setBookId(sqlite3_column_int(ptr, 1));
                    transaction->setMemberId(sqlite3_column_int(ptr, 2));
                    transaction->setTransactionDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 3)));
                    transaction->setDueDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 4)));
                    transaction->setReturningDate(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 5)));
                    transaction->setIsReturned(sqlite3_column_int(ptr, 6));
                    target.push_back(transaction);
                }
                return;
            }
            case 3: {
                while (sqlite3_step(ptr) == SQLITE_ROW){
                    std::shared_ptr<Sanction> sanction = std::make_shared<Sanction>();
                    sanction->setId(sqlite3_column_int(ptr, 0));
                    sanction->setMemberId(sqlite3_column_int(ptr, 1));
                    sanction->setEndOfSanction(reinterpret_cast<const char*>(sqlite3_column_text(ptr, 2)));
                    sanction->setIsActive(sqlite3_column_int(ptr, 3));
                    target.push_back(sanction);
                }
                return;
            }
        }
    } catch (...) { 
        std::cout << "table does not exist" << std::endl;     
        return; 
    }
}

// Inserts and Updates
Book Database::insertOrUpdate(const Book &book){
    Book result;
    // Update
    if (book.getId() != -1) { 
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
                int id = book.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "books", "id", std::to_string(id), false, false);
                return *static_cast<Book*>(r[0].get());
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
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "books", "id", std::to_string(id), false, false);
            return *static_cast<Book*>(r[0].get());
        }
    } catch (...) { return result; }
    return result;
}

Transaction Database::insertOrUpdate(const Transaction &transaction){
    Transaction result;
    // Update
    if (transaction.getId() != -1) {
        try{
            std::ostringstream os;
            os << "UPDATE Transactions SET ";
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
                int id = transaction.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "transactions", "id", std::to_string(id), false, false);
                return *static_cast<Transaction*>(r[0].get());
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
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "transactions", "id", std::to_string(id), false, false);
            return *static_cast<Transaction*>(r[0].get());
        }
    } catch (...) { return result; }
    return result;
}

Member Database::insertOrUpdate(const Member &member){
    Member result;
    // Update
    if (member.getId() != -1) { //TODO: Set all non initialized ID to -1
        try{
            std::ostringstream os;
            os << "UPDATE Members SET ";
            if (member.getName() != "")
                os << "name = '" << member.getName() << "',";
            if (member.getAddress() != "")
                os << "address = '" << member.getAddress() << "',";
            if (member.getEmail() != "")
                os << "email = '" << member.getEmail().c_str() << "',";
            if (member.getPhone() != "")
                os << "phone = '" << member.getPhone() << "', ";
            if (member.getRestrictedUntil() != "")
                os << "restricted_until = '" << member.getPhone() << "', ";
            if (member.getActive())
                os << "active = " << member.getActive() << ")";
            os << "WHERE id = " << member.getId();
            if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
                std::cout << os.str() << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
            } else {
                int id = member.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "transactions", "id", std::to_string(id), false, false);
                return *static_cast<Member*>(r[0].get());
            }
        } catch (...) { return result; }
        return result;
    }
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
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "sanctions", "id", std::to_string(id), false, false);
            return *static_cast<Member*>(r[0].get());
        }
    } catch (...) { return result; }
    return result;
}

Sanction Database::insertOrUpdate(const Sanction &sanction){
    Sanction result;
    // Update
    if (sanction.getId() != -1) { //TODO: Set all non initialized ID to -1
        try{
            std::ostringstream os;
            os << "UPDATE Sanctions SET ";
            if (sanction.getIsActive())
                os << "is_active = " << sanction.getIsActive() << ")";
            os << "WHERE id = " << sanction.getId();
            if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
                std::cout << os.str() << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
            } else {
                int id = sanction.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "transactions", "id", std::to_string(id), false, false);
                return *static_cast<Sanction*>(r[0].get());
            }
        } catch (...) { return result; }
        return result;
    }
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
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "transactions", "id", std::to_string(id), false, false);
            return *static_cast<Sanction*>(r[0].get());
        }
    } catch (...) { return result; }
    return result;
}