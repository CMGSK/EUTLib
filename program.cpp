#include "Database.h"
#include "Commands.h"
#include "LibraryRepository.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

void insertionPath(Database* db, std::string command);
void deletionPath(Database* db, std::string command);
void updatePath(Database* db, std::string command);
void selectionPath(Database* db, std::string command, bool isTotal);
std::string parseCommand(char* &command);

int main() {

    Database* db = new Database();
    
    std::cout << "Insert a command. Type -h for help. \n> ";
    while(true){
        char* command = new char[20]();
        std::cin.getline(command, 20); 
        try{
            std::string parse = parseCommand(command);
            std::string flag = parseCommand(command);
            switch (command_case.at(parse)) { 
                case 0: {
                    db->printHelp();
                    break;
                }
                case 1: {
                    insertionPath(db, flag);
                    break;
                }
                case 2: {
                    deletionPath(db, flag);
                    break;
                }
                case 3: {
                    updatePath(db, flag);
                    break;
                }
                case 4: {
                    selectionPath(db, flag, strcmp(command, "-a") == 0);
                    break;
                }
            }
        } catch (...) {
            std::cout << "Invalid command. If you need help, type -h.";
        }
    }

    return 0;
}

std::string parseCommand(char* &cmd){
    char* iter = cmd;
    while (*iter != '\0' && *iter != ' ')
        iter++;
    *iter++ = 0;
    std::string parse = cmd;
    cmd = iter;
    return parse;
}

