#include "Database.h"
#include "Commands.h"
#include "Member.h"
#include "Transaction.h"
#include <cstdlib>
#include <iostream>

void insertionPath(Database* db, std::string command);

int main() {

    Database* db = new Database();
    
//TODOs: Parse commands. Implement to_string(). Implement getAttributes().
// check if we can avoid double update/delete function with a void*

    //Term program

    std::cout << "Insert a command. Type -h for help. \n> ";
    while(true){
        char* command;
        std::cin >> command;
        try{
            switch (command_case.at(command)) { //TODO Slice at spacebar
                case 0:
                    db->printHelp();
                    break;
                case 1:


            
            }

        } catch (...) {
            std::cout << "Invalid command. If you need help, type -h."
        }



         

    }
    return 0;
}

void insertionPath(Database* db, std::string command) {
    try{
        switch (flag_case.at(command)) {
            case 0: {
                Book* book = new Book();
                std::string input = "";
                std::cout << "Insert a title for the book.\n> ";
                std::cin >> input;
                book->setTitle(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert an author for the book. (Optional)\n> ";
                std::cin >> input;
                book->setAuthor(input);
                input = "";
                std::cout << "Insert an ISBN for the book. \n> ";
                std::cin >> input;
                book->setISBN(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert the ammount of books. \n> ";
                std::cin >> input;
                int n = atoi(input.c_str());
                book->setAvailable(n);

                Book res = db->insertOrUpdate(*book);
                std::cout << "-----> Book inserted\n" << res.to_string() << std::endl;
                return;
            }
            case 1: {
                Member* member = new Member();
                std::string input = "";
                std::cout << "Insert a name.\n> ";
                std::cin >> input;
                member->setName(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert an address for the member. \n> ";
                std::cin >> input;
                member->setAddress(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert an email for the member. \n> ";
                std::cin >> input;
                member->setEmail(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert a phone for the member. \n> ";
                std::cin >> input;
                member->setPhone(input != "" ? input : NULL);
                member->setActive(true);

                Member res = db->insertOrUpdate(*member);
                std::cout << "-----> Member inserted\n" << res.to_string() << std::endl;
            }
            case 2: {
                Transaction* transaction = new Transaction();
                std::string input = "";
                std::cout << "Insert a name.\n> ";
                std::cin >> input;
                member->setName(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert an address for the member. \n> ";
                std::cin >> input;
                member->setAddress(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert an email for the member. \n> ";
                std::cin >> input;
                member->setEmail(input != "" ? input : NULL);
                input = "";
                std::cout << "Insert a phone for the member. \n> ";
                std::cin >> input;
                member->setPhone(input != "" ? input : NULL);
                member->setActive(true);

                Member res = db->insertOrUpdate(*member);
                std::cout << "-----> Member inserted\n" << res.to_string() << std::endl;
            }
        }
    } catch (...) {
        std::cout << "Flag is not valid. If you need help, type -h.";
    }

}
