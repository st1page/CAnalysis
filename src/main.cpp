#include <cstdlib>
#include <iostream>

#include "CodeDiffer.h"
#include "CompUnit.h"
#include "cli.hpp"

using namespace std;

int g_argc;
const char **g_argv;
int cmd_diff(cli::Parser &parser) {
  bool token_based =
      parser.flag("tokenbased")
          .alias("token")
          .description("if is based on token diff, or use graph diff")
          .getValue();

  parser.getRemainingArguments(g_argc, g_argv);
  if (g_argc != 3) {
    cout << "please input diff <file1> <file2>" << endl;
    return EXIT_FAILURE;
  }
  CompUnit *unit1 = new CompUnit((char *)g_argv[1]);
  CompUnit *unit2 = new CompUnit((char *)g_argv[2]);
  unit1->PreAnalyze();
  unit2->PreAnalyze();
  CodeDiffer differ;
  if (token_based) {
    differ.UnitLCSDiff(unit1, unit2);
  } else {
    differ.UnitCallGraphDiff(unit1, unit2);
  }
}
int cmd_audit(cli::Parser &parser) {
  const char *func_list_file =
      parser.option("flist")
          .alias("l")
          .description("the json file of the functions list")
          .required()
          .getValue();
  parser.getRemainingArguments(g_argc, g_argv);
  if (g_argc != 2) {
    cout << "please input diff <file>" << endl;
    return EXIT_FAILURE;
  }
  CompUnit *unit1 = new CompUnit((char *)g_argv[1]);
  unit1->SecAnalyze(func_list_file);

}
void cmd_register(cli::Parser &parser) {
  parser.help() <<
      R"(CAnalysis a C language static analysis tool
    Usage: CAnalysis <command> [args] ...)";

  parser.command("diff")
      .description(
          R"(
    Description: plagiarism detection of files
    Usage:       diff <file1> <file2>)")
      .execute(cmd_diff);
  parser.flag("tokenbased")
      .alias("token")
      .description("if is based on token diff, or use graph diff");

  parser.command("audit")
      .description(
          R"(
    Description: static analysis security problems
    Usage:       audit [args] fileNname
    Usage:       audit [args] DirectoryNames...)")
      .execute(cmd_audit);
  parser.option("flist").alias("l").description(
      "the json file of the functions list");
}
int main(int argc, const char **argv) {
  cli::Parser parser(argc, argv);
  g_argc = argc;
  g_argv = argv;
  cmd_register(parser);
  // in case -output is missing
  if (parser.hasErrors()) return EXIT_FAILURE;
  int res;
  if (!parser.executeCommand(res)) return EXIT_FAILURE;

  // in case the command is missing
  //    if (!parser.executeCommand())
  //        return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
