#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <cstring>


std::vector<std::string> parse_args(const std::string &input) {
    if (input.empty()) {
        return {};
    }
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    return args;
  }

std::string get_fullpath(const std::string &arguments) {
  const char *path_var = std::getenv("PATH");
  std::istringstream iss(path_var);
  std::string path;
  std::string arg = parse_args(arguments)[0];
  while (std::getline(iss, path, ':')) {
    std::string full_path = path + "/" + arg;
    if (access(full_path.c_str(), X_OK) == 0) {
      return full_path;
    }
  }
  return "";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);
    std::vector<std::string> args = parse_args(input);

    if (args.empty()) {
      continue; 
    }

    std::string command = args[0];

    if (args[0] == "exit") {
      if (args[1] == "0") {
        return 0;
      }
    }
    else if (args[0] == "echo") {
      std::string output;

      if (args.size() < 2) {
        std::cout << std::endl;
        continue;
      }      

      for (int i = 1; i < args.size(); i++) {
        if (!output.empty()) {
          output += " ";
        }
        output += args[i];
      }
      std::cout << output << std::endl;
    }
    else if (args[0] == "pwd"){
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
      }
      else {
        std::cerr << "Error getting current directory: " << strerror(errno) << std::endl;
      }
    }
    else if (args[0] == "type") {
      if (args[1] == "echo" || args[1] == "exit" || args[1] == "type") {
          std::cout << args[1] << " is a shell builtin" << std::endl;
      }
      else {
        std::string full_path = get_fullpath(args[1]);

        if (full_path.empty()) {
          std::cout << args[1] << ": not found " << std::endl;
          }
        else {
          std::cout << args[1] << " is " << full_path << std::endl;
        }
      }
    }
    else  {
      // if (args.size() < 2)  {
      //   continue;
      // }

      std::string full_path = get_fullpath(args[0]);

      if (!full_path.empty()) {
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
        }
        else {
          // Fork failed
          std::cerr << "Fork failed: " << strerror(errno) << std::endl;
          return 1;
        }
        }
        else {
          std::cerr << args[0] << ": command not found" << std::endl;
          continue;
        }
    }
  }
  return 0;
}
