#pragma once

#include <unordered_map>

#include "MiniCParser.h"
#include "MiniCTypeVisitor.h"
struct BadFunction {
  std::string name;
  std::string desprition;
};
struct MemAllocFunction {
  std::string name;
  uint32_t siz_arg_place;
  uint32_t num_arg_place;
};
struct MemUseFunction {
  std::string name;
  uint32_t ptr_arg_place;
  uint32_t siz_arg_place;
};

class SecAnalyzer : public MiniCTypeVisitor {
 public:
  SecAnalyzer(CompUnit *comp_unit,const char *funcs_file_name);

 private:
  std::unordered_map<std::string, BadFunction> bad_funcs_;
  std::unordered_map<std::string, MemAllocFunction> alloc_funcs_;
  std::unordered_map<std::string, MemUseFunction> use_funcs_;

  CompUnit *comp_unit_;
  Function *cur_func_;
  const char *func_list_file_name_;
};