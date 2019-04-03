#include<iostream>
#include<string>
//#include<vector>
using namespace std;
// �Զ���һ�������Ŀռ�������
template<typename T>
struct myallocator   //ʹ��struct����Ĭ��Ϊȫ���г�Ա����
{
	// �ڴ濪��
	T* allocate(size_t size)
	{
		return (T*)malloc(size * sizeof(T));
	}

	// �ڴ��ͷ�
	void deallocate(T *ptr,size_t size)
	{
		free(ptr);
	}

	// ������ 1.new 2.new const 3.��λnew
	void construct(T *ptr, const T &obj)
	{
		new (ptr)T(obj); // ��λnew  ��ָ���ڴ�λ�ÿ��ٿռ�
	}

	// ��������
	void destroy(T *ptr)
	{
		ptr->~T();
	}
};

template<typename T, typename allocator = myallocator<T>>
class Vector
{
public:
	// ���캯�������Դ����Զ��ԵĿռ���������������Ĭ�ϵ�allocator
	Vector(int size = 0, const allocator &alloc = allocator())
		:mcur(0), msize(size), mallocator(alloc)
	{
		// ֻ���������ײ�ռ䣬�������κζ���
		mpvec = mallocator.allocate(msize);
	}
	// ��������
	~Vector()
	{
		// �����������еĶ���
		for (int i = 0; i < mcur; ++i)
		{
			mallocator.destroy(mpvec + i);
		}
		// �ͷ�����ռ�õĶ��ڴ�
		mallocator.deallocate(mpvec, msize);
		mpvec = nullptr;
	}
	// �������캯��
	Vector(const Vector<T> &src)
		:mcur(src.mcur)
		, msize(src.msize)
		, mallocator(src.mallocator)
	{
		// ֻ���������ײ�ռ䣬�������κζ���
		mpvec = mallocator.allocate(msize);
		for (int i = 0; i < mcur; ++i)
		{
			// ��ָ���ĵ�ַmpvec+i�Ϲ���һ��ֵΪsrc.mpvec[i]�Ķ���
			mallocator.construct(mpvec + i, src.mpvec[i]);
		}
	}
	// ��ֵ���غ���
	Vector<T> operator=(const Vector<T> &src)
	{
		if (this == &src)
			return *this;

		// �����������еĶ���
		for (int i = 0; i < mcur; ++i)
		{
			mallocator.destroy(mpvec + i);
		}
		// �ͷ�����ռ�õĶ��ڴ�
		mallocator.deallocate(mpvec, msize);

		mcur = src.mcur;
		msize = src.msize;
		// ֻ���������ײ�ռ䣬�������κζ���
		mpvec = mallocator.allocate(msize);
		for (int i = 0; i < mcur; ++i)
		{
			// ��ָ���ĵ�ַmpvec+i�Ϲ���һ��ֵΪsrc.mpvec[i]�Ķ���
			mallocator.construct(mpvec + i, src.mpvec[i]);
		}
		return *this;
	}
	// β���������ݺ���
	void push_back(const T &val)
	{
		if (mcur == msize)
			resize();
		mallocator.construct(mpvec + mcur, val);
		mcur++;
	}
	// β��ɾ�����ݺ���
	void pop_back()
	{
		if (mcur == 0)return;
		// ������ɾ���Ķ���
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
	T *mpvec; // ��̬���飬����������Ԫ��
	int mcur; // ���浱ǰ��ЧԪ�صĸ���
	int msize; // �����������ݺ���ܳ���
	allocator mallocator; // ���������Ŀռ�����������

	// ����2�����ݺ���
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
			// �����������еĶ���
			for (int i = 0; i < msize; ++i)
			{
				mallocator.destroy(mpvec + i);
			}
			// �ͷ�����ռ�õĶ��ڴ�
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
	// C++ STL ���֮һ������
	Test t1, t2, t3;
	cout << "------------" << endl;
	Vector<Test> vec;
	vec.reserve(10); // ֻ��Ԥ���ռ�
	cout << "------------" << endl;
	vec.push_back(t1);
	vec.push_back(std::move(t2));
	vec.push_back(t3);
	cout << "------------" << endl;
	vec.pop_back();
	cout << "------------" << endl;
}