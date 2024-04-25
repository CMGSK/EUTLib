#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <sstream>
#include "LibraryRepository.h"

class Member : public LibraryRepository {
private: 

    std::string name_;
    std::string address_;
    std::string email_;
    std::string phone_;
    std::string restrictedUntil_;
    bool active_;

public:

    Member();
    Member(std::string name, std::string address, std::string email, std::string phone):
    LibraryRepository(), name_(name), address_(address), email_(email), phone_(phone)
    {};
    Member(int id, std::string name):
    LibraryRepository(id), name_(name) 
    {};

    void setName(std::string s) {name_ = s;};
    void setAddress(std::string s) {address_ = s;};
    void setEmail(std::string s) {email_ = s;};
    void setPhone(std::string s) {phone_ =s;};
    void setRestriction(std::string s) {restrictedUntil_ = s;};
    void setActive(bool b) {active_ = b;};
    std::string getName() const {return name_;};
    std::string getAddress() const {return address_;};
    std::string getEmail() const {return email_;};
    std::string getPhone () const {return phone_;};
    std::string getRestrictedUntil() const {return restrictedUntil_;};
    bool getActive() const {return active_;};

    std::string toString() override {
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

};

#endif