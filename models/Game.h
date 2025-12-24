#ifndef GAME_H
#define GAME_H

#include <string>

struct Game {
    int ID;
    std::string Name;
    std::string Price;
    int Quantity;
};

void displayGame(const Game& game);

#endif