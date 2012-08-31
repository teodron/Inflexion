#pragma once
#include <memory>

//! Templated singleton.
/*!
	Template wrapper enforcing the singleton behavior.
*/
template <class T>
class TSingleton
{
private: 	
	//! Singleton instance pointer.
	//static T* instance;
	//! The instance wrapped in a smart pointer: destroys the object automatically

	static std::shared_ptr<T> wrappedInstance;

	//! private constructor.
	TSingleton() { }
	//! private empty copy constructor.
	TSingleton(const TSingleton<T>& sourceObject) {}
	//! assignment operator
	TSingleton<T>& operator=(TSingleton<T>& )
	{

	}

public:
	//! Static singleton instance getter.
	static T* GetInstance()
	{
		if (wrappedInstance.get() == NULL)
			wrappedInstance.reset(new T());
		return wrappedInstance.get();
			/*
		if (instance == NULL)
			instance = new T();
		return instance;
		*/
	}

};

//template <class T> T* TSingleton<T>::instance = NULL;
template <class T> std::shared_ptr<T> TSingleton<T>::wrappedInstance = NULL;