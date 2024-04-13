#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

/**
 * Implemented as AVL tree
 */

#include <functional>
#include <cstddef>
#include <cassert>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
  class Key,
  class T,
  class Compare = std::less<Key>
> class map {
public:
  typedef pair<const Key, T> value_type;
  class iterator;
  using const_iterator = iterator;
  class node {
  public:
    value_type* data;
    node* left;
    node* right;
    node* parent;
    int height;

    node() = delete;
    node(const Key &k, const T &t, node *l = nullptr, node *r = nullptr, node *p = nullptr, int h = 1)
        : left(l), right(r), parent(p), height(h) {
      data = new value_type(k, t);
    }
    node(const value_type &v, node *l = nullptr, node *r = nullptr, node *p = nullptr, int h = 1)
        : left(l), right(r), parent(p), height(h) {
      data = new value_type(v);
    }
    ~node() {
      if(data) {
        data->first.~Key();
        data->second.~T();
        delete data;  
      }
    }
  };

private:
  size_t siz;
  node* root;
  void destroy(node* t) {
    if(!t) return;
    destroy(t->left);
    destroy(t->right);
    delete t;
    return;
  }
  void copy(node* other, node* &new_node, node* parent) {
    if(!other) { new_node = nullptr; return; }
    new_node = new node(*(other->data), nullptr, nullptr, parent, other->height);
    ++siz;
    copy(other->left, new_node->left, new_node);
    copy(other->right, new_node->right, new_node);
    return;
  }
  /*     D
   *    / \
   *   B   E
   *  / \
   * A   C
   */
  void LL(node* &t) {
    node* tmp = t->left; // B
    tmp->parent = t->parent; // B->p
    t->parent = tmp; // D->p
    t->left = tmp->right; // D->l
    if(t->left) { t->left->parent = t; } // C->p
    tmp->right = t; // B->r
    t = tmp;

    t->right->height = measure(t->right); // D->h
    t->height = measure(t); // B->h
  }
  void RR(node* &t) {
    node* tmp = t->right;
    tmp->parent = t->parent;
    t->parent = tmp;
    t->right = tmp->left;
    if(t->right) { t->right->parent = t; }
    tmp->left = t;
    t = tmp;

    t->left->height = measure(t->left);
    t->height = measure(t);
  }
  void LR(node* &t) {
    RR(t->left);
    LL(t);
  }
  void RL(node* &t) {
    LL(t->right);
    RR(t);
  }

  node* fin(const Key &k, node* t) const {
    if(!t) return nullptr;
    if(Compare()(k, t->data->first)) {
      return fin(k, t->left);
    }
    else if(Compare()(t->data->first, k)) {
      return fin(k, t->right);
    }
    else {
      return t;
    }
  }

  int hei(const node* t) const {
    if(!t) return 0;
    return t->height;
  }
  int measure(const node* t) const {
    if(!t) return 0;
    int lh = hei(t->left);
    int rh = hei(t->right);
    if(lh > rh) return lh + 1;
    else return rh + 1;
  }

  pair<node*, bool> ins(const value_type &val, node* &t, node* p) {
    if(!t) { // first node
      t = new node(val, nullptr, nullptr, p, 1);
      return {t, true};
    }
    if(!Compare()(t->data->first, val.first) && !Compare()(val.first, t->data->first)) {
      return {t, false};
    }
    pair<node*, bool> ret;
    if(Compare()(t->data->first, val.first)) { // right
      pair<node*, bool> tmp = ins(val, t->right, t);
      ret.first = tmp.first;
      ret.second = tmp.second;
      if(hei(t->right) - hei(t->left) >= 2) {
        if(Compare()(t->right->data->first, val.first)) {
          RR(t);
        }
        else {
          RL(t);
        }
      }
    }
    else { //left
      pair<node*, bool> tmp = ins(val, t->left, t);
      ret.first = tmp.first;
      ret.second = tmp.second;
      if(hei(t->left) - hei(t->right) >= 2) {
        if(Compare()(val.first, t->left->data->first)) {
          LL(t);
        }
        else {
          LR(t);
        }
      }
    }
    t->height = measure(t);
    return ret;
  }

  node* next(node* t) {
    // if(!t) return nullptr;
    node* ret = t;
    if(t->right) { // have right child, find leftest node on right branch
      ret = t->right;
      while(ret->left) ret = ret->left;
      return ret;
    }
    while(ret->parent && ret->parent->right == ret) { // find first right ancestor
      ret = ret->parent;
    }
    if(!ret->parent) return nullptr; // largest already
    return ret->parent;
  }
  node* prev(node* t) {
    node* ret = t;
    if(t->left) {
      ret = t->left;
      while(ret->right) ret = ret->right;
      return ret;
    }
    while(ret->parent && ret->parent->left == ret) {
      ret = ret->parent;
    }
    if(!ret->parent) return nullptr;
    return ret->parent;
  }
  node* first(node* t) {
    if(!t) return t;
    while(t->left) t = t->left;
    return t; 
  }
  node* last(node* t) { // note that last(root) is not end()
    if(!t) return t;
    while(t->right) t = t->right;
    return t;
  }

public:
  map() {
    siz = 0;
    root = nullptr;
  }
  map(const map &other) {
    copy(other.root, root, nullptr);
    //assert(other.siz == siz);
  }
  map & operator=(const map &other) {
    if(this == &other) return *this;
    destroy(root);
    copy(other.root, root, nullptr);
    return *this;
  }
  ~map() {
    destroy(root);
  }
  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent to key.
   * If no such element exists, an exception of type `index_out_of_bound'
   */
  T & at(const Key &key) {
    node* res = fin(key, root);
    if(!res) {
      throw index_out_of_bound();
    }
    else return res->data->second;
  }
  const T & at(const Key &key) const {
    node* res = fin(key, root);
    if(!res) {
      throw index_out_of_bound();
    }
    else return res->data->second;
  }
  /**
   * TODO
   * access specified element 
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T & operator[](const Key &key) {
    T tmp_t;
    value_type tmp_v(key, tmp_t);
    pair<node*, bool> res = ins(tmp_v, root, nullptr);
    if(res.second) ++siz;
    return res.first->data->second;
  }
  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T & operator[](const Key &key) const {
    node* res = fin(key, root);
    if(!res) {
      throw index_out_of_bound();
    }
    else return res->data->second;
  }

  iterator begin() {
    return iterator(this, first(root));
  }
  // const_iterator cbegin() const {}
  const_iterator cbegin() {
    return const_iterator(this, first(root));
  }

  iterator end() {
    return iterator(this, nullptr);
  }
  const_iterator cend() {
    return const_iterator(this, nullptr);
  }
  bool empty() const {
    return siz == 0;
  }
  size_t size() const {
    return siz;
  }
  void clear() {
    destroy(root);
    root = nullptr;
    siz = 0;
  }
  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the insertion), 
   *   the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    pair<node*, bool> res = ins(value, root, nullptr);
    iterator i(this, res.first);
    if(res.second) {
      ++siz;
      return {i, true};
    }
    else {
      return {i, false};
    }
  }
  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
   */
  void erase(iterator pos) {
    /////////////////////////////////////////
  }

  size_t count(const Key &key) const {
    node* res = fin(key, root);
    if(res) return 1;
    else return 0;
  }
  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is returned.
   */
  iterator find(const Key &key) {
    node* res = fin(key, root);
    return iterator(this, res);
  }
  // const_iterator find(const Key &key) const {}

  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  //class const_iterator;
  class iterator {
    friend class map;
  private:
    map* src;
    node* obj;
    // obj == nullptr indicates it is end()
  public:
    iterator() {
      src = nullptr;
      obj = nullptr;
    }
    iterator(map* src, node* obj) : src(src), obj(obj) {}
    iterator(const iterator &other) {
      src = other.src;
      obj = other.obj;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      if(!obj || !src) throw invalid_iterator();
      iterator ret(*this);
      obj = src->next(obj);
      return ret;
    }
    /**
     * TODO ++iter
     */
    iterator & operator++() {
      if(!obj || !src) throw invalid_iterator();
      obj = src->next(obj);
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      if(!src) throw invalid_iterator();
      iterator ret(*this);
      if(!obj) obj = src->last(src->root);
      else obj = src->prev(obj);
      if(!obj) throw invalid_iterator();
      return ret;
    }
    /**
     * TODO --iter
     */
    iterator & operator--() {
      if(!src) throw invalid_iterator();
      if(!obj) obj = src->last(src->root);
      else obj = src->prev(obj);
      if(!obj) throw invalid_iterator();
      return *this;
    }

    value_type & operator*() const {
      if(!obj) throw invalid_iterator();
      return *(obj->data);
    }
    bool operator==(const iterator &rhs) const {
      return src == rhs.src && obj == rhs.obj;
    }
    //bool operator==(const const_iterator &rhs) const {}
    bool operator!=(const iterator &rhs) const {
      return src != rhs.src || obj != rhs.obj;
    }
    //bool operator!=(const const_iterator &rhs) const {}

    value_type* operator->() const noexcept {
      if(!obj) throw invalid_iterator();
      return obj->data;
    }
  };

  /*
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
    private:
      // data members.
    public:
      const_iterator() {
        // TODO
      }
      const_iterator(const const_iterator &other) {
        // TODO
      }
      const_iterator(const iterator &other) {
        // TODO
      }
      // And other methods in iterator.
      // And other methods in iterator.
      // And other methods in iterator.
  };

  */
};

}

#endif
