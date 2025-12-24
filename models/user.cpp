#include "User.h"
#include <iostream>

void displayUserInfo(const User& user) {
    std::cout << "Username: " << user.username << std::endl;
    std::cout << "Balance: " << user.balance << std::endl;
}

bool checkPassword(const User& user, const std::string& input) {
    return user.password == input;
}