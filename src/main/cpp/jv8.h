#ifndef JV8_H
#define JV8_H

#include <v8.h>

using namespace v8;

namespace jv8 {
class JV8Value;

class JV8 {
public:
	JV8Value* eval(const char*);
};

class JV8Value {
private:
	Persistent<Value> value;
	char* stringbuf;

public:
	JV8Value(Handle<Value>);
	~JV8Value();
	bool isString();
	char* toString();
};
}

#endif
