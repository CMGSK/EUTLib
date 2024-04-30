#include "Database.h"
#include "Commands.h"
#include "LibraryRepository.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <memory>
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
    
    std::cout << "Insert a command. Type -h for help. \n ";
    while(true){
        std::cout << ">" ;
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
                    int comp = strcmp(command, "-a");
                    selectionPath(db, flag, comp==0);
                    break;
                }
            }
        } catch (...) {
            std::cout << "Invalid command. If you need help, type -h.\n>";
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
    std::string partialMatch;
    std::string attribute = std::find_if(std::begin(from_case),
        std::end(from_case),
        [&command](auto e){
            return e.second == flag_case.at(command)-1;
        })->first;

    if(isTotal){
        db->query(r, attribute, "", "", false, true);
    } else {
        std::cout << "Type the the attribute you will look for:\n>";
        db->printAttr(attribute);
        std::getline(std::cin, filter);
        std::cout << "Type the the value you want to check\n>";
        std::getline(std::cin, value);
        std::cout << "Do you want to include partial matches? (Y/N): ";
        std::getline(std::cin, partialMatch); 
        db->query(r, attribute, filter, value, (partialMatch == "Y" || partialMatch == "y"), false);
    }

    if (r.empty()) std::cout << "No results." << std::endl;
    else{
        switch (flag_case.at(command)) {
            case 1:
                for (auto item : r)
                    std::cout << std::static_pointer_cast<Book>(item)->toString();
                break;
            case 2:
                for (auto item : r)
                    std::cout << std::static_pointer_cast<Member>(item)->toString();
                break;
            case 3:
                for (auto item : r)
                    std::cout << std::static_pointer_cast<Transaction>(item)->toString();
                break;
            case 4:
                for (auto item : r)
                    std::cout << std::static_pointer_cast<Sanction>(item)->toString();
                break;
        }
    }
}

void deletionPath(Database* db, std::string command){
    std::string attribute = std::find_if(std::begin(from_case),
        std::end(from_case),
        [&command](auto e){
            return e.second == flag_case.at(command)-1;
        })->first;
    std::string input;
    char bulk;
    std::cout << "Do you need to delete more than one item? (Y/N): ";
    std::cin >> bulk;
    if (bulk == 'Y' || bulk == 'y'){
        std::cout << "Input all the IDs and type 'done' to submit.";
        std::vector<int> id;
        while (input != "done"){
            try{
                std::cout << "\n>";
                std::getline(std::cin, input);
                id.push_back(atoi(input.c_str()));
            } catch (...) { 
                std::cout << "Thats not a valid ID nor command.\n>";
                continue;
            }
        }
        if (db->deleteBulk(attribute, id)) std::cout << "Success. \n--------\n";
    } else {
        std::cout << "Input the target ID.\n>";
        try {
            std::string input;
            std::cin.get();
            std::getline(std::cin, input);
            int id = stoi(input);
            if (db->deleteSingle(attribute, id)) std::cout << "Success. \n--------\n";
        } catch (...){
            std::cout << "Thats not a valid ID nor command.\n>";
        }
    }
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
                Member* member = new Member(false);
                Member res = db->insertOrUpdate(*member);
                std::cout << "-------------\nMember inserted successfully.\n-------------\n\n" << std::endl;
                return;
            }
            case 3: {
                Transaction* transaction = new Transaction(false);
                db->insertOrUpdate(*transaction);
                std::cout << "-------------\nTransaction inserted successfully.\n-------------\n\n" << std::endl;
                return;
            }
            case 4: {
                Sanction* sanction = new Sanction(false);
                db->insertOrUpdate(*sanction);
                std::cout << "-------------\nSanction inserted successfully.\n-------------\n\n" << std::endl;
                return;
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
                return;
            }
            case 2: {
                Member* member = new Member(true);
                Member res = db->insertOrUpdate(*member);
                return;
            }
            case 3: {
                Transaction* transaction = new Transaction(true);
                Transaction res = db->insertOrUpdate(*transaction);
                return;
            }
            case 4: {
                Sanction* sanction = new Sanction(true);
                Sanction res = db->insertOrUpdate(*sanction);
                return;
            }
        }
    } catch (...) { std::cout << "Flag is not valid. If you need help, type -h."; }
}