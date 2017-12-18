#include "Test.hpp"

Test::Test(void) {
  std::cout << "Hi i am default constructor" << std::endl;
}

Test::Test(std::string name) : _name(name) {
  std::cout << "Constructor says your name is " << this->_name << std::endl;
}

Test::Test(Test const & rhs) {
  *this = rhs;
  std::cout << "Hi i am copy constructor" << std::endl;
}

Test::~Test(void) {
  std::cout << "Destructor called" << std::endl;
}

std::string  Test::getName() const{
  return this->_name;
}

/* Non member */

std::ostream & operator<<(std::ostream & o, Test const & fixed) {
  o << "Surcharge : " << fixed.getName() << std::endl;
  return o;
}