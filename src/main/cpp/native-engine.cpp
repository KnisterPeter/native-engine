#include "native-engine.h"
#include <stdlib.h>
#include <exception>

using namespace std;
using namespace v8;
using namespace ne;

NativeEngine::NativeEngine() {
	HandleScope handle_scope;
	context = Context::New();
}

NativeEngine::~NativeEngine() {
	function.Dispose();
	context.Dispose();
}

void NativeEngine::addScript(std::string script) {
	HandleScope handle_scope;
	TryCatch try_catch;
	Context::Scope context_scope(context);

	Handle<String> source = String::New(script.c_str());
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

void NativeEngine::prepareRun(std::string name) {
	HandleScope handle_scope;
	Context::Scope context_scope(context);

	Handle<String> funcName = String::New(name.c_str());
	Handle<Value> func = context->Global()->Get(funcName);
	function = Persistent<Function>::New(Handle<Function>::Cast(func));
}

std::string NativeEngine::execute(std::string input) {
	HandleScope handle_scope;
	TryCatch try_catch;
	Context::Scope context_scope(context);

	Handle<Value> argv[1];
	argv[0] = String::New(input.c_str());
	Handle<Value> result = function->Call(context->Global(), 1, argv);
	if (result->IsString()) {
		String::Utf8Value utf8(result->ToString());
		std::string str = *utf8;
		return str;
	}
	return NULL;
}
