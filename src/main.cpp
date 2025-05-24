#include <iostream>
#include <string>
# include <sstream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);

    if (input.empty()) {
      continue;
    }
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "exit") {
      std::string arg;
      iss >> arg;
      if (arg == "0") {
        return 0;
      }
    }
    else if (command == "echo") {
      std::string word, output;
      while (iss >> word) {
        if (!output.empty()) {
          output += " ";
        }
        output += word;
      }
      std::cout << output << std::endl;
    }
    else if (command == "type") {
      std::string arg, output;
      iss >> arg;
      if (arg == "echo") {
          std::cout << "echo is a shell builtin" << std::endl;
      }
      else if (arg == "exit") {
          std::cout << "exit is a shell builtin" << std::endl;
      }
      else if (command == "type") {
          std::cout << "type is a shell builtin" << std::endl;
      }
      else {
          std::cout << arg << ": command not found" << std::endl;

      }
    }
    else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
