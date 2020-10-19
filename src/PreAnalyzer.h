#pragma once

#include "CompUnit.h"
#include "MiniCTypeVisitor.h"

class PreAnalyzer : public MiniCTypeVisitor {
 private:
  CompUnit *comp_unit_;
  Function *cur_func_;

  Function *ParseFuncDecl(MiniCParser::FuncContext *ctx, Function *func);

 public:
  PreAnalyzer(CompUnit *comp_unit)
      : comp_unit_(comp_unit), MiniCTypeVisitor() {}
  virtual antlrcpp::Any visitFunc(MiniCParser::FuncContext *ctx) override;
  virtual antlrcpp::Any visitFuncCall(
      MiniCParser::FuncCallContext *ctx) override;

  virtual antlrcpp::Any visitVarDef(MiniCParser::VarDefContext *ctx) override;

  virtual antlrcpp::Any visitReturnStmt(
      MiniCParser::ReturnStmtContext *ctx) override;

  virtual antlrcpp::Any visitSingleExpr(
      MiniCParser::SingleExprContext *ctx) override;
  virtual antlrcpp::Any visitBlock(MiniCParser::BlockContext *ctx) override;

  virtual antlrcpp::Any visitIfStmt(MiniCParser::IfStmtContext *ctx) override;
  virtual antlrcpp::Any visitWhileLoop(
      MiniCParser::WhileLoopContext *ctx) override;
  virtual antlrcpp::Any visitDoWhile(MiniCParser::DoWhileContext *ctx) override;

  virtual antlrcpp::Any visitForLoop(MiniCParser::ForLoopContext *ctx) override;

  virtual antlrcpp::Any visitBreak(MiniCParser::BreakContext *ctx) override;

  virtual antlrcpp::Any visitContinue(
      MiniCParser::ContinueContext *ctx) override;
};