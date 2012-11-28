#include "jv8.h"
#include <stdlib.h>
#include <exception>

using namespace std;
using namespace v8;
using namespace jv8;

NativeEngine::NativeEngine() {
	HandleScope handle_scope;
	context = Context::New();
}

NativeEngine::~NativeEngine() {
	context.Dispose();
}

void NativeEngine::addScript(const char* script) {
	TryCatch try_catch;

	HandleScope handle_scope;
	Context::Scope context_scope(context);

	Handle<String> source = String::New(script);
	Handle<Script> compiled = Script::Compile(source);
	if (compiled.IsEmpty()) {
		String::Utf8Value exception(try_catch.Exception());
		throw NativeException(*exception);
	}

	Local<Value> result = compiled->Run();
	if (result.IsEmpty()) {
		String::Utf8Value exception(try_catch.Exception());
		throw NativeException(*exception);
	}
}

void NativeEngine::prepareRun(const char* name) {
	HandleScope handle_scope;
	Context::Scope context_scope(context);

	Handle<String> funcName = String::New(name);
	Handle<Value> func = context->Global()->Get(funcName);
	function = handle_scope.Close(Handle<Function>::Cast(func));
}

const char* NativeEngine::execute(const char* input) {
	TryCatch try_catch;

	HandleScope handle_scope;
	Context::Scope context_scope(context);

	Handle<Value> argv[1];
	argv[0] = String::New(input);
	Handle<Value> result = function->Call(context->Global(), 1, argv);
	// Convert result to string
	return "";
}


JV8Value* JV8::eval(const char* script) {
	//printf("[NATIVE] script: %s\n\n", script);
	HandleScope handle_scope;
	TryCatch try_catch;
	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);
	Handle<String> source = String::New(script);
	Handle<Script> compiled = Script::Compile(source);
	if (compiled.IsEmpty()) {
		String::Utf8Value exception(try_catch.Exception());
		throw NativeException(*exception);
	}
	Local<Value> result = compiled->Run();
	if (result.IsEmpty()) {
		String::Utf8Value exception(try_catch.Exception());
		throw NativeException(*exception);
	}
	JV8Value* value = new JV8Value(result);
	context.Dispose();
	//printf("[NATIVE] str: %s\n", value->toString());
	return value;
}

void JV8::put(const char* key, void* obj) {
	// TODO: Impelement
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
	//printf("[NATIVE] destruct jv8value");
}

void JV8Value::dispose() {
	delete this;
}

bool JV8Value::isString() {
	return value->IsString();
}

char* JV8Value::getString() {
	if (stringbuf == NULL) {
		Local<String> str = value->ToString();
		int len = str->Utf8Length();
		stringbuf = new char[len];
		str->WriteUtf8(stringbuf);
	}
	return stringbuf;
}
