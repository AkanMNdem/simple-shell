#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;
  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);
    if (input == "0" ) {
      std::cout << "exit" << input << std::endl;
      break;
    }
    else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
