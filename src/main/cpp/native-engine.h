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
		static Persistent<ObjectTemplate> callback_template;

		Persistent<Context> context;
		Persistent<Function> function;
		StringFunctionCallback* callback;

		static Handle<Object> WrapCallback(StringFunctionCallback*);
		static Handle<Value> CallbackCall(const Arguments& args);

	public:
		NativeEngine();
		~NativeEngine();
		void setStringFunctionCallback(StringFunctionCallback*);
		void addScript(std::string);
		void prepareRun(std::string);
		std::string execute(std::string);
	};

	Persistent<ObjectTemplate> NativeEngine::callback_template;

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
