#ifndef JV8_H
#define JV8_H

#include <v8.h>
#include <string>
#include <exception>

using namespace v8;

namespace jv8 {
class NativeEngine {
private:
	Persistent<Context> context;
	Handle<Function> function;

public:
	NativeEngine();
	~NativeEngine();
	void addScript(const char*);
	void prepareRun(const char*);
	const char* execute(const char*);
};

class NativeException : public std::exception {
	std::string message;
public:
	NativeException(std::string _message) {
		message = _message;
	}
	virtual ~NativeException() throw() {
	}

	virtual const char* what() const throw() {
		return message.c_str();
	}
};



class JV8;
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

}
#endif
