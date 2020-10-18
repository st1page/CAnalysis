#include <iostream>
#include <cstdlib>
#include "CodeDiffer.h"
#include "CompUnit.h"

int main(int argc, const char **argv) {
  /*  if (argc != 2) {
      std::cerr << "[error] Input sourcefile required: " << argv[0]
                << " <sourcefile>\n";
      return 1;
    }
    CompUnit *unit = new CompUnit((char *)argv[1]);
    std::cout << std::string(unit->file_name_) << endl;
    unit->PreAnalyze();
    unit->PrintFuncCall();
  */
  if (argc != 3) {
    std::cerr << "[error] Input sourcefile required: " << argv[0]
              << " <file1> <file2>\n";
    return 1;
  }
  CompUnit *unit1 = new CompUnit((char *)argv[1]);
  CompUnit *unit2 = new CompUnit((char *)argv[2]);
  unit1->PreAnalyze();
  unit1->PrintFuncCall();
  unit2->PreAnalyze();
  unit2->PrintFuncCall();

  CodeDiffer differ;
  differ.UnitLCSDiff(unit1, unit2);

  return 0;
}
