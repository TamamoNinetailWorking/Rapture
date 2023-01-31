#include <functional>

namespace Test
{

	class CTestClassBase
	{
	public:

		virtual void Call();

		CTestClassBase() {};
		~CTestClassBase() {};
	};

	class CTestClassSub01 : public CTestClassBase
	{
	public:

		void Call() override;

		CTestClassSub01() {};
		~CTestClassSub01() {};
	};

	void BindFunctionTest()
	{
		CTestClassSub01	subObject = {};

		using FFunc = std::function<void()>;

		FFunc func = std::bind(&CTestClassBase::Call, &subObject);

		func();
	}


	void CTestClassBase::Call()
	{
		PRINT("CTestClassBase Call.\n");
	}

	void CTestClassSub01::Call()
	{
		PRINT("Sub01 Call.\n");
	}

};