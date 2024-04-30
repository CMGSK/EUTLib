#include <iostream>
#include <sstream>
#include <string>

#include "Commands.h"
#include "Transaction.h"


Transaction::Transaction(int bookId, int memberId, std::string transactionDate, std::string dueDate, std::string returningDate, bool isReturned, int id){
    setId(id);
    bookId_ = bookId;
    memberId_ = memberId;
    transactionDate_ = transactionDate;
    dueDate_ = dueDate;
    returningDate_ = returningDate;
    isReturned_ = isReturned;
}

Transaction::Transaction(bool isUpdate){
    std::string input = "";
    if (isUpdate){
        std::cout << "Insert the ID.\n> ";
        std::getline(std::cin, input);
        setId(input != "" ? stoi(input) : -1);
        input = "";
    }
    std::cout << "Insert a Book ID.\n> ";
    std::getline(std::cin, input);
    bookId_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert a Member ID.\n> ";
    std::getline(std::cin, input);
    memberId_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert a transaction date.\n> ";
    std::getline(std::cin, input);
    transactionDate_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a due date.\n> ";
    std::getline(std::cin, input);
    dueDate_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a returning date.\n> ";
    std::getline(std::cin, input);
    returningDate_ = input != "" ? input : "null";
    input = "";
    std::cout << "Is returned? (Y/N)";
    std::getline(std::cin, input);
    isReturned_ = (input == "Y" || input == "y") ? true : false;

}

std::string Transaction::toString() {
    std::ostringstream os;
    os << "Member id: " << this->getId() << '\n'
    << " Relation Member-Book -> " << bookId_ << " - " << memberId_ << '\n'
    << " --------------------" << '\n'
    << '\t' << "Transaction date: " << transactionDate_ << '\n'
    << '\t' << "Due date: " << dueDate_ << '\n'
    << '\t' << "Returning date: " << returningDate_ << '\n'
    << '\t' << "Has been returned: " << isReturned_ << std::endl;
    return os.str();
};

std::string Transaction::getUpdateQry() const{
    std::ostringstream os;
    os << "UPDATE Transactions SET ";
    if (bookId_ != -1)
        os << "book_id = '" << bookId_ << "'";
    if (memberId_ != -1)
        os << ",member_id = '" << memberId_ << "'";
    if (transactionDate_ != "")
        os << ",transaction_date = '" << transactionDate_ << "'";
    if (dueDate_ != "")
        os << ",due_date = '" << dueDate_ << "'";
    if (returningDate_ != "")
        os << ",returning_date = '" << returningDate_ << "' ";
    os << "WHERE id = " << getId();
    return os.str();
}

std::string Transaction::getInsertQry() const{
    std::ostringstream os;
    os << "INSERT INTO Transactions (book_id, member_id, transaction_date, due_date, returning_date, is_returned) VALUES (" 
    << "'" << bookId_ << "',"
    << "'" << memberId_ << "',"
    << "'" << transactionDate_ << "',"
    << "'" << dueDate_ << "',"
    << "'" << returningDate_ << "',"
    << "false);";
    return os.str();
}
