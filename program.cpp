#include "Database.h"
#include "Commands.h"
#include "LibraryRepository.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
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
    std::vector<std::shared_ptr<LibraryRepository>> r;
    std::string filter;
    std::string value;
    char partialMatch;

    std::string attribute = std::find_if(std::begin(from_case),
            std::end(from_case),
            [&command](auto e){
            return e.second == flag_case.at(command);
            })->first;
    if(isTotal){
        db->query(r, attribute, "", "", false, true);
        if (r.empty()) std::cout << "No results." << std::endl;
    } 
    else {
        std::cout << "Type the the attribute you will look for:\n>";
        db->printAttr(attribute);
        std::getline(std::cin, filter);
        std::cout << "Type the the value you want to check\n>";
        std::getline(std::cin, value);
        std::cout << "Do you want to include partial matches? (Y/N): ";
        std::cin >> partialMatch;

        db->query(r, attribute, filter, value, partialMatch == 'Y' || partialMatch == 'y', false);
        if (r.empty()) std::cout << "No results." << std::endl;
    }
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
                Book* book = new Book(false);
                db->insertOrUpdate(*book);
                std::cout << "-------------\nBook inserted successfully.\n-------------\n\n" << std::endl;
                return;
            }
            case 2: {
                Member* member = new Member();
                Member res = db->insertOrUpdate(*member);
                std::cout << "-------------\nMember inserted successfully.\n-------------\n\n" << std::endl;
            }
            case 3: {
                Transaction* transaction = new Transaction();
                Transaction res = db->insertOrUpdate(*transaction);
                std::cout << "-------------\nTransaction inserted successfully.\n-------------\n\n" << std::endl;
            }
            case 4: {
                Sanction* sanction = new Sanction();
                Sanction res = db->insertOrUpdate(*sanction);
                std::cout << "-------------\nSanction inserted successfully.\n-------------\n\n" << std::endl;
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}

void updatePath(Database* db, std::string command) {
    try{
        switch (flag_case.at(command)) {
            case 1: {
                Book* book = new Book(true);
                Book res = db->insertOrUpdate(*book);
                std::cout << "-------------\nBook updated successfully.\n-------------\n\n" << std::endl;
                return;
            }
            case 2: {
                Member* member = new Member(true);
                Member res = db->insertOrUpdate(*member);
                std::cout << "-------------\nMember updated successfully.\n-------------\n\n" << std::endl;
            }
            case 3: {
                Transaction* transaction = new Transaction(true);
                Transaction res = db->insertOrUpdate(*transaction);
            std::cout << "-------------\nTransaction updated successfully.\n-------------\n\n" << std::endl;
            }
            case 4: {
                Sanction* sanction = new Sanction(true);
                Sanction res = db->insertOrUpdate(*sanction);
            std::cout << "-------------\nSanction updated successfully.\n-------------\n\n" << std::endl;
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}