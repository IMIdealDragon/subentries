#include<iostream>
using namespace std;
#include<map>
 
template<typename T>
class mshared_ptr
{
public:
	explicit mshared_ptr(T *ptr = NULL);		//构造方法
	~mshared_ptr();		//析构方法
	mshared_ptr(const mshared_ptr<T> &src);		//拷贝构造
	mshared_ptr& operator = (const mshared_ptr<T> &src);		//赋值运算符重载
	T& operator*();		//解引用运算符重载
	T* operator->();	//成员运算符重载
    int count();
private:
	T *_ptr;
    
	static map<T*, int> _map;		//静态数据成员需要在类外进行初始化
};
 
template<typename T>
map<T*, int> mshared_ptr<T>::_map;
 
template<typename T>
mshared_ptr<T>::mshared_ptr(T *ptr)		//构造方法
{
    cout << "mshared_ptr的构造方法正被调用！" << endl;
	_ptr = ptr;
	_map.insert(make_pair(_ptr, 1));
}
 
template<typename T>
mshared_ptr<T>::~mshared_ptr()		//析构方法
{
    cout << "mshared_ptr的析构方法正被调用！" << endl;
	if (--_map[_ptr] <= 0 && NULL != _ptr)
	{
		delete _ptr;
		_ptr = NULL;
		_map.erase(_ptr);
	}
}
 
template<typename T>
mshared_ptr<T>::mshared_ptr(const mshared_ptr<T> &src)	//拷贝构造
{
    cout << "mshared_ptr的拷贝构造方法正被调用！" << endl;
	_ptr = src._ptr;
	_map[_ptr]++;
}
 
template<typename T>
mshared_ptr<T>& mshared_ptr<T>::operator=(const mshared_ptr<T> &src)		//赋值运算符重载
{
    cout << "mshared_ptr的拷贝赋值方法正被调用！" << endl;
	if (_ptr == src._ptr)
	{
		return *this;
	}
 
	if (--_map[_ptr] <= 0 && NULL != _ptr)
	{
		delete _ptr;
		_ptr = NULL;
		_map.erase(_ptr);
	}
 
	_ptr = src._ptr;
	_map[_ptr]++;
	return *this;
}
 
template<typename T>
T& mshared_ptr<T>::operator*()		//解引用运算符重载
{
	return *_ptr;
}
 
template<typename T>
T* mshared_ptr<T>::operator->()		//成员运算符重载
{
	return _ptr;
}

template<typename T>
int mshared_ptr<T>::count()		//成员运算符重载
{
	return _map[_ptr];
}
