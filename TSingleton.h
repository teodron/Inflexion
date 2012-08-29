#pragma once

//! Templated singleton.
/*!
	Template wrapper enforcing the singleton behavior.
*/
template <class T>
class TSingleton
{
private: 	
	//! Singleton instance pointer.
	static T* instance;
	//! private constructor.
	TSingleton() { }
	//! private empty copy constructor.
	TSingleton(const TSingleton<T>& sourceObject) {}

public:
	//! Static singleton instance getter.
	static T* GetInstance()
	{
		if (instance == NULL)
			instance = new T();
		return instance;
	}

};

template <class T> T* TSingleton<T>::instance = NULL;