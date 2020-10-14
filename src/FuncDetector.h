#pragma once

#include "CompUnit.h"
#include "MiniCBaseVisitor.h"
#include "MiniCTypeVisitor.h"

class FuncDectector : public MiniCTypeVisitor {
 private:
  CompUnit *comp_unit_;
  Function *cur_func_;

  Function *ParseFuncDecl(MiniCParser::FuncContext *ctx, Function *func) {
    // TODO(st1page): check if the args is same when the func has been decl.
    func->def_ctx_ = ctx;
    func->ret_type_ = visit(ctx->type());
    func->args_.resize(ctx->varDecl().size());
    for (auto i = 0; i < ctx->varDecl().size(); ++i) {
      func->args_[i] = visit(ctx->varDecl(i));
      auto res = func->symbol_table_.emplace(func->args_[i].name_,
                                             func->args_[i].type_);
      if (func->stat_ == Function::Stat::unknown && !res.second) {
        std::cerr << "In file " << comp_unit_->file_name_ << " ";
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of the arg" << func->args_[i].name_
                  << "\n";
        exit(1);
      }
    }
    return func;
  }

 public:
  FuncDectector(CompUnit *comp_unit)
      : comp_unit_(comp_unit), MiniCTypeVisitor() {}
  virtual antlrcpp::Any visitFunc(MiniCParser::FuncContext *ctx) override {
    std::string func_name = ctx->Ident()->getText();
    auto res = comp_unit_->func_table_.emplace(func_name,
                                               std::make_unique<Function>());
    Function *func = res.first->second.get();
    func->name_ = func_name;

    if (ctx->Semicolon()) {
      // function declearation
      ParseFuncDecl(ctx, func);
      func->stat_ = Function::Stat::decleared;
      return func;
    } else {
      // function defination
      if (func->stat_ == Function::Stat::defined) {
        std::cerr << "In file " << comp_unit_->file_name_ << " ";
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of function " << func_name << "\n";
        exit(1);
      }
      ParseFuncDecl(ctx, func);
      func->stat_ = Function::Stat::defined;
      cur_func_ = func;
      for (int i = 0; i < ctx->blockItem().size(); i++) {
        visit(ctx->blockItem(i));
      }
      return func;
    }
  }
  virtual antlrcpp::Any visitFuncCall(MiniCParser::FuncCallContext *ctx) override {
    std::string func_name = ctx->Ident()->getText();
    auto it = comp_unit_->func_table_.find(func_name);
    if(it==comp_unit_->func_table_.end()) {
        std::cerr << "In file " << comp_unit_->file_name_ << " ";
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] undeclared function " << func_name << "\n";
        exit(1);
    }
    cur_func_->call_funcs_.push_back(it->second.get());
    return visitChildren(ctx);
  }

};