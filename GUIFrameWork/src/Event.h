#pragma once
#include <vector>
#include <tuple>






using std::vector;
template<typename ret, typename... types>
class Event
{
	vector<void*> objs;
	vector<ret(*)(void*, types...)> functions;
	vector<ret(*)(types...)> functionsStatic;
public:
	Event() {}
	~Event() {}

	void Add(void* obj, ret(*function)(void*, types...)){
		objs.push_back(obj);
		functions.push_back(function);
	}

	void Add(ret(*function)(types...)) {
		functionsStatic.push_back(function);
	}
	void Remove(ret(*function)(void*, types...)){
		for (size_t i = 0; i < functions.size(); i++)
		{
			if(functions[i] == function){
				functions.swap(i, functions.size() - 1);
				functions.pop_back();
				objs.swap(i, objs.size() - 1);
				objs.pop_back();
			}
		}
	}

	void Remove(ret(*function)(types...)) {
		for (size_t i = 0; i < functionsStatic.size(); i++)
		{
			if (functionsStatic[i] == function) {
				functionsStatic.swap(i, functionsStatic.size() - 1);
				functionsStatic.pop_back();
			}
		}
	}

	void Remove(void* obj) {
		for (size_t i = 0; i < objs.size(); i++)
		{
			if (objs[i] == obj) {
				functions.swap(i, functions.size() - 1);
				functions.pop_back();
				objs.swap(i, objs.size() - 1);
				objs.pop_back();
			}
		}
	}


	void Call(types... t) {
		operator()(t...);
	}
	
	void operator()(types... t) {
		for (size_t i = 0; i < functions.size(); i++)
		{
			functions[i](objs[i],t...);
		}
		for (size_t i = 0; i < functionsStatic.size(); i++)
		{
			functionsStatic[i](t...);
		}
	}
};

