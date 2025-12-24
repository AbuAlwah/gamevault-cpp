#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

struct User {
    std::string username;
    std::string password;
    std::string role;
    bool isAdmin;
    double balance;
    std::vector<std::string> ownedGames;
};

void displayUserInfo(const User& user);
bool checkPassword(const User& user, const std::string& input);

#endif