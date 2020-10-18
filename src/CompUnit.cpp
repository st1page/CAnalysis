#include "CompUnit.h"

#include <fstream>
#include <iostream>

#include "MiniCLexer.h"
#include "MiniCParser.h"
#include "MiniCVisitor.h"
#include "PreAnalyzer.h"

std::string Type::ToString(std::string arg_name) {
  if (is_ptr_) {
    std::string ret = name_;
    int i = ptr_arr_lens_.size() - 1;
    while (i >= 0 && ptr_arr_lens_[i] == 0) {
      ret.push_back('*');
      i--;
    }
    ret += " " + arg_name;
    while (i >= 0) {
      ret += "[" + std::to_string(ptr_arr_lens_[i]) + "]";
      i--;
    }
    return ret;
  } else {
    return name_ + " " + arg_name;
  }
}

#define ENUM_TYPE_CASE(x) \
  case x:                 \
    return std::string(#x);

std::string Stmt::Cate2String(const enum Cate cate) {
  switch (cate) {
    ENUM_TYPE_CASE(VarDef)
    ENUM_TYPE_CASE(Return)
    ENUM_TYPE_CASE(SingleExpr)
    ENUM_TYPE_CASE(BlockStart)
    ENUM_TYPE_CASE(BlockEnd)
    ENUM_TYPE_CASE(If)
    ENUM_TYPE_CASE(Else)
    ENUM_TYPE_CASE(While)
    ENUM_TYPE_CASE(DoWhile)
    ENUM_TYPE_CASE(For)
    ENUM_TYPE_CASE(Break)
    ENUM_TYPE_CASE(Continue)
  }
  return "Unsupported Cate";
}

#undef ENUM_TYPE_CASE

bool Stmt::Equal(const Stmt &other) {
  if (cate_ != other.cate_) return false;
  else return true;
//  TODO(st1page): compare different expr   
/*  switch (cate_) {
    case VartrueDef:
      break;
    case Return:
      break;
    case SingleExpr:
      break;    
    case If:
      break;
    case While:
      break;
    case DoWhile:
      break;
    case For:
      break;
    case BlockStart:
    case BlockEnd:
    case Else:
    case Break:
    case Continue:
      return ;
  }
  return false;
*/
}

std::string Function::ToString() {
  std::string ret = "";
  ret += ret_type_->ToString();
  ret += " " + name_ + "(";
  for (int i = 0; i < args_.size(); i++) {
    if (i) ret += ",";
    ret += args_[i].type_->ToString(args_[i].name_);
  }
  ret += ")";
  return ret;
}

CompUnit::CompUnit(char *file_name) {
  file_name_ = file_name;
  std::ifstream file;
  file.open(file_name_);
  if (file.fail()) {
    std::cerr << "open file " << std::string(file_name_) << " error\n";
    exit(1);
  }

  input_ = new antlr4::ANTLRInputStream(file);
  lexer_ = new MiniCLexer(input_);
  tokens_ = new antlr4::CommonTokenStream(lexer_);
  parser_ = new MiniCParser(tokens_);

  file.close();
}

CompUnit::~CompUnit() {
  if (input_ != nullptr) delete input_;
  if (lexer_ != nullptr) delete lexer_;
  if (tokens_ != nullptr) delete tokens_;
  if (parser_ != nullptr) delete parser_;
}

// detect all functions & make the func table
void CompUnit::PreAnalyze() {
  PreAnalyzer pre_analyzer(this);
  pre_analyzer.visitProg(parser_->prog());
}

void CompUnit::PrintFuncList() {
  std::cout << "functions of compile unit " << file_name_ << std::endl;
  for (auto const &it : func_table_) {
    std::cout << it.second->ToString() << std::endl;
  }
}
void CompUnit::PrintFuncCall() {
  std::cout << "functions of compile unit " << file_name_ << std::endl;
  for (auto const &it : func_table_) {
    std::cout << it.second->ToString() << std::endl;
    std::cout << "calling:" << std::endl;
    for (auto const &func : it.second->call_funcs_) {
      std::cout << "    " << func->ToString() << std::endl;
    }
  }
}
