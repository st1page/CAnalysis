#include "SecAnalyzer.h"

#include <fstream>
#include <iostream>

#include "json.hpp"

using Json = nlohmann::json;

void to_json(Json& j, const BadFunction& func) {
  j = Json{{"name", func.name}, {"description", func.desprition}};
}
void from_json(const Json& j, BadFunction& func) {
  j.at("name").get_to(func.name);
  j.at("description").get_to(func.desprition);
}
void to_json(Json& j, const MemAllocFunction& func) {
  j = Json{{"name", func.name},
           {"sizArgPlace", func.siz_arg_place},
           {"numArgPlace", func.num_arg_place}};
}
void from_json(const Json& j, MemAllocFunction& func) {
  j.at("sizArgPlace").get_to(func.siz_arg_place);
  j.at("numArgPlace").get_to(func.num_arg_place);
  j.at("name").get_to(func.name);
}
void to_json(Json& j, const MemUseFunction& func) {
  j = Json{{"name", func.name},
           {"ptrArgPlace", func.ptr_arg_place},
           {"sizArgPlace", func.siz_arg_place}};
}
void from_json(const Json& j, MemUseFunction& func) {
  j.at("ptrArgPlace").get_to(func.ptr_arg_place);
  j.at("sizArgPlace").get_to(func.siz_arg_place);
  j.at("name").get_to(func.name);
}

SecAnalyzer::SecAnalyzer(CompUnit* comp_unit, const char* file_name)
    : comp_unit_(comp_unit),
      MiniCTypeVisitor(),
      func_list_file_name_(file_name) {
  std::ifstream file;
  file.open(func_list_file_name_);
  if (file.fail()) {
    std::cerr << "open file " << std::string(func_list_file_name_)
              << " error\n";
    exit(1);
  }

  Json funcs_j;
  file >> funcs_j;

  int bad_cnt = funcs_j["blacklisk"].size();
  for (int i = 0; i < bad_cnt; i++) {
    BadFunction func = funcs_j["blacklisk"][i];
    bad_funcs_.emplace(func.name, func);

    std::cout << "loading bad_func  : " << func.name << "\n";
  }

  int alloc_cnt = funcs_j["memalloc"].size();
  for (int i = 0; i < alloc_cnt; i++) {
    MemAllocFunction func = funcs_j["memalloc"][i];
    alloc_funcs_.emplace(func.name, func);
    std::cout << "loading alloc_func : " << func.name << "\n";
  }

  int use_cnt = funcs_j["memuse"].size();
  for (int i = 0; i < use_cnt; i++) {
    MemUseFunction func = funcs_j["memuse"][i];
    use_funcs_.emplace(func.name, func);
    std::cout << "loading use_func : " << func.name << "\n";
  }
}
