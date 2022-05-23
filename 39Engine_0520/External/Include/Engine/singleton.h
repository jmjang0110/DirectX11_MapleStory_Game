#pragma once

template<typename T>
class CSingleton
{
	typedef void(*DESTORY)(void);

private:
	static T* m_Inst;

public:
	static T* GetInst()
	{
		if (nullptr == m_Inst)
		{
			m_Inst = new T;
		}

		return m_Inst;
	}

	static void Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}

public:
	CSingleton()
	{
		atexit((DESTORY)CSingleton<T>::Destroy);
	}

	virtual ~CSingleton()
	{

	}
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;