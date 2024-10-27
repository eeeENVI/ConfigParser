#include "..\configParser.h"
#include <iostream>

/*
# example.conf
title="Example App"
resolution_height=1080
resolution_width=1920
fullscreen=true
frameRateLimit=75
volume=0.5
*/

int main()
{
    ConfigParser parser;

    std::string path = "example.conf";

    parser.loadFromFile(path);

    std::cout << parser.getValue<std::string>("title") << "\n";
    std::cout << parser.getValue<int>("resolution_width") << "\n";
    std::cout << parser.getValue<int>("resolution_height") << "\n";
    std::cout << parser.getValue<int>("frameRateLimit") << "\n";
    std::cout << parser.getValue<bool>("fullscreen") << "\n";
    std::cout << parser.getValue<float>("volume") << "\n";

    try {
        std::cout << parser.getValue<bool>("non_existent_key") << "\n"; // <-- this one will throw error
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    } // When working with external data always nice to use try & catch block

    return 0;
}