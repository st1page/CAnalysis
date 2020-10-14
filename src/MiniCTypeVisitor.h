#pragma once

#include "CompUnit.h"
#include "MiniCBaseVisitor.h"

class MiniCTypeVisitor : public MiniCBaseVisitor {
 public:
  virtual antlrcpp::Any visitVarDecl(MiniCParser::VarDeclContext *ctx) override {
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
    return Var(arg_name, type);
  }
  virtual antlrcpp::Any visitVoidType(MiniCParser::VoidTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "void";
    type->is_void_ = true;
    return type;
  }

  virtual antlrcpp::Any visitCharType(MiniCParser::CharTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "char";
    type->is_int_ = true;
    type->int_is_signed_ = true;
    type->int_length_ = 8;
    return type;
  }
  virtual antlrcpp::Any visitUCharType(MiniCParser::UCharTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "uchar";
    type->is_int_ = true;
    type->int_is_signed_ = false;
    type->int_length_ = 8;
    return type;
  }
  virtual antlrcpp::Any visitIntType(MiniCParser::IntTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "int";
    type->is_int_ = true;
    type->int_is_signed_ = true;
    type->int_length_ = 32;
    return type;
  }

  virtual antlrcpp::Any visitUIntType(MiniCParser::UIntTypeContext *ctx) override {
    std::shared_ptr<Type> type = std::make_shared<Type>();
    type->name_ = "uint";
    type->is_int_ = true;
    type->int_is_signed_ = false;
    type->int_length_ = 32;
    return type;
  }

  virtual antlrcpp::Any visitPointerType(
      MiniCParser::PointerTypeContext *ctx) override {
    std::shared_ptr<Type> type = visit(ctx->type());
    type->is_ptr_ = true;
    type->ptr_arr_lens_.resize(ctx->Multiplication().size(), 0);
    return type;
  }
};