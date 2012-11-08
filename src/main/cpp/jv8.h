#ifndef JV8_H
#define JV8_H

#include <v8.h>

namespace jv8 {
	class Value;

	class JV8 {
	private:
	public:
		JV8() {};
		~JV8() {};

		Value* eval(const char*);
	};

	class Value {
	};
};

#endif

