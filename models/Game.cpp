#include "Game.h"
#include <iostream>

void displayGame(const Game& game) {
    std::cout << "[" << game.ID << "] "
              << game.Name << " - $"
              << game.Price<<" (Qty: "<< game.Quantity << ")" << std::endl;
}