#include "jv8.h"
#include <stdlib.h>

using namespace v8;
using namespace jv8;

JV8Value* JV8::eval(const char* script) {
	//printf("[NATIVE] script: %s\n\n", script);
	HandleScope handle_scope;
	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);
	Handle<String> source = String::New(script);
	Handle<Script> compiled = Script::Compile(source);
	Local<Value> result = compiled->Run();
	JV8Value* value = new JV8Value(result);
	context.Dispose();
	printf("[NATIVE] str: %s\n", value->toString());
	return value;
}

JV8Value::JV8Value(Handle<Value> _value) {
	value = Persistent<Value>::New(_value);
	stringbuf = NULL;
}

JV8Value::~JV8Value() {
	value.Dispose();
	if (stringbuf != NULL) {
		delete stringbuf;
	}
}

bool JV8Value::isString() {
	return value->IsString();
}

char* JV8Value::toString() {
	if (stringbuf == NULL) {
		Local<String> str = value->ToString();
		int len = str->Utf8Length();
		stringbuf = new char[len];
		str->WriteUtf8(stringbuf);
	}
	return stringbuf;
}
