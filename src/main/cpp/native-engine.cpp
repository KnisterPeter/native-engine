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

void NativeEngine::setStringFunctionCallback(StringFunctionCallback* _callback) {
	callback = _callback;
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
	if (!func->IsFunction()) {
		throw NativeException("No function '" + name + "' found");
	}
	function = Persistent<Function>::New(Handle<Function>::Cast(func));
}

std::string NativeEngine::execute(std::string input) {
	HandleScope handle_scope;
	TryCatch try_catch;
	Context::Scope context_scope(context);

	Handle<Object> callback_obj = WrapCallback(callback);
	context->Global()->Set(String::New("resolver"), callback_obj);

	Handle<Value> argv[1];
	argv[0] = String::New(input.c_str());
	printf("exec 2\n");
	Handle<Value> result = function->Call(context->Global(), 1, argv);
	printf("exec3\n");
	if (result->IsString()) {
		printf("exec4\n");
		String::Utf8Value utf8(result->ToString());
		printf("exec5\n");
		std::string str = *utf8;
		printf("exec6\n");
		return str;
	}
	printf("exec7\n");
	return NULL;
}

Handle<Object> NativeEngine::WrapCallback(StringFunctionCallback* callback) {
	HandleScope handle_scope;

	if (callback_template.IsEmpty()) {
		Handle<ObjectTemplate> raw_template = ObjectTemplate::New();
		//raw_template->SetInternalFieldCount(1);
		//raw_template->SetNamedPropertyHandler(CallbackGet, CallbackSet);
		callback_template = Persistent<ObjectTemplate>::New(raw_template);
		callback_template->Set(String::New("resolve"), FunctionTemplate::New(CallbackCall));
	}
	Handle<ObjectTemplate> templ = callback_template;
	Handle<Object> resolver = templ->NewInstance();

	//Handle<External> callback_ptr = External::New((void*)callback);
	//resolver->SetInternalField(0, callback_ptr);
	return handle_scope.Close(resolver);
}

Handle<Value> NativeEngine::CallbackCall(const Arguments& args) {
	printf("CallbackCall\n");
	return Handle<Value>();
}
