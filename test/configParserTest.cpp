#include "..\configParser.h"
#include <iostream>

int main() 
{
    ConfigParser parser;

    // 1. loadFromFile Good Syntax expected
    // gotta check ASSERT in future
    try {
        parser.loadFromFile("Test1.config");

        std::string str_1 = parser.getValue<std::string>("str_1");
        std::string str_2 = parser.getValue<std::string>("str_2");
        std::string str_3 = parser.getValue<std::string>("str_3");
        std::string str_4 = parser.getValue<std::string>("str_4");
        std::string str_5 = parser.getValue<std::string>("str_5");

        std::cout << "str_1:" << str_1 << "\n";
        std::cout << "str_2:" << str_2 << "\n";
        std::cout << "str_3:" << str_3 << "\n";
        std::cout << "str_4:" << str_4 << "\n";
        std::cout << "str_5:" << str_5 << "\n";

        int i_1 = parser.getValue<int>("i_1");
        int i_2 = parser.getValue<int>("i_2");
        int i_3 = parser.getValue<int>("i_3");
        int i_4 = parser.getValue<int>("i_4");

        // we expect int to be >= 0 and it works
        unsigned int u_1 = parser.getValue<int>("i_1") + parser.getValue<int>("i_1");
        std::cout << "u_1:" << u_1 << "\n";

        std::cout <<  "i_1:" << i_1 << "\n";
        std::cout <<  "i_2:" << i_2 << "\n";
        std::cout <<  "i_3:" << i_3 << "\n";
        std::cout <<  "i_4:" << i_4 << "\n";

        float f_1 = parser.getValue<float>("f_1");
        float f_2 = parser.getValue<float>("f_2");
        float f_3 = parser.getValue<float>("f_3");
        float f_4 = parser.getValue<float>("f_4");

        std::cout << "f_1:" << f_1 << "\n";
        std::cout << "f_2:" << f_2 << "\n";
        std::cout << "f_3:" << f_3 << "\n";
        std::cout << "f_4:" << f_4 << "\n";
        std::cout << "f_3 - f_4:" << f_3 - f_4 << "\n";

        bool b_1 = parser.getValue<bool>("b_1");
        bool b_2 = parser.getValue<bool>("b_2");
       
        std::cout << "b_1:" << b_1 << "\n";
        std::cout << "b_2:" << b_2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 2. saveToFile

    
    try {
        parser.clear();

        // Setting values of different types using templates
        parser.setValue("title","New Game Config");
        parser.setValue("version", 14.2f);
        parser.setValue("resolution_width", 1920);
        parser.setValue("resolution_height", 1080);
        parser.setValue("fullscreen", true);
        parser.setValue("verticalSync", false);
        parser.setValue("contextSettings_antialiasingLevel", 16);

        unsigned int u_1 = 420;
        parser.setValue("u_1",u_1);

        // Getting values back (with type-safety)
        std::cout << parser.getValue<std::string>("title") << "\n";
        std::cout << parser.getValue<float>("version") << "\n";
        std::cout << parser.getValue<int>("resolution_width") << "\n";
        std::cout << parser.getValue<int>("resolution_height") << "\n";
        std::cout << parser.getValue<bool>("fullscreen") << "\n";
        std::cout << parser.getValue<bool>("verticalSync") << "\n";
        std::cout << parser.getValue<int>("contextSettings_antialiasingLevel") << "\n";
        std::cout << parser.getValue<unsigned int>("u_1") << "\n";
        parser.clearKeys({"contextSettings_antialiasingLevel","version"});

        parser.saveToFile("Test2.config");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}