#ifndef NATIVE_ENGINE_H
#define NATIVE_ENGINE_H

#include <v8.h>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <stdlib.h>

#ifdef DEBUG
	#undef DEBUG
	#define DEBUG(...) printf(__VA_ARGS__); fflush(NULL)
#else
	#define DEBUG(...)
#endif

using namespace v8;

namespace ne {

	/**
	 *
	 */
	class NativeException : public std::exception {
	private:
		std::string message;

	public:
		NativeException(std::string message) {
			this->message = message;
		}
		virtual ~NativeException() throw() {}

		virtual const char* what() const throw() {
			return this->message.c_str();
		}
	};

	/**
	 *
	 */
	template <typename CALLBACK>
	class NativeEngine {
	private:
		class CallbackRef {
		public:
			std::string name;
			CALLBACK callback;

			CallbackRef() {};
			CallbackRef(std::string _name, CALLBACK _callback) : name(_name), callback(_callback) {}
		};

		Isolate* isolate;
		Persistent<Context> context;
		std::vector<std::string> scripts;
		std::map<std::string, CallbackRef> callbacks;

		CALLBACK requireCallback;
		std::string requireBasePath;
		std::map<std::string, Handle<Value> > requireResults;

		void setupContext(Persistent<Context> context) {
			HandleScope handle_scope;

			//DEBUG("Add require method\n");
			Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(RequireCall, this->enginePtrToExternal(this));
			context->Global()->Set(String::New("require"), funcTmpl->GetFunction());

			typename std::map<std::string, CallbackRef>::iterator citer;
			for (citer = this->callbacks.begin(); citer != this->callbacks.end(); citer++) {
				std::string name = citer->first;
				//DEBUG("Add %s method\n", name.c_str());
				Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(CallbackCall, this->callbackPtrToExternal(&citer->second));
				context->Global()->Set(String::New(name.c_str()), funcTmpl->GetFunction());
			}

			std::vector<std::string>::iterator siter;
			for (siter = this->scripts.begin(); siter != this->scripts.end(); siter++) {
				this->compileAndRun(*siter);
			}
		}

		Local<Value> compileAndRun(std::string script) {
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

		static Handle<Value> RequireCall(const Arguments& args) {
			if (args.Length() > 0) {
				if (args[0]->IsString()) {
					NativeEngine<CALLBACK>* ne = externalToEnginePtr(args.Data());
					String::Utf8Value utf8(args[0]->ToString());
					std::string module = *utf8;
					std::string path = ne->requireBasePath + module;
					// TODO: normalize path
					// TODO: check if path is already in 'required-modules'
					return executeRequire(ne, module, path);
				}
			}
			return v8::Undefined();
		}

		static Handle<Value> executeRequire(NativeEngine<CALLBACK>* ne, std::string module, std::string path) {
			HandleScope handle_scope;

			char* res = ne->requireCallback(path.c_str());
			std::string code = std::string("var exports = {};\n") + res + "\nexports;";

			std::string save = ne->requireBasePath;
			ne->requireBasePath = path.substr(0, path.find_last_of("/") + 1);

			Persistent<Context> moduleContext = Context::New();
			Context::Scope context_scope(moduleContext);
			Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(RequireCall, ne->enginePtrToExternal(ne));
			moduleContext->Global()->Set(String::New("require"), funcTmpl->GetFunction());
			Local<Value> result = ne->compileAndRun(code);
			moduleContext.Dispose();

			ne->requireBasePath = save;
			return handle_scope.Close(result);
		}

		static Handle<Value> CallbackCall(const Arguments& args) {
			HandleScope handle_scope;

			if (args.Length() > 0) {
				if (args[0]->IsString()) {
					String::Utf8Value utf8(args[0]->ToString());
					std::string str = *utf8;
					CallbackRef *ref = externalToCallbackPtr(args.Data());
					char* res = ref->callback(str.c_str());
					Handle<String> result = String::New(res);
					return handle_scope.Close(result);
				}
			}
			return v8::Undefined();
		}

		Local<External> enginePtrToExternal(NativeEngine<CALLBACK>* ref) {
			return External::New(reinterpret_cast<void *>(ref));
		};

		static NativeEngine<CALLBACK>* externalToEnginePtr(Local<Value> data) {
			if (!data.IsEmpty() && data->IsExternal()) {
				return static_cast<NativeEngine<CALLBACK>*>(External::Cast(*data)->Value());
			}
			return NULL;
		};

		Local<External> callbackPtrToExternal(CallbackRef* ref) {
			return External::New(reinterpret_cast<void *>(ref));
		};

		static CallbackRef* externalToCallbackPtr(Local<Value> data) {
			if (!data.IsEmpty() && data->IsExternal()) {
				return static_cast<CallbackRef*>(External::Cast(*data)->Value());
			}
			return NULL;
		};

		void setUpContext() {
			Isolate::Scope iscope(this->isolate);
			Locker locker(this->isolate);
			HandleScope handle_scope;

			this->context = Context::New();
		};

		void tearDownContext() {
			Isolate::Scope iscope(this->isolate);
			Locker locker(this->isolate);
			HandleScope handle_scope;

			this->context.Dispose();
		};

	public:
		NativeEngine() {
			DEBUG("Constructor called\n");
			this->isolate = Isolate::New();
			this->requireBasePath = "";

			this->setUpContext();
		}
		~NativeEngine() {
			DEBUG("Destructor called\n");
			this->tearDownContext();
			this->isolate->Dispose();
		}

		void setRequireCallback(CALLBACK callback) {
			//DEBUG("Set the common-js require callback resolver\n");
			this->requireCallback = callback;
		}

		void addFunctionCallback(std::string name, CALLBACK callback) {
			this->callbacks[name] = CallbackRef(name, callback);
		}

		void addScript(std::string script) {
			this->scripts.push_back(script);
		}

		std::string execute(std::string input) {
			DEBUG("execute %s\n", input.c_str());
			Isolate::Scope iscope(this->isolate);
			Locker locker(this->isolate);
			HandleScope handle_scope;
			TryCatch try_catch;

			Context::Scope context_scope(this->context);

			//DEBUG("Setup V8 context\n");
			this->setupContext(this->context);
			Local<Value> result = this->compileAndRun(input);

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
	};

}

#endif
