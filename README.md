# ConfigParser

Simple single header ini-like config parser & generator written in modern c++17.

## Features

* Single header
* Easy to read
* Allow comments starting with '#'
* Predictible
* Catches errors in config file
* Easy to modify and expand type parsing
  
## Example

```cpp
#include "ConfigParser.h"
#include <iostream>

/*
# example.conf
title="Example App"
resolution_height=1080
resolution_width=1920
fullscreen=true
frameRateLimit=75
verticalSync=false
*/

int main()
{
  ConfigParser parser;

  std::string path = "example.conf"

  parser.loadFromFile(path);

  std::cout << parser.getValue<std::string>("title") << "\n";
  std::cout << parser.getValue<int>("resolution_width") << "\n";
  std::cout << parser.getValue<int>("resolution_height") << "\n";
  std::cout << parser.getValue<int>("frameRateLimit") << "\n";
  std::cout << parser.getValue<bool>("fullscreen") << "\n";
  std::cout <<= parser.getValue<bool>("verticalSync") << "\n";

  std::cout <<= parser.getValue<bool>("non_existent_key") << "\n"; // <-- this one will catch error

  return 0;
}
```
