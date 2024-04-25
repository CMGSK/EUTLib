#ifndef SANCTION_H
#define SANCTION_H

#include "LibraryRepository.h"
#include <string>
#include <sstream>

class Sanction : public LibraryRepository {
private: 

    int memberId_;
    std::string endOfSanction_;
    bool isActive_;

public:

    Sanction();
    Sanction(int memberId, std::string endOfSanction):
    LibraryRepository(), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};
    Sanction(int id, int memberId, std::string endOfSanction):
    LibraryRepository(id), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};

    void setMemberId(int n) {memberId_ = n;};
    void setEndOfSanction(std::string s) { endOfSanction_ = s;};
    void setIsActive(bool b) {isActive_ = b;};
    void markAsFinished(){isActive_ = false;};
    int getMemberId() const {return memberId_;};
    std::string getEndOfSanction()  const {return endOfSanction_;};
    bool getIsActive() const {return isActive_;};

    std::string toString() override {
       std::ostringstream os;
       os << "Sanction id: " << this->getId() << '\n'
       << "On member: " << memberId_ << '\n'
       << "----------" << '\n'
       << '\t' << "End of sanction: " << endOfSanction_ << '\n'
       << '\t' << "Is active: " << isActive_ << std::endl;
       return os.str();
    };
};

#endif