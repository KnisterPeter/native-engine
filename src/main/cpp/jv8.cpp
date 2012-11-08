#include "jv8.h"

using namespace v8;

jv8::Value* jv8::JV8::eval(const char* script) {
	HandleScope handle_scope;
	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);
	Handle<String> source = String::New(script);
	Handle<Script> compiled = Script::Compile(source);
	Handle<Value> result = compiled->Run();
	context.Dispose();
	return NULL;
}

