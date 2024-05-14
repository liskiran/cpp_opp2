#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename Type>
class Container {
  public:
  struct Node {
    Type _value;
    Node *_prev = nullptr;
    Node *_next = nullptr;

    explicit Node() : _value(0), _prev(nullptr), _next(nullptr) {
    }

    explicit Node(Type value) : _value(value), _prev(nullptr), _next(nullptr) {
    }
    explicit Node(Type &&value) : _value(std::move(value)), _prev(nullptr), _next(nullptr) {
    }

    explicit Node(const Type &value, Node *prev, Node *next)
        : _value(value), _prev(prev), _next(next) {
    }
    explicit Node(Type &&value, Node *prev, Node *next)
        : _value(std::move(value)), _prev(prev), _next(next) {
    }
  };

  public:
  explicit Container() : _head(nullptr), _tail(nullptr), _size(0), _is_cleared(false) {
  }

  ~Container() {
    clear();
  }

  struct Iterator {
private:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Type;
    using pointer = Node *;
    using reference = Node &;

public:
    Iterator(pointer ptr) : m_ptr(ptr) {
    }
    Type &operator*() const {
      return m_ptr->_value;
    }
    Type &operator->() {
      return *m_ptr->_value;
    }
    Iterator &operator++() {
      m_ptr = m_ptr->_next;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.m_ptr == b.m_ptr;
    }
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.m_ptr != b.m_ptr;
    }

private:
    pointer m_ptr;
  };

  public:
  Iterator begin() noexcept {
    return Iterator(_head);
  }
  Iterator end() noexcept {
    return Iterator(_tail->_next);
  }

  public:
  const Iterator begin() const {
    return Iterator(_head);
  }

  const Iterator end() const {
    Iterator(_tail->_next);
  }

  public:
  void push_front(Type value) {

    Node *new_front = new Node(std::move(value), nullptr, _head);
    if (_size == 0) {
      _head = new_front;
      _tail = _head;
    } else {
      _head->_prev = new_front;
      _head = new_front;
    }
    _size++;
  }

  void push_back(Type value) {
    Node *new_back = new Node(std::move(value), _tail, nullptr);
    if (_size == 0) {
      _tail = _head = new_back;
    } else {
      _tail->_next = new_back;
      _tail = new_back;
    }
    _size++;
  }

  public:
  void pop_front() {
    if (_size == 0) {
      throw std::runtime_error("method: remove_front, error: empty container");
    }
    if (_size == 1) {
      _head = _tail = nullptr;
    }
    if (_size > 1) {
      Node *remove_front = _head;
      _head = _head->_next;
      delete remove_front;
    }

    _size--;
  }

  Type &pop_back() {
    if (_size == 0) {
      throw std::runtime_error("method: remove_front, error: empty container");
    }
    auto &pop_value = back();
    if (_size == 1) {
      _head = _tail = nullptr;
    }

    if (_size > 1) {
      Node *remove_back = _tail;
      _tail = _tail->_prev;
      delete remove_back;
    }

    _size--;
    return pop_value;
  }

  public:
  Type &front() const {
    if (_size == 0) {
      throw std::runtime_error("method: front, error: empty container");
    }
    return _head->_value;
  }

  Type &back() const {
    if (_size == 0) {
      throw std::runtime_error("method: back, error: empty container!");
    }
    return _tail->_value;
  }

  public:
  size_t size() const {
    return _size;
  }  // O(1) WORK

  bool empty() {
    return size() == 0;
  }

  public:
  void swap(Container &another) {
    if (this->_head == nullptr || another._head == nullptr) {
      throw std::runtime_error("method: swap, error: one of the container is empty");
    }
    std::swap(this->_head, another._head);
    std::swap(this->_tail, another._tail);
    std::swap(_size, another._size);
  }

  void reverse() {
    if (_head == nullptr || _tail == nullptr) {
      throw std::runtime_error("method: reverse, error: container is empty");
    }

    Node *from_begin = _head;
    Node *from_end = _tail;

    for (size_t iter = 0; from_begin != from_end && iter < _size / 2;
         from_begin = from_begin->_next, from_end = from_end->_prev) {
      std::swap(from_begin->_value, from_end->_value);
    }
  }

  bool is_cleared() const {
    return _is_cleared;
  }

  void clear() {
    _is_cleared = true;

    if (_is_cleared || _head == nullptr || _tail == nullptr || empty()) {

      return;
    }

    for (Node *p = _head; p != _tail->_next;) {
      Node *current_node = p;
      p = p->_next;
      delete current_node;
    }

    _head = _tail = nullptr;
    _size = 0;
  }

  void print() {
    if (_head == nullptr) {
      std::clog << "Nothing print, container is empty\n";
      return;
    }

    std::clog << "Values:\n";

    for (Node *p = _head; p != _tail->_next; p = p->_next) {
      std::clog << "reference: " << p << ", value: " << p->_value << "\n";
    }

    std::clog << '\n';
  }

  private:
  Node *_head;
  Node *_tail;

  private:
  size_t _size;
  bool _is_cleared;
};
