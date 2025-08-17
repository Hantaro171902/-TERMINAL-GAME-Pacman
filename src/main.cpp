#include "game.hpp"
#include <iostream>

using namespace std;

int main() {
    try {
        Game game;
        game.start();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred" << endl;
        return 1;
    }
    
    return 0;
}
