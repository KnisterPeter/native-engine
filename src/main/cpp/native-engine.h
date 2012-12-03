#ifndef NATIVE_ENGINE_H
#define NATIVE_ENGINE_H

#include <v8.h>
#include <string>
#include <exception>

using namespace v8;

namespace ne {

	typedef char* (ResolverCallback)(const char*);

	/**
	 *
	 */
	class NativeEngine {
	private:
		Persistent<Context> context;
		Persistent<Function> function;
		ResolverCallback* callback;

	public:
		NativeEngine();
		~NativeEngine();
		void setResolverCallback(ResolverCallback*);
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
