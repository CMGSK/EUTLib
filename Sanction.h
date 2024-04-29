#include <iostream>
#include "LibraryRepository.h"

class Sanction : public LibraryRepository {
private: 

    int memberId_;
    std::string endOfSanction_;
    bool isActive_;

public:

    Sanction() {};
    Sanction(bool isUpdate);
    Sanction(int memberId, std::string endOfSanction, bool isActive, int id = 0);
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

    std::string toString() override;

    std::string getUpdateQry() const;

    std::string getInsertQry() const;
};

