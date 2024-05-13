#include <iostream>
#include <sstream>
#include <string>

#include "Student.h"
#include "Transaction.h"


Student::Student(std::string name, std::string subject, std::string phone, std::string email, int promotion, int currentYear, int avgGrade, bool active, int id){
    setId(id);
    name_ = name;
    subject_ = subject;
    phone_ = phone;
    email_ = email;
    promotion_ = promotion;
    currentYear_ = currentYear;
    avgGrade_ = avgGrade;
    active_ = active;
}

Student::Student(bool isUpdate){
    std::string input = "";
    if (isUpdate){
        std::cout << "Insert the ID.\n> ";
        std::getline(std::cin, input);
        setId(input != "" ? stoi(input) : -1);
        input = "";
    }
    std::cout << "Insert a Student name.\n> ";
    std::getline(std::cin, input);
    name_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a Student subject.\n> ";
    std::getline(std::cin, input);
    subject_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a Student phone.\n> ";
    std::getline(std::cin, input);
    phone_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a Student email.\n> ";
    std::getline(std::cin, input);
    email_ = input != "" ? input : "null";
    input = "";
    std::cout << "Insert a Student year of promotion.\n> ";
    std::getline(std::cin, input);
    phone_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert a Student current year.\n> ";
    std::getline(std::cin, input);
    phone_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Insert a Student average grade.\n> ";
    std::getline(std::cin, input);
    char* init = input.data();
    char* end = input.data() + input.length();
    phone_ = input != "" ? std::strtof(init, &end) : -1;
    input = "";
    std::cout << "Insert a Student current year.\n> ";
    phone_ = input != "" ? stoi(input) : -1;
    input = "";
    std::cout << "Is returned? (Y/N)";
    std::getline(std::cin, input);
    active_ = (input == "Y" || input == "y") ? true : false;
}

std::string Student::toString() {
    std::ostringstream os;
    os << "Student id: " << this->getId() << '\n'
    << " --------------------" << '\n'
    << '\t' << "Name: " << name_ << '\n'
    << '\t' << "Subject: " << subject_ << '\n'
    << '\t' << "Phone: " << phone_ << '\n'
    << '\t' << "Email: " << email_ << '\n'
    << '\t' << "Promotion of: " << promotion_ << '\n'
    << '\t' << "Current year: " << currentYear_ << '\n'
    << '\t' << "Average grade: " << avgGrade_ << '\n'
    << '\t' << "Is active: " << active_ << std::endl;
    return os.str();
};

std::string Student::getUpdateQry() const{
    std::ostringstream os;
    os << "UPDATE Transactions SET ";
    if (name_ != "")
        os << "name = '" << name_ << "'";
    if (subject_ != "")
        os << ",subject = '" << subject_ << "'";
    if (phone_ != "")
        os << ",phone = '" << phone_ << "'";
    if (email_ != "")
        os << ",email = '" << email_ << "'";
    if (promotion_ != -1)
        os << ",promotion = '" << promotion_ << "'";
    if (currentYear_ != -1)
        os << ",current_year = '" << currentYear_ << "'";
    if (avgGrade_ != -1)
        os << ",avg_grade = '" << avgGrade_ << "'";
    os << ",active = " << active_ << " ";
    os << "WHERE id = " << getId();
    return os.str();
}

std::string Student::getInsertQry() const{
    std::ostringstream os;
    os << "INSERT INTO Students (name, subject, phone, email, proomtion, current_year, avg_grade, active) VALUES (" 
    << "'" << name_ << "',"
    << "'" << subject_ << "',"
    << "'" << phone_ << "',"
    << "'" << email_ << "',"
    << "'" << promotion_ << "',"
    << "'" << currentYear_ << "',"
    << "'" << avgGrade_ << "',"
    << active_ << ");";
    return os.str();
}

