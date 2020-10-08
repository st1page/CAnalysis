#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "MiniCLexer.h"
#include "MiniCParser.h"
#include "MiniCVisitor.h"

struct Type {
 public:
  std::string name_;
  // left value 0, right value 1
  int value_type;

  bool is_void_ = false;
  bool is_int_ = false;
  bool int_is_signed_;
  uint32_t int_length_;
  bool is_ptr_ = false;
  std::vector<uint32_t> ptr_arr_lens_;
  // TODO(st1page): add struct Type
  //    bool is_struct_ = false;
  //        std::vector<std::shared_ptr<Type>> struct_var_types_;
  std::string ToString(std::string arg_name = "");
};

struct Arg {
  std::shared_ptr<Type> type_;
  std::string name_;
  Arg(): name_("NoneArg") {}
  Arg(std::string name, std::shared_ptr<Type> type)
      : type_(type), name_(name) {}
};

class Function {
 public:
  std::string name_;

  std::vector<Arg> args_;
  std::shared_ptr<Type> ret_type_;

  MiniCParser::FuncContext* def_ctx_;

  std::vector<Function*> called_funcs_;
  std::unordered_map<std::string, std::shared_ptr<Type>> symbol_table_;

  std::string ToString();
};

class CompUnit {
 public:
  std::string file_name_;
  antlr4::ANTLRInputStream* input_;
  MiniCLexer* lexer_;
  antlr4::CommonTokenStream* tokens_;

  MiniCParser* parser_;

  std::unordered_map<std::string, std::unique_ptr<Function>> func_table_;
  std::unordered_map<std::string, std::shared_ptr<Type>> global_symbol_table_;

  CompUnit(char* file_name);
  ~CompUnit();
  // detect all functions & make the func table
  void DetectFuncs();

  // gen the call graph & symbol tables
  void Analysis();

  void PrintFuncList();
};
