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

struct Var {
  std::shared_ptr<Type> type_;
  std::string name_;
  Var() : name_("NoneVar") {}
  Var(std::string name, std::shared_ptr<Type> type)
      : type_(type), name_(name) {}
};

class Stmt {
 public:
  enum Cate {
    VarDef,
    Return,
    SingleExpr,
    BlockStart,
    BlockEnd,
    If,
    Else,
    While,
    DoWhile,
    For,
    Break,
    Continue,
  };

 private:
  static std::string Cate2String(const enum Cate cate);

 public:
  MiniCParser::StmtContext* ctx_;
  Cate cate_;
  Stmt(Cate cate, MiniCParser::StmtContext* ctx) : cate_(cate), ctx_(ctx) {}
  std::string CateString() const { return Cate2String(cate_); }
  bool Equal(const Stmt& other);
};

class Function {
 public:
  uint32_t id_;

  std::string name_;
  enum Stat { unknown, decleared, defined } stat_;

  std::vector<Var> args_;
  std::shared_ptr<Type> ret_type_;

  MiniCParser::FuncContext* def_ctx_;

  std::vector<Function*> call_funcs_;
  std::unordered_map<std::string, std::shared_ptr<Type>> symbol_table_;

  std::vector<Stmt> stmts_;

  std::string ToString();
};

class CompUnit {
 public:
  std::string file_name_;
  antlr4::ANTLRInputStream* input_;
  MiniCLexer* lexer_;
  antlr4::CommonTokenStream* tokens_;

  MiniCParser* parser_;

  uint32_t func_num_ = 0;

  std::unordered_map<std::string, std::unique_ptr<Function>> func_table_;
  std::unordered_map<std::string, std::shared_ptr<Type>> global_symbol_table_;

  CompUnit(char* file_name);
  ~CompUnit();

  Function* AddFunc(std::string name);

  // detect all functions & make the func table
  void PreAnalyze();

  void SecAnalyze(const char *func_list_file);
  void PrintFuncList();

  void PrintFuncCall();
};
