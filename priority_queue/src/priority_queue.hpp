#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
  class node {
  public:
    node* left;
    node* right;
    T* content;
    int depth;
    node() = delete;
    node(const T &con, int d = 0) : depth(d) {
      content = new T(con);
      left = nullptr;
      right = nullptr;
    }
    ~node() {
      delete content;
    }
  };

  int siz;
  node* root;
  //static Compare cmp;

public:
  priority_queue() {
    siz = 0;
    root = nullptr;
  }

  priority_queue(const priority_queue &other) {
    root = copy(other.root);
    siz = other.siz;
  }

  ~priority_queue() {
    destroy(root);
  }

  priority_queue &operator=(const priority_queue &other) {
    if(this == &other) return *this;
    destroy(root);
    root = copy(other.root);
    siz = other.siz;
    return *this;
  }
  /**
   * get the top of the queue.
   * @return a reference of the top element.
   * throw container_is_empty if empty() returns true;
   */
  const T & top() const {
    if(empty()) throw container_is_empty();
    return *(root->content);
  }

  void push(const T &e) {
    node *add = new node(e);
    try {
      root = fuse(root, add);
    }
    catch(...) { // Avoid memleak lest the comparison failed
      delete add;
      throw;
    }
    ++siz;
  }

  void pop() {
    if(empty()) throw container_is_empty();
    --siz;
    node* temp = root;
    root = fuse(root->left, root->right);
    delete temp;
  }

  size_t size() const {
    return siz;
  }

  bool empty() const {
    if(root) return false;
    return true;
  }
  /**
   * merge two priority_queues with at most O(logn) complexity.
   * clear the other priority_queue.
   */
  void merge(priority_queue &other) {
    root = fuse(root, other.root);
    siz += other.siz;
    other.siz = 0;
    other.root = nullptr;
  }

private:
  int dep(node* p) const {
    if(p) return p->depth;
    return -1;
  }
  void exchange(node* &x, node* &y) {
    node* temp = x;
    x = y;
    y = temp;
  }
  node* copy(node* other) {
    if(!other) return nullptr;
    node* cur = new node(*(other->content), other->depth);
    cur->left = copy(other->left);
    cur->right = copy(other->right);
    return cur;
  }
  void destroy(node* p) {
    if(!p) return;
    destroy(p->left);
    destroy(p->right);
    delete p;
    return;
  }
  /**
   * fuse invalidate original relations.
   */
  node* fuse(node* x, node* y) {
    if(!x) return y;
    if(!y) return x;
    if(Compare()(*(x->content), *(y->content))) {
      exchange(x, y);
    }
    // x is the new root
    x->right = fuse(x->right, y);
    if(dep(x->left) < dep(x->right)) {
      exchange(x->left, x->right);
    }
    x->depth = dep(x->right) + 1;
    return x;
  }
};

}

#endif
