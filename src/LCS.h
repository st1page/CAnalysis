#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
class LCS {
 public:
  LCS(uint32_t len1, uint32_t len2) : len1_(len1), len2_(len2) {
    equal_ = (bool *)calloc(len1 * len2, sizeof(bool));
    dp_ = (uint32_t *)calloc(len1 * len2, sizeof(uint32_t));
  }
  ~LCS() {
    if (equal_ != nullptr) free(equal_);
    if (dp_ != nullptr) free(dp_);
  }

  int SetEqual(uint32_t x, uint32_t y) {
    if (x > len1_ || y > len2_) return 1;
    equal_[dim_redu(x, y)] = true;
    return 0;
  }

  struct Cursor {
    uint32_t i1;
    uint32_t i2;
  };

  std::vector<Cursor> Solve() {
    for (int i = 0; i < len1_; i++) {
      if (equal_[dim_redu(i, 0)])
        dp_[dim_redu(i, 0)] = 1;
      else
        dp_[dim_redu(i, 0)] = 0;
    }
    for (int j = 0; j < len2_; j++) {
      if (equal_[dim_redu(0, j)])
        dp_[dim_redu(0, j)] = 1;
      else
        dp_[dim_redu(0, j)] = 0;
    }
    for (int i = 1; i < len1_; i++)
      for (int j = 1; j < len2_; j++) {
        if (equal_[dim_redu(i, j)])
          dp_[dim_redu(i, j)] = dp_[dim_redu(i - 1, j - 1)] + 1;
        else
          dp_[dim_redu(i, j)] =
              std::max(dp_[dim_redu(i - 1, j)], dp_[dim_redu(i, j - 1)]);
      }
    get_ans(len1_ - 1, len2_ - 1);
    return ans_;
  }

 private:
  uint32_t dim_redu(uint32_t x, uint32_t y) { return x * len2_ + y; }
  void get_ans(int32_t x, int32_t y) {
    if (x < 0 || y < 0) return;
    if (equal_[dim_redu(x, y)]) {
      get_ans(x - 1, y - 1);
      ans_.push_back({x, y});
    } else {
      if (x>0 && dp_[dim_redu(x - 1, y)] == dp_[dim_redu(x, y)])
        get_ans(x - 1, y);
      else
        get_ans(x, y - 1);
    }
  }
  uint32_t len1_;
  uint32_t len2_;
  bool *equal_ = nullptr;
  uint32_t *dp_ = nullptr;
  std::vector<Cursor> ans_;
};