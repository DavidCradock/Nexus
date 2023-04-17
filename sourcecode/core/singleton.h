#pragma once
#pragma once
#include "../precompiled_header.h"

// Usage:
// Create a class from this template class with something like...
// class MyClass : public Singleton<MyClass>
// From then on, whenever you create an object of MyClass like normal, it only has one instance, all variables are shared.
// To create a pointer to the singleton...
// MyClass *pMyClass = MyClass::getPointer();
// Then you can access all the public stuff with somethine like...
// pMyClass->someMethod();
template <class T>
class Singleton
{
public:
	virtual ~Singleton()
	{
		if (pInstance)
		{
			delete pInstance;
			pInstance = 0;
		}
	}

	// Obtain pointer to class object
	// If not yet created, the object will be created here and it's constructor called 
	static T* getPointer(void)
	{
		if (!pInstance)
		{
			pInstance = new T();
		}
		return pInstance;
	}

protected:
	static T* pInstance;
};

template <class T> T* Singleton<T>::pInstance = 0;

