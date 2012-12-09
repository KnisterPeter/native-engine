#include "native-engine.h"
#include <stdlib.h>
#include <exception>

using namespace std;
using namespace v8;
using namespace ne;

NativeEngine::NativeEngine() {
	HandleScope handle_scope;
	this->context = Context::New();
}

NativeEngine::~NativeEngine() {
	this->function.Dispose();
	this->context.Dispose();
}

Local<External> NativeEngine::classPtrToExternal() {
	HandleScope handle_scope;
	return handle_scope.Close(External::New(reinterpret_cast<void *>(this)));
}

NativeEngine* NativeEngine::externalToClassPtr(Local<Value> data) {
	if (!data.IsEmpty() && data->IsExternal()) {
		return static_cast<NativeEngine*>(External::Unwrap(data));
	}
	return NULL;
}

void NativeEngine::setStringFunctionCallback(std::string name, StringFunctionCallback* callback) {
	this->name = name;
	this->callback = callback;
}

void NativeEngine::addScript(std::string script) {
	HandleScope handle_scope;
	TryCatch try_catch;
	Context::Scope context_scope(this->context);

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
	Context::Scope context_scope(this->context);

	Handle<Value> func = this->context->Global()->Get(String::New(name.c_str()));
	if (!func->IsFunction()) {
		throw NativeException("No function '" + name + "' found");
	}
	this->function = Persistent<Function>::New(Handle<Function>::Cast(func));
}

std::string NativeEngine::execute(std::string input) {
	HandleScope handle_scope;
	TryCatch try_catch;
	Context::Scope context_scope(this->context);

	if (this->callback != NULL) {
		Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(CallbackCall, this->classPtrToExternal());
		this->context->Global()->Set(String::New(name.c_str()), funcTmpl->GetFunction());
	}

	Handle<Value> argv[1];
	argv[0] = String::New(input.c_str());
	Handle<Value> result = this->function->Call(this->context->Global(), 1, argv);
	if (result.IsEmpty()) {
		throw NativeException("Script result was empty");
	}
	if (!(result->IsString() || result->IsStringObject())) {
		throw NativeException("Script result was not string");
	}
	String::Utf8Value utf8(result->ToString());
	std::string str = *utf8;
	return str;
}

Handle<Value> NativeEngine::CallbackCall(const Arguments& args) {
	HandleScope handle_scope;

	if (args.Length() > 0) {
		if (args[0]->IsString()) {
			String::Utf8Value utf8(args[0]->ToString());
			std::string str = *utf8;
			NativeEngine *ne = externalToClassPtr(args.Data());
			char* res = ne->callback(str.c_str());
			Handle<String> result = String::New(res);
			delete res;
			return handle_scope.Close(result);
		}
	}
	return v8::Undefined();
}
