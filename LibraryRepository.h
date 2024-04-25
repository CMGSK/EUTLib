#ifndef LIBRARYREPOSITORY_H
#define LIBRARYREPOSITORY_H

#include <memory>
class LibraryRepository {

private:
    int id_;

public:

    LibraryRepository(): id_(-1){};
    LibraryRepository(int id): id_(id){};

    int getId() const {return id_;};
    void setId(int id){id_ = id;};

    virtual std::string toString();

};


#endif