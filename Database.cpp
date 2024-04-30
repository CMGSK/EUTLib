#include <iostream>
#include <ostream>
#include <vector>
#include <memory>
#include <sstream>
#include <string>

#include "Database.h"
#include "Commands.h"
#include "LibraryRepository.h"

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
    std::cout << ">";
}

void Database::printAttr(std::string item){
    try{
        switch (from_case.at(item)) {
            case 0: 
                std::cout << "Book attributes:" << std::endl;
                std::cout << "Title, author, genre, ISBN, year, (number of)available." << std::endl;
                break;
            case 1: 
                std::cout << "Member attributes:" << std::endl;
                std::cout << "Name, address, email, phone, active, restricted_until." << std::endl;
                break;
            case 2: 
                std::cout << "Transaction attributes:" << std::endl;
                std::cout << "Book_id, Member_id, transaction_date, due_date, returning_date, is_returned." << std::endl;
                break;
            case 3: 
                std::cout << "Sanction attributes:" << std::endl;
                std::cout << "Member_id, end_of_sanction, is_active." << std::endl;
                break;
        }
    } catch (...) { std::cout << "That kind of item doesn't exist" << std::endl;}
}

//Deletes
bool Database::deleteSingle(std::string table, int id){
    std::ostringstream os;
    os << "DELETE FROM " << table << " WHERE id = " << id;

    if (sqlite3_exec(cx_, os.str().c_str(), NULL, 0, &err_)){
        std::cout << os.str() << std::endl;
        std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << os.str();
        return false;
    }
    return true;
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
    }
    return true;
}

//Selects
void Database::query(std::vector<std::shared_ptr<LibraryRepository>> &target, std::string from, std::string filter, std::string value, bool partialMatch, bool isTotal){
    std::ostringstream os;
    os << "SELECT * FROM " << from;
    if (!isTotal) os << " WHERE " << filter << (partialMatch ? " LIKE '%" : "='") << value << (partialMatch ? "%'" : "'");

    if(sqlite3_prepare_v2(cx_, os.str().c_str(), -1, &ptr_, NULL)){
        std::cout << sqlite3_errmsg(cx_) << std::endl;
        return;
    }

    try{
        switch (from_case.at(from)) {
            case 0: {
                while (sqlite3_step(ptr_) == SQLITE_ROW){
                    std::shared_ptr<Book> book = std::make_shared<Book>(
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 1)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 2)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 3)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 4)),
                        sqlite3_column_int(ptr_, 5),
                        sqlite3_column_int(ptr_, 6),
                        sqlite3_column_int(ptr_, 0)
                    );
                    target.push_back(book);
                }
                return;
            }
            case 1: {
                while (sqlite3_step(ptr_) == SQLITE_ROW){
                    std::shared_ptr<Member> member = std::make_shared<Member>(
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 1)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 2)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 3)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 4)),
                        sqlite3_column_int(ptr_, 5),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 6)),
                        sqlite3_column_int(ptr_, 0)
                    );
                    target.push_back(member);
                }
                return;
            }
            case 2: {
                while (sqlite3_step(ptr_) == SQLITE_ROW){
                    std::shared_ptr<Transaction> transaction = std::make_shared<Transaction>(
                        sqlite3_column_int(ptr_, 1),
                        sqlite3_column_int(ptr_, 2),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 3)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 4)),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 5)),
                        sqlite3_column_int(ptr_, 6),
                        sqlite3_column_int(ptr_, 0)
                    );
                    target.push_back(transaction);
                }
                return;
            }
            case 3: {
                while (sqlite3_step(ptr_) == SQLITE_ROW){
                    std::shared_ptr<Sanction> sanction = std::make_shared<Sanction>(
                        sqlite3_column_int(ptr_, 1),
                        reinterpret_cast<const char*>(sqlite3_column_text(ptr_, 2)),
                        sqlite3_column_int(ptr_, 3),
                        sqlite3_column_int(ptr_, 0)
                    );
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
            std::string qry = book.getUpdateQry();
            if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
                std::cout << qry << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
            } else {
                int id = book.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "books", "id", std::to_string(id), false, false);
                std::cout << "\n-------------\nBook updated successfully.\n-------------\n\n" << std::endl;
                Book result = *std::static_pointer_cast<Book>(r[0]);
                std::cout << result.toString() << std::endl;
                return result;
            }
        } catch (...) { return result; }
        return result;
    }
    //Insertion
    try{
        std::string qry = book.getInsertQry();
        if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
            std::cout << qry << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
        } else {
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "books", "id", std::to_string(id), false, false);
            Book result = *std::static_pointer_cast<Book>(r[0]);
            std::cout << result.toString() << std::endl;
            return result;
        }
    } catch (...) { return result; }
    return result;
}

