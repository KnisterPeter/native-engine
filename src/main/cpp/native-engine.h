#ifndef NATIVE_ENGINE_H
#define NATIVE_ENGINE_H

#include <v8.h>
#include <string>
#include <exception>

using namespace v8;

namespace ne {

	typedef char* (StringFunctionCallback)(const char*);

	/**
	 *
	 */
	class NativeEngine {
	private:
		static Persistent<FunctionTemplate> callback_template;

		Persistent<Context> context;
		Persistent<Function> function;
		StringFunctionCallback* callback;

		static Handle<Value> CallbackCall(const Arguments& args);

		Local<External> classPtrToExternal() {
			HandleScope handle_scope;
			return handle_scope.Close(External::New(reinterpret_cast<void *>(this)));
		}

		static NativeEngine* externalToClassPtr(Local<Value> data) {
			if (!data.IsEmpty() && data->IsExternal()) {
				return static_cast<NativeEngine*>(External::Unwrap(data));
			}
			return NULL;
		}

	public:
		NativeEngine();
		~NativeEngine();
		void setStringFunctionCallback(StringFunctionCallback*);
		void addScript(std::string);
		void prepareRun(std::string);
		std::string execute(std::string);
	};

	Persistent<FunctionTemplate> NativeEngine::callback_template;

	/**
	 *
	 */
	class NativeException : public std::exception {
		std::string message;
	public:
		NativeException(std::string _message) {
			message = _message;
		}
		virtual ~NativeException() throw() {
		}

		virtual const char* what() const throw() {
			return message.c_str();
		}
	};

}
#endif
