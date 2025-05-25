#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <cstring>


std::vector<std::string> parse_args(const std::string &input) {
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    return args;
  }



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

    std::vector<std::string> args = parse_args(input);
    // std::istringstream iss(input);
    std::string command = args[0];
    // iss >> command;

    if (args[0] == "exit") {
      if (args[1] == "0") {
        return 0;
      }
    }
    else if (args[0] == "echo") {
      std::string output;
      for (int i = 1; i < args.size(); i++) {
        if (!output.empty()) {
          output += " ";
        }
        output += args[i];
      }
      std::cout << output << std::endl;
    }
    else if (args[0] == "type") {
      if (args[1] == "echo" || args[1] == "exit" || args[1] == "type") {
          std::cout << args[1] << " is a shell builtin" << std::endl;
      }

      else {
          const char *path_var = std::getenv("PATH");
          std::istringstream iss2(path_var);
          std::string path;
          bool found = false;
          std::string arg = args[1];
          while (std::getline(iss2, path, ':')) {
            std::string full_path = path + "/" + arg;
            if (access(full_path.c_str(), X_OK) == 0) {
              std::cout << args[2] << " is " << full_path << std::endl;
              found = true;
              break;
            }
          }
          if (!found) {
            std::cout << arg << ": not found" << std::endl;
          }
      }
    }
    else {
      const char *path_var = std::getenv("PATH");
      std::istringstream iss2(path_var);
      std::string path;
      bool found = false;
      std::string arg = args[1];

      while (std::getline(iss2, path, ':')) {
        std::string full_path = path + "/" + arg;
        if (access(full_path.c_str(), X_OK) == 0) {
          std::vector<char*> exec_args;
          for (std::string &a : args) {
            exec_args.push_back(const_cast<char*>(a.c_str()));
          }
          exec_args.push_back(nullptr);

          pid_t pid = fork();

          if (pid == 0) {
            // Child process executes the command
            execvp(full_path.c_str(), exec_args.data());
            std::cerr << "Error executing " << full_path << ": " << strerror(errno) << std::endl;
            return 1;
          }
          else if (pid > 0){
            // Parent process waits for the child to finish
            int status;
            waitpid(pid, &status, 0);
            std::cout << "Command executed with PID: " << pid << std::endl;
          }
          else {
            // Fork failed
            std::cerr << "Fork failed: " << strerror(errno) << std::endl;
            return 1;
          }
        }
      }
    }
  }
  return 0;
}
