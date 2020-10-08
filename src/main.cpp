#include <iostream>

#include "CompUnit.h"

using namespace std;
int main(int argc, const char **argv) {
  if (argc != 2) {
    std::cerr << "[error] Input sourcefile required: " << argv[0]
              << " <sourcefile>\n";
    return 1;
  }
  CompUnit *unit = new CompUnit((char *)argv[1]);
  std::cout << std::string(unit->file_name_) << endl;
  unit->DetectFuncs();
  unit->PrintFuncCall();
  return 0;

}
