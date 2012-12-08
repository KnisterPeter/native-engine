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
		Persistent<Context> context;
		Persistent<Function> function;
		std::string name;
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
		void setStringFunctionCallback(std::string, StringFunctionCallback*);
		void addScript(std::string);
		void prepareRun(std::string);
		std::string execute(std::string);
	};

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
