#pragma once

#include <cstdint>
#include <cstdlib>

class GraphMatrix {
 public:
  GraphMatrix(uint32_t node_num) : node_num_(node_num) {
    node_num = node_num;
    g_ = (bool*)calloc(node_num * node_num, sizeof(bool));
  }
  ~GraphMatrix(){
    free(g_);
  }
  bool ExistEdge(uint32_t x, uint32_t y) {
    return g_[EdgeId(x,y)];
  }
  bool SetEdge(uint32_t x, uint32_t y) {
    bool ret = g_[EdgeId(x,y)];
    g_[EdgeId(x,y)] = true;
    return ret;
  }

  uint32_t EdgeId(uint32_t x, uint32_t y) { return x * node_num_ + y; }
  uint32_t node_num_;
  bool* g_;
};