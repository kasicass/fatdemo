#include "FatFramework/FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

#if defined(FAT_OS_WINDOWS)
// for COMPolicy test
#include <d3d9.h>
#endif

using namespace Fat;

TEST_DECLARE(TestSmartPtrAssertPolicy)
{
	class MyType
	{
	public:
		void Function() {}
		void ConstFunction() const {}
	};

	typedef TSmartPtr<MyType, TSmartPtrAssertPolicy> MyTypePtr;
	typedef TSmartPtr<const MyType, TSmartPtrAssertPolicy> MyTypeConstPtr;

	// Comparison operators
	{
		MyType oObject;

		MyTypePtr pObject1;
		MyTypeConstPtr pConstObject1(&oObject);
		MyTypePtr pObject2(&oObject);

		FatTest(pObject1 == MyTypePtr());
		FatTest(pObject1 == pObject1);
		FatTest(pConstObject1 == pObject2);
		FatTest(pConstObject1 != pObject1);
		FatTest(pConstObject1.GetUnsafe() == &oObject);
		FatTest(pObject1.GetUnsafe() != &oObject);
	}

	// Dereferencing
	{
		// Unassigned pointer
		MyTypePtr pObject;

		// Dereferencing should assert on unassigned pointers
		FatTestAssert(pObject->Function());
		FatTestAssert(*pObject);
		FatTestAssert(pObject.Get());

		// Dereferencing should succeed on assigned pointers
		MyType oObject;
		pObject = &oObject;
		pObject->Function();
		pObject->ConstFunction();

		// Even on const pointer
		MyTypeConstPtr pConstObject(&oObject);
		pConstObject->ConstFunction();
	}
}

TEST_DECLARE(TestSmartPtrIntrusivePolicy)
{
	class MyType : public IntrusiveCounter {};

	typedef TSmartPtr<MyType, TSmartPtrIntrusivePolicy> MyTypePtr;

	MyTypePtr pObject1;
	MyTypePtr pObject2 = FatNew(MyType);

	FatTest(pObject2->GetRefCount() == 1);

	pObject1 = pObject2;
	FatTest(pObject2->GetRefCount() == 2);

	pObject1 = NULL;
	FatTest(pObject2->GetRefCount() == 1);

	{
		MyTypePtr pObject3(pObject2);
		FatTest(pObject2->GetRefCount() == 2);
	}
	FatTest(pObject2->GetRefCount() == 1);

	pObject2 = NULL;
}

TEST_DECLARE(TestSmartPtrCOMPolicy)
{
#if defined(FAT_OS_WINDOWS)

	typedef TSmartPtr<IUnknown, TSmartPtrCOMPolicy> IUnknownPtr;

	// refCount = 1
	IUnknown* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// refCount = 2
	IUnknownPtr pObject1;
	IUnknownPtr pObject2(pD3D);

	// refCount = 3
	pObject1 = pObject2;

	// refCount = 2
	pObject1 = NULL;

	{
		IUnknownPtr pObject3(pObject2);
	}

	// refCount = 1
	pObject2 = NULL;

	FatTest(pD3D->Release() == 0);

#endif
}

#endif
