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
  unit->PreAnalyze();
  unit->PrintFuncCall();
  
  // if (argc != 3) {
  //   std::cerr << "[error] Input sourcefile required: " << argv[0]
  //             << " <file1> <file2>\n";
  //   return 1;
  // }
  // CompUnit *unit1 = new CompUnit((char *)argv[1]);
  // CompUnit *unit2 = new CompUnit((char *)argv[2]);
  // unit1->DetectFuncs();
  // unit2->DetectFuncs();

  // MiniCDifferVistor differ;

  // for (auto const &it1 : unit1->func_table_) {
  //   for (auto const &it2 : unit1->func_table_) {
  //     Function *func1 = it1.second.get();
  //     Function *func2 = it2.second.get();

  //     std::cout << " differ start\n";
  //     std::cout << func1->ToString() << "\n";
  //     std::cout << func2->ToString() << "\n";
  //     differ.similarity(func1->def_ctx_->blockItem(),
  //                       func2->def_ctx_->blockItem());
  //   }
  // }

  return 0;
}
