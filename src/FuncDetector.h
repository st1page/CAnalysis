#pragma once

#include "CompUnit.h"
#include "MiniCBaseVisitor.h"
class FuncDectector : public MiniCBaseVisitor {
 private:
  CompUnit *comp_unit_;

 public:
  FuncDectector(CompUnit *comp_unit)
      : comp_unit_(comp_unit), MiniCBaseVisitor() {}
  antlrcpp::Any visitFunc(MiniCParser::FuncContext *ctx) override {
    std::string func_name = ctx->Ident()->getText();
    // function declearation
    if (ctx->Semicolon()) return visitChildren(ctx);

    // function defination
    auto res = comp_unit_->func_table_.emplace(func_name,
                                               std::make_unique<Function>());
    if (!res.second) {
      std::cerr << "In file " << comp_unit_->file_name_ << " ";
      std::cerr << "line " << ctx->start->getLine() << ": ";
      std::cerr << "[ERROR] Redefinition of function " << func_name << "\n";
      exit(1);
    }

    Function *func = res.first->second.get();
    func->def_ctx_ = ctx;
    func->ret_type_ = visit(ctx->type());
    func->name_ = func_name;
    func->args_.resize(ctx->argDecl().size());
    for (auto i = 0; i < ctx->argDecl().size(); ++i) {
      func->args_[i] = visit(ctx->argDecl(i));
      auto res = func->symbol_table_.emplace(func->args_[i].name_,
                                             func->args_[i].type_);
      if (!res.second) {
        std::cerr << "In file " << comp_unit_->file_name_ << " ";
        std::cerr << "line " << ctx->start->getLine() << ": ";
        std::cerr << "[ERROR] Redefinition of the arg" << func->args_[i].name_
                  << "\n";
        exit(1);
      }
    }
    return func;
  }
  antlrcpp::Any visitArgDecl(MiniCParser::ArgDeclContext *ctx) override {
    std::string arg_name = ctx->Ident()->getText();

    std::shared_ptr<Type> type = visit(ctx->type());
    uint32_t arr_dim = ctx->Integer().size();
    uint32_t ptr_dim = type->ptr_arr_lens_.size();

    if (arr_dim) {
      type->is_ptr_ = true;
      type->ptr_arr_lens_.resize(arr_dim + ptr_dim);
      for (int i = 0; i < arr_dim; i++) {
        type->ptr_arr_lens_[arr_dim - i - 1] =
            std::stoi(ctx->Integer()[i]->getText());
      }
      for (int i = 0; i < ptr_dim; i++) {
        type->ptr_arr_lens_[arr_dim + i] = 0;
      }
    }
    return Arg(arg_name, type);
  }
  antlrcpp::Any visitVoidType(MiniCParser::VoidTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "void";
    type->is_void_ = true;
    return type;
  }

  antlrcpp::Any visitCharType(MiniCParser::CharTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "char";
    type->is_int_ = true;
    type->int_is_signed_ = true;
    type->int_length_ = 8;
    return type;
  }
  antlrcpp::Any visitUCharType(MiniCParser::UCharTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "uchar";
    type->is_int_ = true;
    type->int_is_signed_ = false;
    type->int_length_ = 8;
    return type;
  }
  antlrcpp::Any visitIntType(MiniCParser::IntTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "int";
    type->is_int_ = true;
    type->int_is_signed_ = true;
    type->int_length_ = 32;
    return type;
  }

  antlrcpp::Any visitUIntType(MiniCParser::UIntTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "uint";
    type->is_int_ = true;
    type->int_is_signed_ = false;
    type->int_length_ = 32;
    return type;
  }

  antlrcpp::Any visitPointerType(
      MiniCParser::PointerTypeContext *ctx) override {
    std::shared_ptr<Type> type = visit(ctx->type());
    type->is_ptr_ = true;
    type->ptr_arr_lens_.resize(ctx->Multiplication().size(), 0);
    return type;
  }
};