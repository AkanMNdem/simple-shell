#include <iostream>
#include <string>
# include <sstream>
#include <unistd.h>

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
      std::string arg;
      iss >> arg;
      if (arg == "echo") {
          std::cout << "echo is a shell builtin" << std::endl;
      }
      else if (arg == "exit") {
          std::cout << "exit is a shell builtin" << std::endl;
      }
      else if (arg == "type") {
          const char *path_var = std::getenv("PATH");
          std::istringstream iss2(path_var);
          std::string path;
          bool found = false;
          iss >> arg;
          while (std::getline(iss2, path, ':')) {
            std::string full_path = path + "/" + arg;
            if (access(full_path.c_str(), X_OK) == 0) {
              std::cout << arg << " is" << full_path << std::endl;
              found = true;
              break;
            }
          }

          if (!found) {
            std::cout << arg << ": not found" << std::endl;
          }

          std::cout << "type is a shell builtin" << std::endl;

      }
      else {
          std::cout << arg << ": not found" << std::endl;

      }
    }
    else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
