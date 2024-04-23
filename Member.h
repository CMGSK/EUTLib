#ifndef MEMBER_H
#define MEMBER_H

#include <string>

class Member {
private: 

    int id_;
    std::string name_;
    std::string address_;
    std::string email_;
    std::string phone_;
    std::string restrictedUntil_;
    bool active_;

public:

    Member();
    Member(std::string name, std::string address, std::string email, std::string phone):
    id_(-1), name_(name), address_(address), email_(email), phone_(phone)
    {};
    Member(int id, std::string name):
    id_(id), name_(name) 
    {};

    void setId(int n);
    void setName(std::string s);
    void setAddress(std::string s);
    void setEmail(std::string s);
    void setPhone(std::string s);
    void setRestriction(std::string s);
    void setActive(bool b) {active_ = b;};
    int getId() const {return id_;};
    std::string getName() const {return name_;};
    std::string getAddress() const {return address_;};
    std::string getEmail() const {return email_;};
    std::string getPhone () const {return phone_;};
    std::string getRestrictedUntil() const {return restrictedUntil_;};
    bool getActive() const {return active_;};

};

#endif