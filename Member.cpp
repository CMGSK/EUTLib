#include <iostream>
#include <sstream>
#include <string>

#include "Commands.h"
#include "Member.h"


Member::Member(std::string name, std::string address, std::string email, std::string phone, bool isActive, std::string restrictedUntil, int id = 0){
    setId(id);
    name_ = name == "" ? "null" : name;
    address_ = address;
    email_ = email;
    phone_ = phone;
    restrictedUntil_ = restrictedUntil;
    active_ = isActive;
}

Member::Member(bool isUpdate){
    std::string input = "";
    if(isUpdate){
        std::cout << "Insert the ID.\n> ";
        std::getline(std::cin, input);
        setId(input != "" ? stoi(input) : -1);
        input = "";
    }
    std::cout << "Insert a name.\n> ";
    std::getline(std::cin, input);
    name_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert an address for the member. \n> ";
    std::getline(std::cin, input);
    address_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert an email for the member. \n> ";
    std::getline(std::cin, input);
    email_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a phone for the member. \n> ";
    std::getline(std::cin, input);
    phone_ = input != "" ? input : "null";
    input = "";
    std::cout << "Is the memeber active? (Y/N) \n> ";
    std::getline(std::cin, input);
    active_ = (input == "Y" || input == "y") ? true : false;
}

std::string Member::getUpdateQry() const{
    std::ostringstream os;
    os << "UPDATE Members SET ";
    if (name_ != "")
        os << "name = '" << name_ << "'";
    if (address_ != "")
        os << ",address = '" << address_ << "'";
    if (email_ != "")
        os << ",email = '" << email_ << "'";
    if (phone_ != "")
        os << ",phone = '" << phone_ << "'";
    if (restrictedUntil_ != "")
        os << ",restricted_until = '" << restrictedUntil_ << "'";
    os << "active = " << active_ << " ";
    os << "WHERE id = " << getId();
    return os.str();
}

std::string Member::getInsertQry() const {
    std::ostringstream os;
    os << "INSERT INTO Members (name, address, email, phone, active, restricted_until) VALUES (" 
    << "'" << name_ << "',"
    << "'" << address_ << "',"
    << "'" << email_ << "',"
    << "'" << phone_ << "',"
    << "'" << active_ << "',"
    << "'" << restrictedUntil_ << "');";
    return os.str();
}

std::string Member::toString() {
    std::ostringstream os;
    os << "Member id: " << this->getId() << '\n'
    << "--------" << '\n'
    << '\t' << "Name: " << name_ << '\n'
    << '\t' << "Address: " << address_ << '\n'
    << '\t' << "Email: " << email_ << '\n'
    << '\t' << "Phone: " << phone_ << '\n'
    << '\t' << "Current restriction: " << restrictedUntil_ << '\n'
    << '\t' << "Active user: " << active_ << std::endl;
    return os.str();
};