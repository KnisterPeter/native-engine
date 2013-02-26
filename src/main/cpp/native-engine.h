#ifndef NATIVE_ENGINE_H
#define NATIVE_ENGINE_H

#include <v8.h>
#include <string>
#include <sstream>
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
	 * Utility function for splitting a string
	 */
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	/**
	 * Utility function for splitting a string
	 */
	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		return split(s, delim, elems);
	}

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
			return message.c_str();
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

		class ModuleContext {
		private:
			NativeEngine<CALLBACK>* ne;
			std::string id;
			Persistent<Context> context;

		public:
			ModuleContext(NativeEngine<CALLBACK>* ne, std::string id) {
				DEBUG("ModuleContext('%s')\n", id.c_str());

				this->ne = ne;
				this->id = id;
				this->context = Context::New();

				HandleScope handleScope;
				Context::Scope context_scope(context);

				Local<Object> data = Object::New();
				data->Set(String::New("parentModuleId"), String::New(id.c_str()));
				data->Set(String::New("engine"), ne->enginePtrToExternal());
				Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(RequireCall, data);
				context->Global()->Set(String::New("require"), funcTmpl->GetFunction());
			}
			~ModuleContext() {
				DEBUG("~ModuleContext('%s')\n", id.c_str());
				context.Dispose();
			}

			Handle<Value> execute() {
				HandleScope handleScope;
				Context::Scope context_scope(context);

				Local<String> exportsName = String::New("exports");
				if (context->Global()->Has(exportsName)) {
					return handleScope.Close(context->Global()->Get(exportsName));
				}

				context->Global()->Set(exportsName, Object::New());
DEBUG("ModuleContext#execute() - require '%s'\n", id.c_str());
				// TODO: Handle exceptions
				char* script = ne->requireCallback(id.c_str());
				ne->compileAndRun(id, script);
				return handleScope.Close(context->Global()->Get(exportsName));
			}
		};

		Isolate* isolate;
		Persistent<Context> context;
		std::vector<CallbackRef*> callbacks;

		CALLBACK requireCallback;
		std::map<std::string, ModuleContext*> modules;

		Local<Value> compileAndRun(std::string name, std::string script) {
			HandleScope handleScope;
			TryCatch tryCatch;

			Handle<String> source = String::New(script.c_str());
			Handle<Script> compiled = Script::Compile(source, String::New(name.c_str()));
			if (compiled.IsEmpty()) {
				String::Utf8Value exception(tryCatch.Exception());
				throw NativeException(*exception);
			}

			Local<Value> result = compiled->Run();
			if (result.IsEmpty()) {
				String::Utf8Value exception(tryCatch.Exception());
				throw NativeException(*exception);
			}

			return handleScope.Close(result);
		}

		std::string parseModulePath(std::string parentId, std::string id) {
			DEBUG("parseModulePath('%s', '%s')\n", parentId.c_str(), id.c_str());
			std::string moduleId;
			size_t idx = parentId.find_last_of("/");
			if (idx != std::string::npos) {
				moduleId = parentId.substr(0, idx + 1);
			} else {
				moduleId = "";
			}
			moduleId += id;
			std::vector<std::string> parts = split(moduleId, '/');
			for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); it++) {
				std::string part = *it;
				if (part.compare(".") == 0) {
					parts.erase(it);
				} else if (part.compare("..") == 0) {
					parts.erase(it);
					parts.erase(it - 1);
				}
			}
			moduleId = "";
			for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); it++) {
				moduleId += "/" + *it;
			}
			return moduleId.substr(1);
		}

		static Handle<Value> RequireCall(const Arguments& args) {
			if (args.Length() > 0) {
				if (args[0]->IsString()) {
					HandleScope handleScope;

					Local<Object> data = Object::Cast(*(args.Data()));
					NativeEngine<CALLBACK>* ne = externalToEnginePtr(data->Get(String::New("engine")));
					String::Utf8Value utf8(args[0]->ToString());
					std::string moduleId = *utf8;
					String::Utf8Value parentModuleId(data->Get(String::New("parentModuleId")));
					moduleId = ne->parseModulePath(*parentModuleId, moduleId);

					ModuleContext* moduleContext;
					typename std::map<std::string, ModuleContext*>::iterator it = ne->modules.find(moduleId);
					if (it != ne->modules.end()) {
						moduleContext = it->second;
					} else {
						moduleContext = new ModuleContext(ne, moduleId);
						ne->modules[moduleId] = moduleContext;
					}
					return moduleContext->execute();
				}
			}
			return v8::Undefined();
		}

		static Handle<Value> CallbackCall(const Arguments& args) {
			if (args.Length() > 0) {
				if (args[0]->IsString()) {
					HandleScope handleScope;

					String::Utf8Value utf8(args[0]->ToString());
					std::string str = *utf8;
					CallbackRef *ref = externalToCallbackPtr(args.Data());
					char* res = ref->callback(str.c_str());
					Handle<String> result = String::New(res);
					return handleScope.Close(result);
				}
			}
			return v8::Undefined();
		}

		Local<External> enginePtrToExternal() {
			return External::New(reinterpret_cast<void *>(this));
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
			Locker locker(isolate);
			Isolate::Scope iscope(isolate);
			HandleScope handleScope;

			context = Context::New();
			Context::Scope context_scope(context);

			Local<Object> data = Object::New();
			data->Set(String::New("parentModuleId"), String::New(""));
			data->Set(String::New("engine"), enginePtrToExternal());
			Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(RequireCall, data);
			context->Global()->Set(String::New("require"), funcTmpl->GetFunction());
		};

		void tearDownContext() {
			Locker locker(isolate);
			Isolate::Scope iscope(isolate);
			HandleScope handleScope;
			Context::Scope context_scope(context);

			for (typename std::vector<CallbackRef*>::iterator it = callbacks.begin();
					it != callbacks.end();
					it++) {
				delete *it;
			}

			for (typename std::map<std::string, ModuleContext*>::iterator it = modules.begin();
					it != modules.end();
					it++) {
				delete it->second;
			}

			context.Dispose();
		};

	public:
		NativeEngine() {
			DEBUG("NativeEngine()\n");
			isolate = Isolate::New();

			setUpContext();
		}
		~NativeEngine() {
			DEBUG("~NativeEngine()\n");
			tearDownContext();
			isolate->Dispose();
		}

		void setRequireCallback(CALLBACK callback) {
			//DEBUG("Set the common-js require callback resolver\n");
			requireCallback = callback;
		}

		void addFunctionCallback(std::string name, CALLBACK callback) {
			DEBUG("addFunctionCallback('%s')\n", name.c_str());

			Locker locker(isolate);
			Isolate::Scope iscope(isolate);
			HandleScope handleScope;
			Context::Scope context_scope(context);

			CallbackRef* ref = new CallbackRef(name, callback);
			callbacks.push_back(ref);

			Local<FunctionTemplate> funcTmpl = FunctionTemplate::New(CallbackCall, callbackPtrToExternal(ref));
			context->Global()->Set(String::New(name.c_str()), funcTmpl->GetFunction());
		}

		void addScript(std::string name, std::string script) {
			DEBUG("addScript('%s')\n", name.c_str());

			Locker locker(isolate);
			Isolate::Scope iscope(isolate);
			HandleScope handleScope;
			Context::Scope context_scope(context);

			compileAndRun(name, script);
		}

		std::string execute(std::string input) {
			DEBUG("execute %s\n", input.c_str());

			Locker locker(isolate);
			Isolate::Scope iscope(isolate);
			HandleScope handleScope;
			Context::Scope context_scope(context);

			TryCatch tryCatch;
			Local<Value> result = compileAndRun("execute", input);
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
