#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Container.h"

class Object {
  public:
  Object() {
    _counter++;
  }

  virtual ~Object() {
    _counter--;
  }

  size_t counter() const {
    return _counter;
  }

  virtual std::string toString() const noexcept = 0;

  private:
  inline static size_t _counter = 0;
};

class NamedObject : virtual Object {
  public:
  NamedObject(std::string const &name) : Object(), _name(name) {};

  std::string toString() const noexcept override {
    return _name;
  }

  private:
  std::string _name;
};

class Task : public virtual Object {
  public:
  virtual void execute() = 0;

  bool is_done() const {
    return _is_done;
  }

  void set_id_done() {
    _is_done = true;
  }

  private:
  bool _is_done = false;
};

enum class Operation : char { Add = '+', Subtract = '-', Multiply = '*', Divide = '/' };

template <typename T>
class Binary : public Task, public NamedObject {
  public:
  Binary(Operation op, T first_op, T second_op)
      : NamedObject("Binary"), _op(op), _first_op(first_op), _second_op(second_op), _result(0) {
  }

  std::string toString() const noexcept override {
    std::stringstream message;
    if (is_done()) {
      message << _first_op << static_cast<char>(_op) << _second_op << " = " << _result;
    } else {
      message << "None";
    }
    return message.str();
  }

  void execute() override {
    set_id_done();
    switch (_op) {
      case Operation::Add: {
        _result = _first_op + _second_op;
        break;
      }
      case Operation::Subtract: {
        _result = _first_op - _second_op;
        break;
      }
      case Operation::Divide: {
        if (std::abs(_second_op) > 0) {
          _result = _first_op / _second_op;
        } else {
          throw std::runtime_error("Zero division");
        }
        break;
      }
      case Operation::Multiply: {
        _result = _first_op * _second_op;
        break;
      }
    }
  }

  private:
  Operation _op;

  private:
  T _first_op;
  T _second_op;

  private:
  T _result;
};

template <typename T>
class Add : public Task {
  public:
  Add(std::unique_ptr<T> task, Container<std::unique_ptr<T>> &container)
      : _task(std::move(task)), _container(container) {
  }

  std::string toString() const noexcept override {
    return "added task";
  }

  void execute() override {
    _container.push_back(std::move(_task));
  }

  private:
  Container<std::unique_ptr<T>> &_container;
  std::unique_ptr<T> _task;
};

template <typename T>
class ClearContainer : public Task {
  public:
  ClearContainer(Container<std::unique_ptr<T>> &container) : _container(container) {
  }

  std::string toString() const noexcept override {
    std::stringstream message;
    message << "status: " << (is_done() ? "cleared" : "not cleared");
    return message.str();
  }

  void execute() override {
    if (!is_done()) {
      _container.clear();
    } else {
      std::clog << "Container already is empty!";
    }
    set_id_done();
  }

  private:
  Container<std::unique_ptr<T>> &_container;
};

template <typename T>
class Counter : public Task {
  public:
  Counter(Container<std::unique_ptr<T>> &container) : _counter(0), _container(container) {
    _counter = container.size();
  }

  std::string toString() const noexcept override {
    std::stringstream task_information;
    task_information << "count of tasks: ";
    if (is_done()) {
      task_information << _counter;
    } else {
      task_information << "None";
    }
    return task_information.str();
  }

  std::size_t get_contrainer_size() const {
    return _container.size();
  }

  void execute() override {
    _counter = _container.size();
    set_id_done();
  }

  private:
  size_t _counter;
  Container<std::unique_ptr<T>> &_container;
};

class ObjectCounter : public Task {
  public:
  ObjectCounter() {
  }

  std::string toString() const noexcept override {
    std::stringstream message;
    message << "count of all objects: ";

    if (is_done()) {
      message << _result;
    } else {
      message << "None";
    }

    return message.str();
  }

  void execute() override {
    set_id_done();
    _result = counter();
  }

  private:
  size_t _result = 0;
};

template <typename T>
class ObjectCounterContainer : public Task {
  public:
  ObjectCounterContainer(Container<std::unique_ptr<T>> &container) : _container(container) {
  }

  std::string toString() const noexcept override {
    std::stringstream message;
    message << "count of objects in container: ";

    if (is_done()) {
      message << _result;
    } else {
      message << "None";
    }

    return message.str();
  }

  void execute() override {
    set_id_done();
    _result = _container.size();
  }

  private:
  size_t _result;
  Container<std::unique_ptr<T>> _container;
};