#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

// Author @eeeENVII 

/*
    supported features:
        single header file
        type-safe setting values
        # comments are ignored by parser
        key=value dictionary style
        check for valid key names with accordance to cpp variables
        check for duplicate keys   
    supported types at ConfigValue --> 

    TODO:
    variable interpolation with $(variable) ? With implicit or explicit order
    MAYBE:
    maybe some specification for order? change to bst instead hashTable 
*/

#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <variant>
#include <vector>
#include <iostream>

class ConfigParser
{
private:
    // Modern aliases
    using ConfigValue = std::variant<std::string, unsigned int, int, float, bool>; // remember to add support to parseValue & saveToFile
    using ConfigMap = std::unordered_map<std::string, ConfigValue>;

    ConfigMap configMap;

    inline std::string trim(const std::string& str);
    inline bool isKeyValid(const std::string& key) const;
    inline ConfigValue parseValue(const std::string& value);
public:
    inline ConfigParser() = default;
    inline ~ConfigParser() = default;

    inline void loadFromFile(const std::string& filename);
    inline void saveToFile(const std::string& filename) const;

    inline void clear(); // Clears all entries
    inline void clearKeys(const std::vector<std::string>& keys); // Select keys to clear

    inline std::vector<std::string> getKeys() const; 

    template<typename T>
    inline T getValue(const std::string& key) const;

    template <typename T>
    inline void setValue(const std::string& key, const T& value);

};

inline std::string ConfigParser::trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

inline bool ConfigParser::isKeyValid(const std::string& key) const {

    // Key should not be empty
    if (key.empty()) return false;

    // Check if the first character is a letter or underscore
    if (!std::isalpha(key[0]) && key[0] != '_') return false;

    // Check remaining characters are alphanumeric or underscores
    for (size_t i = 1; i < key.size(); ++i) {
        if (!std::isalnum(key[i]) && key[i] != '_') return false;
    }

    return true;
}

// TODO:    support more value_types if neccesary and update saveToFile
// TODO:    multiple '-' might be problem ? stoi/stof cuts string after non numerical tho
inline ConfigParser::ConfigValue ConfigParser::parseValue(const std::string& value) 
{
        if (value.front() == '"' && value.back() == '"') {
            return value.substr(1, value.size() - 2);   // String 

        } else if (value.find_first_not_of("0123456789-") == std::string::npos && 
        (value[0] == '-' || value.find_first_not_of('-') != std::string::npos)) {
            
            try {
                return std::stoi(value);                // Int
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << ". Please enter a valid integer." << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << ". The number is too large or too small." << std::endl;
            } 

        } else if (value.find_first_not_of("0123456789.-") == std::string::npos &&
        std::count(value.begin(), value.end(), '.') == 1 &&
        (value[0] == '-' || value[0] == '.' || isdigit(value[0]))) {

            try {
                return std::stof(value);                // Float   
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << ". Please enter a valid float." << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << ". The number is too large or too small." << std::endl;
            }

        } else if (value == "0" || value == "1" || value == "true" || value == "false") {
            return  (value == "true" || value == "1"); // Bool
        }

        throw std::invalid_argument("configParserError: Unknown value type: " + value);
    }

inline void ConfigParser::loadFromFile(const std::string& filename) 
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("configParserError: Unable to open file for reading: " + filename);
    }

    std::string line;
    int line_number = 0;

    while (std::getline(file, line)) 
    {
        line_number++;
        line = trim(line);

        // Ignore empty lines and comments
        if (!(line.empty() || line[0] == '#'))
        {
            size_t pos = line.find('=');
            if (pos == std::string::npos) 
            {
                throw std::runtime_error("configParserError: Line syntax error" + std::to_string(line_number) + ": no '=' sign");
            }

            std::string key = trim(line.substr(0, pos)); 
            std::string value = trim(line.substr(pos + 1));

            // Check if the key is valid
            if (!isKeyValid(key)) {
                throw std::runtime_error("configParserError: Invalid key name at line " + std::to_string(line_number) + ": '" + key + "'");
            }

            // finding key duplicate
            if (configMap.find(key) != configMap.end()) {
                throw std::runtime_error("configParserError: Key '" + key + "' duplicated at line: " + std::to_string(line_number));
            }

            // Parse value and assign to key in HashMap
            configMap[key] = parseValue(value);
        }
    }

    // unnecessary in modern C++11 with RAII
    file.close();
}

// TODO: Maybe add support for comments ?
inline void ConfigParser::saveToFile(const std::string& filename) const 
{
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }

    for (const auto& [key, value] : configMap) {
        file << key << '=';

        // Format the value based on its type
        if (std::holds_alternative<std::string>(value)) {
            file << '"' << std::get<std::string>(value) << '"';  // Enclose strings in quotes

        } else if (std::holds_alternative<int>(value)) {
            file << std::get<int>(value);                        // Output integer as-is

        } else if (std::holds_alternative<unsigned int>(value)) {
            file << std::get<unsigned int>(value);              // Output unsigned integer as-is

        } else if (std::holds_alternative<float>(value)) {
            file << std::get<float>(value);                      // Output float as-is

        } else if (std::holds_alternative<bool>(value)) {
            file << (std::get<bool>(value) ? "true" : "false");  // Use "true" or "false" for bools

        }

        file << '\n'; 
    }

    file.close();
}

inline void ConfigParser::clear()
{
    configMap.clear(); 
}

inline void ConfigParser::clearKeys(const std::vector<std::string>& keys) 
{
    for (const auto& key : keys) 
    {
        configMap.erase(key);
    }
}

inline std::vector<std::string> ConfigParser::getKeys() const 
{
    std::vector<std::string> keys;
    for (const auto& [key, _] : configMap) {
        keys.push_back(key);
    }
    return keys;
}

template<typename T>
inline T ConfigParser::getValue(const std::string& key) const 
{
    auto it = configMap.find(key);
    if (it == configMap.end()) 
    {
        throw std::runtime_error("configParserError: The key '" + key + "' was not found in configuration.");
    }

    return std::get<T>(it->second);
}

template <typename T>
inline void ConfigParser::setValue(const std::string& key, const T& value) {
        if (!isKeyValid(key)) {
            throw std::runtime_error("Invalid key name: '" + key + "'");

        }
        configMap[key] = std::move(value);
    }

#endif // CONFIGPARSER_H