void selectionPath(Database* db, std::string command, bool isTotal){
    try{
        switch (flag_case.at(command)) {
            case 1: {
                Book::execSelect(isTotal);
                break;
            }
            case 2: {
                if(isTotal){
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    db->query(r, "members", "", "", false, true);
                    if (r.empty()) std::cout << "No results." << std::endl;
                    for(auto item : r)
                        std::static_pointer_cast<Member>(item)->toString();
                } 
                else {
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    std::string filter;
                    std::string value;
                    char partialMatch;

                    std::cout << "Type the the attribute you will look for:\n>";
                    db->printAttr("members");
                    std::getline(std::cin, filter);
                    std::cout << "Type the the value you want to check\n>";
                    std::getline(std::cin, value);
                    std::cout << "Do you want to include partial matches? (Y/N): ";
                    std::cin >> partialMatch;

                    db->query(r, "members", filter, value, partialMatch == 'Y' || partialMatch == 'y', false);
                    if (r.empty()) std::cout << "No results." << std::endl;

                    for(auto item : r)
                        std::static_pointer_cast<Member>(item)->toString();
                }
            }
            case 3: {
                if(isTotal){
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    db->query(r, "transactions", "", "", false, true);
                    if (r.empty()) std::cout << "No results." << std::endl;
                    for(auto item : r)
                        std::static_pointer_cast<Transaction>(item)->toString();
                } 
                else {
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    std::string filter;
                    std::string value;
                    char partialMatch;

                    std::cout << "Type the the attribute you will look for:\n>";
                    db->printAttr("transactions");
                    std::getline(std::cin, filter);
                    std::cout << "Type the the value you want to check\n>";
                    std::getline(std::cin, value);
                    std::cout << "Do you want to include partial matches? (Y/N): ";
                    std::cin >> partialMatch;

                    db->query(r, "transactions", filter, value, partialMatch == 'Y' || partialMatch == 'y', false);
                    if (r.empty()) std::cout << "No results." << std::endl;

                    for(auto item : r)
                        std::static_pointer_cast<Transaction>(item)->toString();
                }
            }
            case 4: {
                if(isTotal){
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    db->query(r, "sanctions", "", "", false, true);
                    if (r.empty()) std::cout << "No results." << std::endl;
                    for(auto item : r)
                        std::static_pointer_cast<Sanction>(item)->toString();
                } 
                else {
                    std::vector<std::shared_ptr<LibraryRepository>> r;
                    std::string filter;
                    std::string value;
                    char partialMatch;

                    std::cout << "Type the the attribute you will look for:\n>";
                    db->printAttr("sacntions");
                    std::getline(std::cin, filter);
                    std::cout << "Type the the value you want to check\n>";
                    std::getline(std::cin, value);
                    std::cout << "Do you want to include partial matches? (Y/N): ";
                    std::cin >> partialMatch;

                    db->query(r, "sanctions", filter, value, partialMatch == 'Y' || partialMatch == 'y', false);

                    for(auto item : r)
                        std::static_pointer_cast<Sanction>(item)->toString();
                }
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}

void deletionPath(Database* db, std::string command){
    try{
        std::string input;
        switch (flag_case.at(command)) {
            case 0: {
                char bulk;
                std::cout << "Do you need to delete more than one item? (Y/N): ";
                std::cin >> bulk;
                if (bulk == 'Y' || bulk == 'y'){
                    std::cout << "Input all the IDs and type 'done' to submit.\n>";
                    std::vector<int> id;
                    while (input != "done"){
                        try{
                            std::getline(std::cin, input);
                            id.push_back(atoi(input.c_str()));
                        } catch (...) { 
                            std::cout << "Thats not a valid ID nor command.\n>";
                            continue;
                        }
                    }
                    db->deleteBulk("books", id);
                } else {
                    std::cout << "Input the target ID.\n>";
                    try {
                        std::string input;
                        std::getline(std::cin, input);
                        int id = atoi(input.c_str());
                        db->deleteSingle("books", id);
                    } catch (...){
                        std::cout << "Thats not a valid ID nor command.\n>";
                        return;
                    }
                }
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}

void insertionPath(Database* db, std::string command) {
    try{
        switch (flag_case.at(command)) {
            case 1: {
                Book* book = new Book();
                std::string input = "";
                std::cout << "Insert a title for the book.\n> ";
                std::getline(std::cin, input);
                book->setTitle(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an author for the book. (Optional)\n> ";
                std::getline(std::cin, input);
                book->setAuthor(input);
                input = "";
                std::cout << "Insert an ISBN for the book. \n> ";
                std::getline(std::cin, input);
                book->setISBN(input != "" ? input : "null");
                input = "";
                std::cout << "Insert the ammount of books. \n> ";
                std::getline(std::cin, input);
                book->setAvailable(atoi(input.c_str()));

                db->insertOrUpdate(*book);
                std::cout << "-------------\nBook inserted successfully.\n-------------\n\n" << std::endl;
                return;
            }
            case 2: {
                Member* member = new Member();
                std::string input = "";
                std::cout << "Insert a name.\n> ";
                std::getline(std::cin, input);
                member->setName(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an address for the member. \n> ";
                std::getline(std::cin, input);
                member->setAddress(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an email for the member. \n> ";
                std::getline(std::cin, input);
                member->setEmail(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a phone for the member. \n> ";
                std::getline(std::cin, input);
                member->setPhone(input != "" ? input : "null");
                member->setActive(true);

                Member res = db->insertOrUpdate(*member);
                std::cout << "-----> Member inserted\n\n" << res.toString() << std::endl;
            }
            case 3: {
                Transaction* transaction = new Transaction();
                std::string input = "";
                std::cout << "Insert a Book ID.\n> ";
                std::getline(std::cin, input);
                transaction->setBookId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert a Member ID.\n> ";
                std::getline(std::cin, input);
                transaction->setMemberId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert a transaction date.\n> ";
                std::getline(std::cin, input);
                transaction->setTransactionDate(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a due date.\n> ";
                std::getline(std::cin, input);
                transaction->setDueDate(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a returning date.\n> ";
                std::getline(std::cin, input);
                transaction->setReturningDate(input != "" ? input : "null");
                input = "";
                transaction->setIsReturned(false);

                Transaction res = db->insertOrUpdate(*transaction);
                std::cout << "-----> Transaction inserted.\n\n" << res.toString() << std::endl;
            }
            case 4: {
                Sanction* sanction = new Sanction();
                std::string input = "";
                std::cout << "Insert a Member ID.\n> ";
                std::getline(std::cin, input);
                sanction->setMemberId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert an end of sanction date.\n> ";
                std::getline(std::cin, input);
                sanction->setEndOfSanction(input != "" ? input : "null");
                input = "";
                sanction->setIsActive(false);

                Sanction res = db->insertOrUpdate(*sanction);
                std::cout << "-----> Sanction inserted\n\n" << res.toString() << std::endl;
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}

void updatePath(Database* db, std::string command) {
    try{
        switch (flag_case.at(command)) {
            case 1: {
                Book* book = new Book();
                std::string input = "";
                std::cout << "Insert the ID.\n> ";
                std::getline(std::cin, input);
                book->setId(atoi(input.c_str()));
                input = "";
                std::cout << "Insert a title for the book.\n> ";
                std::getline(std::cin, input);
                book->setTitle(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an author for the book. (Optional)\n> ";
                std::getline(std::cin, input);
                book->setAuthor(input);
                input = "";
                std::cout << "Insert an ISBN for the book. \n> ";
                std::getline(std::cin, input);
                book->setISBN(input != "" ? input : "null");
                input = "";
                std::cout << "Insert the ammount of books. \n> ";
                std::getline(std::cin, input);
                book->setAvailable(atoi(input.c_str()));

                Book res = db->insertOrUpdate(*book);
                std::cout << "-----> Book inserted\n" << res.toString() << std::endl;
                return;
            }
            case 2: {
                Member* member = new Member();
                std::string input = "";
                std::cout << "Insert the ID.\n> ";
                std::getline(std::cin, input);
                member->setId(atoi(input.c_str()));
                input = "";
                std::cout << "Insert a name.\n> ";
                std::getline(std::cin, input);
                member->setName(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an address for the member. \n> ";
                std::getline(std::cin, input);
                member->setAddress(input != "" ? input : "null");
                input = "";
                std::cout << "Insert an email for the member. \n> ";
                std::getline(std::cin, input);
                member->setEmail(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a phone for the member. \n> ";
                std::getline(std::cin, input);
                member->setPhone(input != "" ? input : "null");
                member->setActive(true);

                Member res = db->insertOrUpdate(*member);
                std::cout << "-----> Member inserted\n" << res.toString() << std::endl;
            }
            case 3: {
                Transaction* transaction = new Transaction();
                std::string input = "";
                std::cout << "Insert the ID.\n> ";
                std::getline(std::cin, input);
                transaction->setId(atoi(input.c_str()));
                input = "";
                std::cout << "Insert a Book ID.\n> ";
                std::getline(std::cin, input);
                transaction->setBookId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert a Member ID.\n> ";
                std::getline(std::cin, input);
                transaction->setMemberId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert a transaction date.\n> ";
                std::getline(std::cin, input);
                transaction->setTransactionDate(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a due date.\n> ";
                std::getline(std::cin, input);
                transaction->setDueDate(input != "" ? input : "null");
                input = "";
                std::cout << "Insert a returning date.\n> ";
                std::getline(std::cin, input);
                transaction->setReturningDate(input != "" ? input : "null");
                input = "";
                transaction->setIsReturned(false);

                Transaction res = db->insertOrUpdate(*transaction);
                std::cout << "-----> Member inserted\n" << res.toString() << std::endl;
            }
            case 4: {
                Sanction* sanction = new Sanction();
                std::string input = "";
                std::cout << "Insert the ID.\n> ";
                std::getline(std::cin, input);
                sanction->setId(atoi(input.c_str()));
                input = "";
                std::cout << "Insert a Member ID.\n> ";
                std::getline(std::cin, input);
                sanction->setMemberId(atoi(input.c_str()) != -1 ? atoi(input.c_str()) : -1);
                input = "";
                std::cout << "Insert an end of sanction date.\n> ";
                std::getline(std::cin, input);
                sanction->setEndOfSanction(input != "" ? input : "null");
                input = "";
                sanction->setIsActive(false);

                Sanction res = db->insertOrUpdate(*sanction);
                std::cout << "-----> Member inserted\n" << res.toString() << std::endl;
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}