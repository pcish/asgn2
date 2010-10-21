#ifndef ENGINE_ERROR_H
#define ENGINE_ERROR_H

#include <exception>
using namespace std;

namespace Shipping {

class Error : public std::exception {
  public:
    Error(string errorMessage) : std::exception() {
        errorMessage_ = errorMessage;
    }
    ~Error() throw() {}
    virtual const char* what() const throw() {
        return errorMessage_.c_str();
    }
  private:
    string errorMessage_;
};

class ValueError : public Error {
  public:
    ValueError(string errorMessage) : Error(errorMessage) {
    }
    ~ValueError() throw() {}
};

}

#endif
