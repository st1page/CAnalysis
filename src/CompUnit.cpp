#include "CompUnit.h"

#include <fstream>
#include <iostream>

#include "FuncDetector.h"
#include "MiniCLexer.h"
#include "MiniCParser.h"
#include "MiniCVisitor.h"

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
void CompUnit::DetectFuncs() {
  FuncDectector func_dectector(this);
  func_dectector.visitProg(parser_->prog());
}

// gen the call graph & symbol tables
void CompUnit::Analysis() {}

void CompUnit::PrintFuncList() {
  std::cout << "functions of compile unit " << file_name_ << std::endl;
  for (auto const &it : func_table_) {
    std::cout << it.second->ToString() << std::endl;
  }
}
