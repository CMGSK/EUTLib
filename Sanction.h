#ifndef SANCTION_H
#define SANCTION_H

#include <string>

class Sanction {
private: 

    int id_;
    int memberId_;
    std::string endOfSanction_;
    bool isActive_;

public:

    Sanction();
    Sanction(int memberId, std::string endOfSanction):
    id_(-1), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};
    Sanction(int id, int memberId, std::string endOfSanction):
    id_(id), memberId_(memberId), endOfSanction_(endOfSanction), isActive_(true)
    {};

    void setId(int n);
    void setMemberId(int n);
    void setEndOfSanction(std::string s);
    void setIsActive(bool b);
    void markAsFinished(){isActive_ = false;};
    int getId() const {return id_;};
    int getMemberId() const {return memberId_;};
    std::string getEndOfSanction()  const {return endOfSanction_;};
    bool getIsActive() const {return isActive_;};

};

#endif