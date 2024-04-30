#include <iostream>
#include "Sanction.h"

Sanction::Sanction(int memberId, std::string endOfSanction, bool isActive, int id){
    setId(id);
    endOfSanction_ = endOfSanction == "" ? "null" : endOfSanction;
    memberId_ = memberId;
    isActive_ = isActive;
}

Sanction::Sanction(bool isUpdate){
    std::string input = "";
    if(isUpdate){
        std::cout << "Insert the ID.\n> ";
        std::getline(std::cin, input);
        setId(input != "" ? stoi(input) : -1);
        input = "";
    }
    std::cout << "Insert a Member ID.\n> ";
    std::getline(std::cin, input);
    memberId_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert an end of sanction date.\n> ";
    std::getline(std::cin, input);
    endOfSanction_ = input != "" ? input : "null";
    input = "";
    std::cout << "Is the sanction active? (Y/N)";
    std::getline(std::cin, input);
    isActive_ = input == "y" || input == "Y" ? true : false;
}

std::string Sanction::toString() {
    std::ostringstream os;
    os << "Sanction id: " << this->getId() << '\n'
    << "On member: " << memberId_ << '\n'
    << "----------" << '\n'
    << '\t' << "End of sanction: " << endOfSanction_ << '\n'
    << '\t' << "Is active: " << isActive_ << std::endl;
    return os.str();
    };

std::string Sanction::getInsertQry() const{
    std::ostringstream os;
    os << "INSERT INTO Sanctions (member_id, end_of_sanction, is_active) VALUES (" 
    << "'" << memberId_ << "',"
    << "'" << endOfSanction_ << "',"
    << "'" << isActive_ << "');";
    return os.str();
}

std::string Sanction::getUpdateQry() const{
    std::ostringstream os;
    os << "UPDATE Sanctions SET ";
    os << "is_active = " << isActive_ << " ";
    os << "WHERE id = " << getId();
    return os.str();
}

