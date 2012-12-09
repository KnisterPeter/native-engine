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
		Local<External> classPtrToExternal();
		static NativeEngine* externalToClassPtr(Local<Value>);

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

}
#endif
