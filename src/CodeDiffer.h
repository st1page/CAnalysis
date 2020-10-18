#pragma once

#include <algorithm>
#include <iostream>

#include "CompUnit.h"
#include "LCS.h"
class CodeDiffer {
 public:
  static double FuncDiff(Function* func1, Function* func2) {
    uint32_t len1 = func1->stmts_.size();
    uint32_t len2 = func2->stmts_.size();
    LCS lcs(len1, len2);
    for (int i = 0; i < len1; i++)
      for (int j = 0; j < len2; j++) {
        if (func1->stmts_[i].Equal(func2->stmts_[j])) lcs.SetEqual(i, j);
      }
    uint32_t lcs_len = lcs.Solve().size();
    std::cout << len1 << "  len1\n";
    std::cout << len2 << "  len2\n";

    std::cout << lcs_len << "  lcs len\n";
    return 1.0 * lcs_len / std::min(len1, len2);
  }
  static double UnitLCSDiff(CompUnit* unit1, CompUnit* unit2) {
    for (auto const& it1 : unit1->func_table_) {
      for (auto const& it2 : unit2->func_table_) {
        double x = FuncDiff(it1.second.get(), it2.second.get());
        std::cout << x << "\n";

        if (x > 0.8) {
          std::cout << "\n";
          std::cout << it1.second->ToString() << "\n";
          std::cout << it2.second->ToString() << "\n";
          std::cout << "Coincidence radio: " << x << "\n";
        }
      }
    }
  }
  static double UnitCallGraphDiff(CompUnit* unit1, CompUnit* unit2) {
    //TODO(st1page): graph diff
    unit1->PrintFuncCall();
    unit2->PrintFuncCall();
    uint32_t cnt = 0;
    double sum = 0.0;
    for (auto const& it1 : unit1->func_table_) {
      for (auto const& it2 : unit2->func_table_) {
        double x = FuncDiff(it1.second.get(), it2.second.get());
        if (x > 0.8) {
          cnt++;
          sum += x;
        }
      }
    }
    if (sum / cnt > 0.9) {
      std::cout << "\n";
      std::cout << "Coincidence radio: " << sum / cnt << "\n";
    }
  }
};