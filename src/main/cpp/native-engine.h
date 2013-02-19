#ifndef NATIVE_ENGINE_H
#define NATIVE_ENGINE_H

#include <v8.h>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <stdlib.h>

#define DEBUG(...) printf(__VA_ARGS__); fflush(stdout)
//#define DEBUG(...)

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
			NativeEngine<CALLBACK>* ne;
			std::string name;

			CallbackRef(NativeEngine<CALLBACK>* _ne, std::string _name) : ne(_ne), name(_name) {}

			bool operator<(const CallbackRef& other) const {
				return name < other.name;
			}
		};

		std::vector<std::string> scripts;
		std::map<CallbackRef, CALLBACK> callbacks;

		Local<Value> compile(std::string script) {
			Locker locker;
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

		void setupContext(Persistent<Context> context) {
			Locker locker;
			HandleScope handle_scope;

			std::vector<std::string>::iterator siter;
			for (siter = this->scripts.begin(); siter != this->scripts.end(); siter++) {
				this->compile(*siter);
			}

			typename std::map<CallbackRef, CALLBACK>::iterator citer;
			for (citer = this->callbacks.begin(); citer != this->callbacks.end(); citer++) {
				CallbackRef ref = citer->first;
				Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(CallbackCall, this->classPtrToExternal(&ref));
				context->Global()->Set(String::New(ref.name.c_str()), funcTmpl->GetFunction());
			}
		}

		static Handle<Value> CallbackCall(const Arguments& args) {
			Locker locker;
			HandleScope handle_scope;

			if (args.Length() > 0) {
				if (args[0]->IsString()) {
					String::Utf8Value utf8(args[0]->ToString());
					std::string str = *utf8;
					CallbackRef *ref = externalToClassPtr(args.Data());
					char* res = ref->ne->callbacks[*ref](str.c_str());
					Handle<String> result = String::New(res);
					delete res;
					return handle_scope.Close(result);
				}
			}
			return v8::Undefined();
		}

		Local<External> classPtrToExternal(CallbackRef* ref) {
			Locker locker;
			HandleScope handle_scope;
			return handle_scope.Close(External::New(reinterpret_cast<void *>(ref)));
		};

		static CallbackRef* externalToClassPtr(Local<Value> data) {
			if (!data.IsEmpty() && data->IsExternal()) {
				return static_cast<CallbackRef*>(External::Cast(*data)->Value());
			}
			return NULL;
		};

	public:
		NativeEngine() {}
		~NativeEngine() {}

		void setFunctionCallback(std::string name, CALLBACK callback) {
			this->callbacks[CallbackRef(this, name)] = callback;
		}

		void addScript(std::string script) {
			this->scripts.push_back(script);
		}

		std::string execute(std::string input) {
			//DEBUG("execute %s\n", input.c_str());
			Locker locker;
			HandleScope handle_scope;
			TryCatch try_catch;

			Persistent<Context> context = Context::New();
			Context::Scope context_scope(context);

			this->setupContext(context);
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
	};

}

#endif