Transaction Database::insertOrUpdate(const Transaction &transaction){
    Transaction result;
    // Update
    if (transaction.getId() != -1) {
        try{
            std::string qry = transaction.getUpdateQry();
            if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
                std::cout << qry << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
            } else {
                int id = transaction.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "transactions", "id", std::to_string(id), false, false);
                std::cout << "\n-------------\nTransaction updated successfully.\n-------------\n\n" << std::endl;
                Transaction result = *std::static_pointer_cast<Transaction>(r[0]);
                std::cout << result.toString() << std::endl;
                return result;
            }
        } catch (...) { return result; }
        return result;
    }
    //Insertion
    try{
        std::string qry = transaction.getInsertQry();
        if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
            std::cout << qry << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
        } else {
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "transactions", "id", std::to_string(id), false, false);
            Transaction result = *std::static_pointer_cast<Transaction>(r[0]);
            std::cout << result.toString() << std::endl;
            return result;
        }
    } catch (...) { return result; }
    return result;
}

Member Database::insertOrUpdate(const Member &member){
    Member result;
    // Update
    if (member.getId() != -1) { 
        try{
            std::string qry = member.getUpdateQry();
            if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
                std::cout << qry << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
            } else {
                int id = member.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "members", "id", std::to_string(id), false, false);
                std::cout << "\n-------------\nMember updated successfully.\n-------------\n\n" << std::endl;
                Member result = *std::static_pointer_cast<Member>(r[0]);
                std::cout << result.toString() << std::endl;
                return result;
            }
        } catch (...) { return result; }
        return result;
    }
    //Insertion
    try{
        std::string qry = member.getInsertQry();
        if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
            std::cout << qry << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
        } else {
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "members", "id", std::to_string(id), false, false);
            Member result = *std::static_pointer_cast<Member>(r[0]);
            std::cout << result.toString() << std::endl;
            return result;
        }
    } catch (...) { return result; }
    return result;
}

Sanction Database::insertOrUpdate(const Sanction &sanction){
    Sanction result;
    // Update
    if (sanction.getId() != -1) { 
        try{
            std::string qry = sanction.getUpdateQry();
            if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
                std::cout << qry << std::endl;
                std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
            } else {
                int id = sanction.getId();
                std::vector<std::shared_ptr<LibraryRepository>> r;
                query(r, "sanctions", "id", std::to_string(id), false, false);
                std::cout << "\n-------------\nSanction updated successfully.\n-------------\n\n" << std::endl;
                Sanction result = *std::static_pointer_cast<Sanction>(r[0]);
                std::cout << result.toString() << std::endl;
                return result;
            }
        } catch (...) { return result; }
        return result;
    }
    //Insertion
    try{
        std::string qry = sanction.getInsertQry();
        if (sqlite3_exec(cx_, qry.c_str(), NULL, 0, &err_)){
            std::cout << qry << std::endl;
            std::cout << "Error:" << sqlite3_errmsg(cx_) << std::endl << "Query: " << qry;
        } else {
            int id = sqlite3_last_insert_rowid(cx_);
            std::vector<std::shared_ptr<LibraryRepository>> r;
            query(r, "sanctions", "id", std::to_string(id), false, false);
            Sanction result = *std::static_pointer_cast<Sanction>(r[0]);
            std::cout << result.toString() << std::endl;
            return result;
        }
    } catch (...) { return result; }
    return result;
}