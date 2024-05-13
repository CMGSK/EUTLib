#include <iostream>
#include <string>
#include "LibraryRepository.h"

class Student : public LibraryRepository {
private: 

    int id_;
    std::string name_;
    std::string subject_;
    std::string phone_;
    std::string email_;
    int promotion_;
    int currentYear_;
    float avgGrade_;
    bool active_;

public:

    Student() {};
    Student(bool isUpdate);
    Student(std::string name, std::string subject, std::string phone, std::string email, int promotion, int currentYear, int avgGrade, bool active, int id = -1);

    void setName(std::string s) {name_ = s;};
    void setSubject(std::string s) {subject_ = s;};
    void setPhone(std::string s) {phone_ =s;};
    void setEmail(std::string s) {email_ = s;};
    void setPromotion(int i) {promotion_ = i;};
    void setCurrentYear(int i) {currentYear_ = i;};
    void setAvgGrade(float f) {avgGrade_ = f;};
    void setActive(bool b) {active_ = b;};
    int getId () const {return id_;};
    std::string getName() const {return name_;};
    std::string getAddress() const {return subject_;};
    std::string getEmail() const {return email_;};
    std::string getPhone () const {return phone_;};
    int getPromotion() const {return promotion_;};
    int getCurrentYear() const {return currentYear_;};
    float getAvgGrade() const {return avgGrade_;};
    bool getActive() const {return active_;};

    std::string toString() override;

    std::string getUpdateQry() const;

    std::string getInsertQry() const;


};

