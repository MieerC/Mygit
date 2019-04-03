#include<iostream>
#include<string>
//#include<vector>
using namespace std;
// 自定义一个容器的空间配置器
template<typename T>
struct myallocator   //使用struct建立默认为全公有成员的类
{
	// 内存开辟
	T* allocate(size_t size)
	{
		return (T*)malloc(size * sizeof(T));
	}

	// 内存释放
	void deallocate(T *ptr,size_t size)
	{
		free(ptr);
	}

	// 对象构造 1.new 2.new const 3.定位new
	void construct(T *ptr, const T &obj)
	{
		new (ptr)T(obj); // 定位new  在指定内存位置开辟空间
	}

	// 对象析构
	void destroy(T *ptr)
	{
		ptr->~T();
	}
};

template<typename T, typename allocator = myallocator<T>>
class Vector
{
public:
	// 构造函数，可以传入自定以的空间配置器，否则用默认的allocator
	Vector(int size = 0, const allocator &alloc = allocator())
		:mcur(0), msize(size), mallocator(alloc)
	{
		// 只开辟容器底层空间，不构造任何对象
		mpvec = mallocator.allocate(msize);
	}
	// 析构函数
	~Vector()
	{
		// 先析构容器中的对象
		for (int i = 0; i < mcur; ++i)
		{
			mallocator.destroy(mpvec + i);
		}
		// 释放容器占用的堆内存
		mallocator.deallocate(mpvec, msize);
		mpvec = nullptr;
	}
	// 拷贝构造函数
	Vector(const Vector<T> &src)
		:mcur(src.mcur)
		, msize(src.msize)
		, mallocator(src.mallocator)
	{
		// 只开辟容器底层空间，不构造任何对象
		mpvec = mallocator.allocate(msize);
		for (int i = 0; i < mcur; ++i)
		{
			// 在指定的地址mpvec+i上构造一个值为src.mpvec[i]的对象
			mallocator.construct(mpvec + i, src.mpvec[i]);
		}
	}
	// 赋值重载函数
	Vector<T> operator=(const Vector<T> &src)
	{
		if (this == &src)
			return *this;

		// 先析构容器中的对象
		for (int i = 0; i < mcur; ++i)
		{
			mallocator.destroy(mpvec + i);
		}
		// 释放容器占用的堆内存
		mallocator.deallocate(mpvec, msize);

		mcur = src.mcur;
		msize = src.msize;
		// 只开辟容器底层空间，不构造任何对象
		mpvec = mallocator.allocate(msize);
		for (int i = 0; i < mcur; ++i)
		{
			// 在指定的地址mpvec+i上构造一个值为src.mpvec[i]的对象
			mallocator.construct(mpvec + i, src.mpvec[i]);
		}
		return *this;
	}
	// 尾部插入数据函数
	void push_back(const T &val)
	{
		if (mcur == msize)
			resize();
		mallocator.construct(mpvec + mcur, val);
		mcur++;
	}
	// 尾部删除数据函数
	void pop_back()
	{
		if (mcur == 0)return;
		// 析构被删除的对象
		mcur--;
		mallocator.destroy(mpvec + mcur);
		
	}
	void reserve(int size)
	{
		if (size <= msize)
			return;
		T *ptmp = mallocator.allocate(2 * msize);
		for (int i = 0; i < msize; ++i)
		{
			mallocator.construct(ptmp + i, mpvec[i]);
		}
		mallocator.deallocate(mpvec, msize);
		mpvec = ptmp;
		msize = size;
	}
private:
	T *mpvec; // 动态数组，保存容器的元素
	int mcur; // 保存当前有效元素的个数
	int msize; // 保存容器扩容后的总长度
	allocator mallocator; // 定义容器的空间配置器对象

	// 容器2倍扩容函数
	void resize()
	{
		if (msize == 0)
		{
			mpvec = mallocator.allocate(sizeof(T));
			mcur = 0;
			msize = 1;
		}
		else
		{
			T *ptmp = mallocator.allocate(2 * msize);
			for (int i = 0; i < msize; ++i)
			{
				mallocator.construct(ptmp + i, mpvec[i]);
			}
			// 先析构容器中的对象
			for (int i = 0; i < msize; ++i)
			{
				mallocator.destroy(mpvec + i);
			}
			// 释放容器占用的堆内存
			mallocator.deallocate(mpvec, msize);
			mpvec = ptmp;
			msize *= 2;
		}
	}
};
class Test
{
public:
	Test() :mptr(new int[10000])
	{
		cout << "Test()" << endl;
	}
	~Test()
	{
		delete[]mptr;
		cout << "~Test()" << endl;
	}
	Test(const Test& src)
	{
		cout << "Test(const Test&)" << endl;
		mptr = new int[10000];
		memcpy(mptr, src.mptr, 10000 * 4);
	}
	void operator=(const Test& src)
	{
		cout << "operator=" << endl;
		if (this == &src)
			return;
		delete []mptr;
		mptr = new int[10000];
		memcpy(mptr, src.mptr, 10000 * 4);
	}
private:
	int *mptr;
};
int main()
{
	// C++ STL 组件之一：容器
	Test t1, t2, t3;
	cout << "------------" << endl;
	Vector<Test> vec;
	vec.reserve(10); // 只想预留空间
	cout << "------------" << endl;
	vec.push_back(t1);
	vec.push_back(std::move(t2));
	vec.push_back(t3);
	cout << "------------" << endl;
	vec.pop_back();
	cout << "------------" << endl;
}