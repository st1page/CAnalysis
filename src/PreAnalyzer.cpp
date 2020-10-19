#include "PreAnalyzer.h"

#include "CompUnit.h"
Function *PreAnalyzer::ParseFuncDecl(MiniCParser::FuncContext *ctx,
                                     Function *func) {
  // TODO(st1page): check if the args is same when the func has been decl.
  func->def_ctx_ = ctx;
  func->ret_type_ = visit(ctx->type());
  func->args_.resize(ctx->varDecl().size());
  for (auto i = 0; i < ctx->varDecl().size(); ++i) {
    func->args_[i] = visit(ctx->varDecl(i));
    auto res =
        func->symbol_table_.emplace(func->args_[i].name_, func->args_[i].type_);
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

antlrcpp::Any PreAnalyzer::visitFunc(MiniCParser::FuncContext *ctx) {
  std::string func_name = ctx->Ident()->getText();
  Function *func = comp_unit_->AddFunc(func_name);

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
antlrcpp::Any PreAnalyzer::visitFuncCall(MiniCParser::FuncCallContext *ctx) {
  std::string func_name = ctx->Ident()->getText();
  auto it = comp_unit_->func_table_.find(func_name);
  if (it == comp_unit_->func_table_.end()) {
    std::cerr << "In file " << comp_unit_->file_name_ << " ";
    std::cerr << "line " << ctx->start->getLine() << ": ";
    std::cerr << "[ERROR] undeclared function " << func_name << "\n";
    exit(1);
  }
  cur_func_->call_funcs_.push_back(it->second.get());
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitVarDef(MiniCParser::VarDefContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::VarDef, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitReturnStmt(
    MiniCParser::ReturnStmtContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::Return, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitSingleExpr(
    MiniCParser::SingleExprContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::SingleExpr, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitBlock(MiniCParser::BlockContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::BlockStart, ctx);
  antlrcpp::Any ret = visitChildren(ctx);
  cur_func_->stmts_.emplace_back(Stmt::Cate::BlockEnd, ctx);
  return ret;
}

antlrcpp::Any PreAnalyzer::visitIfStmt(MiniCParser::IfStmtContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::If, ctx);
  if (ctx->Else()) {
    visit(ctx->stmt(0));
    cur_func_->stmts_.emplace_back(Stmt::Cate::Else, ctx);
    return visit(ctx->stmt(1));
  } else {
    return visit(ctx->stmt(0));
  }
}

antlrcpp::Any PreAnalyzer::visitWhileLoop(MiniCParser::WhileLoopContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::While, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitDoWhile(MiniCParser::DoWhileContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::DoWhile, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitForLoop(MiniCParser::ForLoopContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::For, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitBreak(MiniCParser::BreakContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::Break, ctx);
  return visitChildren(ctx);
}

antlrcpp::Any PreAnalyzer::visitContinue(MiniCParser::ContinueContext *ctx) {
  cur_func_->stmts_.emplace_back(Stmt::Cate::Continue, ctx);
  return visitChildren(ctx);
}
