#pragma once

#include "MiniCBaseVisitor.h"
#include "MiniCParser.h"
class BadFunction{
  std::string name;
  std::string desprition;
};
class MemAllocFunction{
  std::string name;
  uint32_t siz_arg;
  uint32_t num_arg;
  uint32_t siz_val;
  uint32_t num_val;
};
class MemUseFunction{
  std::string name;
  uint32_t ptr_arg;
  uint32_t siz_arg;
  uint32_t num_arg;
  uint32_t siz_val;
  uint32_t num_val;
};

class SecAnalyzer : public MiniCBaseVisitor{
public:
  virtual antlrcpp::Any visitFunc(MiniCParser::FuncContext *ctx) override {
    return visitChildren(ctx);
  }

private:
  std::vector<BadFunction> bad_funcs_;
  std::vector<MemAllocFunction> alloc_funcs_;
  std::vector<MemUseFunction> use_funcs_;

  char *report_file_name_;
};