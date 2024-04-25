#include <iostream>
#include <unordered_map>

const std::unordered_map<std::string,int> command_case{
   {"-h", 0},
   {"ins", 1},
   {"del", 2},
   {"mod", 3},
   {"sel", 4}
};

const std::unordered_map<std::string,int> flag_case{
   {"-a", 0},
   {"-b", 1},
   {"-m", 2},
   {"-t", 3},
   {"-s", 4}
};

const std::unordered_map<std::string,int> from_case{
   {"books", 0},
   {"members", 1},
   {"transactions", 2},
   {"sanctions", 3},
};
