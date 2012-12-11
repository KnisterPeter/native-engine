#include "native-engine.h"
#include <stdlib.h>
#include <exception>

using namespace v8;
using namespace ne;

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

Local<Value> NativeEngine::compile(std::string script) {
	HandleScope handle_scope;
	TryCatch try_catch;

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

	return handle_scope.Close(result);
}

std::string NativeEngine::execute(std::string input) {
	HandleScope handle_scope;
	TryCatch try_catch;

	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);

	std::vector<std::string>::const_iterator i = scripts.begin();
	std::vector<std::string>::const_iterator n = scripts.end();
	for (; i != n; ++i) {
		this->compile(*i);
	}

	if (this->callback != NULL) {
		Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(CallbackCall, this->classPtrToExternal());
		context->Global()->Set(String::New(name.c_str()), funcTmpl->GetFunction());
	}

	Local<Value> result = this->compile(input);
	context.Dispose();
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

