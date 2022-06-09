#pragma once

//A singleton manager class which any manager classes will inherit from

template<typename T>

class Manager
{
protected:
	//Constructor and Destructor
	Manager();
	Manager(Manager& a_manager) = delete;
	Manager& operator=(const Manager& a_manager) = delete;
	virtual ~Manager();
	void SetInstance(T* a_instance);
private:
	static T* m_pInstance;
public:
	static T* CreateInstance();
	static T* GetInstance();
	static void DestroyInstance();
};

//definition of m_pInstance set to nullptr
template <typename T> T* Manager<T>::m_pInstance = nullptr;

template <typename T> T* Manager<T>::CreateInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new T();
	}
	return m_pInstance;
}

template <typename T> T* Manager<T>::GetInstance()
{
	return m_pInstance;
}

template <typename T> void Manager<T>::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

template <typename T> Manager<T>::Manager() {}
template <typename T> Manager<T>::~Manager() {}