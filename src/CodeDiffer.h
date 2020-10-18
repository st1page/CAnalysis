#pragma once

#include <algorithm>
#include <functional>
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
    if (len1 == 0 && len2==0) return 1;
    if (len1 < 5 || len2 < 5)
      return 1.0 * lcs_len / std::max(len1, len2);
    else
      return 1.0 * lcs_len / std::min(len1, len2);
  }
  static double UnitLCSDiff(CompUnit* unit1, CompUnit* unit2) {
    for (auto const& it1 : unit1->func_table_) {
      for (auto const& it2 : unit2->func_table_) {
        double x = FuncDiff(it1.second.get(), it2.second.get());

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
    // TODO(st1page): graph diff
    std::cout << "\n";
    unit1->PrintFuncCall();
    std::cout << "\n";
    unit2->PrintFuncCall();
    std::cout << "\n";

    std::vector<double> vi;
    for (auto const& it1 : unit1->func_table_) {
      for (auto const& it2 : unit2->func_table_) {
        double x = FuncDiff(it1.second.get(), it2.second.get());
        vi.push_back(x);
      }
    }
    sort(vi.begin(), vi.end(), std::greater<double>());
    uint32_t cnt = std::min(unit1->func_table_.size(), unit2->func_table_.size());
    double sum = 0.0;
    for(int i=0;i<cnt;i++) sum+=vi[i];
        std::cout << "Coincidence radio: " << sum << "\n";

    std::cout << "Coincidence radio: " << sum / cnt << "\n";
  }
};