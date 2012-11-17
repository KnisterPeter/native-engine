#ifndef JV8_H
#define JV8_H

#include <v8.h>
#include <string>
#include <exception>

using namespace v8;

namespace jv8 {
class JV8Exception;
class JV8Value;

class JV8 {
public:
	JV8Value* eval(const char*);
	void put(const char*, void*);
};

class JV8Value {
private:
	Persistent<Value> value;
	char* stringbuf;

public:
	JV8Value(Handle<Value>);
	~JV8Value();
	void dispose();
	bool isString();
	char* getString();
};

class JV8Exception : public std::exception {
	std::string message;
public:
	JV8Exception(std::string _message) {
		message = _message;
	}
	virtual ~JV8Exception() throw() {
	}

	virtual const char* what() const throw() {
		return message.c_str();
	}
};

}
#endif
