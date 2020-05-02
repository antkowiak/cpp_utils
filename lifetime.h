#pragma once

// C++ Class to track/debug the object "lifetime" of potential containers and
// data structures

#include <iostream>
#include <sstream>
#include <string>

class LifeTime {
private:
  std::string name;
  size_t id;

  static size_t counter;

  static std::string generate_string(const std::string &reason,
                                     const LifeTime &obj,
                                     const LifeTime *const fromPtr = nullptr) {
    std::stringstream ss;
    ss << "[";
    ss.width(17);
    ss << reason << "] [ID: ";
    ss.width(3);
    ss << obj.id << "] [NAME: ";
    ss.width(8);
    ss << std::left << obj.name << std::right << "] [ADDR: 0x" << (&obj) << "]";

    if (fromPtr != nullptr) {
      ss << " [FROM: ";
      ss.width(3);
      ss << fromPtr->id << "]";
    }

    return ss.str();
  }

public:
  LifeTime() {
    name = "";
    id = ++counter;
    std::cout << generate_string("DEFAULT_CONSTRUCT", *this) << std::endl;
  }

  LifeTime(const std::string &n) {
    name = n;
    id = ++counter;
    std::cout << generate_string("CONSTRUCT", *this) << std::endl;
  }

  LifeTime(const LifeTime &rhs) {
    name = rhs.name;
    id = ++counter;
    std::cout << generate_string("COPY_CONSTRUCT", *this, &rhs) << std::endl;
  }

  ~LifeTime() { std::cout << generate_string("DESTRUCT", *this) << std::endl; }

  LifeTime &operator=(const LifeTime &rhs) {
    std::cout << generate_string("ASSIGN", *this, &rhs) << std::endl;
    name = rhs.name;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const LifeTime &rhs) {
    os << generate_string("PRINT", rhs);
    return os;
  }
};

size_t LifeTime::counter = 0;
