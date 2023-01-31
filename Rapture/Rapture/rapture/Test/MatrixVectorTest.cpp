
#include <DirectXMath.h>

//#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/DirectX12/GlueMath.h>


using namespace DirectX;
using namespace ATLANTIS_NAMESPACE::Glue;


namespace Test
{

#define PRINT_VECTOR(name,v) (PRINT("%s x:%.2f, y:%.2f, z:%.2f\n",name,v.x,v.y,v.z))

#define CREATE_XMVECTOR_FROM3(name) \
	XMVECTOR xm##name = XMLoadFloat3(&name)\


#define CREATE_AND_LOAD_VECTOR3(name)\
	Vector3 name = {}; \
	CREATE_XMVECTOR_FROM3(name); \

#define CREATE_AND_LOAD_VECTOR3_PARAM(name,x,y,z)\
	Vector3 name = Vector3(x,y,z);\
	CREATE_XMVECTOR_FROM3(name);\

#define STORE_AND_PRINT_VECTOR(name) \
	XMStoreFloat3(&name ,xm##name);\
	PRINT_VECTOR(#name,name);\


	namespace VectorTest
	{
		void VectorTest1()
		{
			Vector3 vec1 = {};
			vec1 = Vector3(3.f, 5.f, 7.f);
			Vector3 vec2 = {};
			Vector3 vec3 = {};
			XMVECTOR xmVec = {};

			xmVec = XMVectorNegate(XMLoadFloat3(&vec1));
			XMStoreFloat3(&vec2, xmVec);

			xmVec = XMVectorSelect(xmVec, xmVec, g_XMSelect1110.v);
			XMStoreFloat3(&vec3, xmVec);

			PRINT("Vector1 x:%.2f, y:%.2f, z:%.2f\n", vec1.x, vec1.y, vec1.z);
			PRINT("Vector2 x:%.2f, y:%.2f, z:%.2f\n", vec2.x, vec2.y, vec2.z);
			PRINT("Vector3 x:%.2f, y:%.2f, z:%.2f\n", vec3.x, vec3.y, vec3.z);
		}
	};

	namespace MatrixTest
	{
		void MatrixTest1()
		{
			Vector3 EyePos = Vector3(10.f, 25.f, 1.f);
			Vector3 FocusPos = Vector3(10.f, 25.f, 5.f);
			Vector3 UpDirection = Vector3(0.f, 1.f, 0.f);


			XMVECTOR xmEyePos = XMLoadFloat3(&EyePos);
			XMVECTOR xmFocusPos = XMLoadFloat3(&FocusPos);
			XMVECTOR xmUpDirection = XMLoadFloat3(&UpDirection);

			XMVECTOR xmEyeDirection = XMVectorSubtract(xmFocusPos, xmEyePos);

			//CREATE_AND_LOAD_VECTOR3_PARAM(EyePos, 10.f, 25.f, 1.f);
			//CREATE_AND_LOAD_VECTOR3_PARAM(EyeDirection, 0.f, 0.f, 1.f);
			//CREATE_AND_LOAD_VECTOR3_PARAM(UpDirection, 0.f, 1.f, 0.f);

			// 視点方向ベクトルの正規化
			//Vector3 R2 = {};
			//XMVECTOR xmR2 = XMLoadFloat3(&EyeDirection);
			//xmR2 = XMVector3Normalize(xmR2);
			//XMStoreFloat3(&R2, xmR2);

			CREATE_AND_LOAD_VECTOR3(R2);
			xmR2 = XMVector3Normalize(xmEyeDirection);


			// 視点方向ベクトルと上方向ベクトルの外積
			//Vector3 R0 = {};
			//CREATE_XMVECTOR_FROM3(R0);
			CREATE_AND_LOAD_VECTOR3(R0);
			xmR0 = XMVector3Cross(xmUpDirection, xmR2);
			xmR0 = XMVector3Normalize(xmR0);
			//XMStoreFloat3(&R0, xmR0);
			//PRINT_VECTOR("R0", R0);
			STORE_AND_PRINT_VECTOR(R0);

			// R2とR0の外積
			CREATE_AND_LOAD_VECTOR3(R1);
			xmR1 = XMVector3Cross(xmR2, xmR0);
			STORE_AND_PRINT_VECTOR(R1);

			//PRINT_VECTOR("R2", R2);
			STORE_AND_PRINT_VECTOR(R2);

			// 視点位置ベクトルの逆ベクトル
			CREATE_AND_LOAD_VECTOR3(NegEyePosition);
			xmNegEyePosition = XMVectorNegate(xmEyePos);
			STORE_AND_PRINT_VECTOR(NegEyePosition);

			// D0,D1,D2のカメラの姿勢ベクトル
			CREATE_AND_LOAD_VECTOR3(D0);
			CREATE_AND_LOAD_VECTOR3(D1);
			CREATE_AND_LOAD_VECTOR3(D2);

			xmD0 = XMVector3Dot(xmR0, xmNegEyePosition);
			xmD1 = XMVector3Dot(xmR1, xmNegEyePosition);
			xmD2 = XMVector3Dot(xmR2, xmNegEyePosition);

			STORE_AND_PRINT_VECTOR(D0);
			STORE_AND_PRINT_VECTOR(D1);
			STORE_AND_PRINT_VECTOR(D2);

			// 行列の最終成分
			CREATE_AND_LOAD_VECTOR3(M0);
			CREATE_AND_LOAD_VECTOR3(M1);
			CREATE_AND_LOAD_VECTOR3(M2);
			CREATE_AND_LOAD_VECTOR3(M3);

			xmM0 = XMVectorSelect(xmD0, xmR0, g_XMSelect1110.v);
			xmM1 = XMVectorSelect(xmD1, xmR1, g_XMSelect1110.v);
			xmM2 = XMVectorSelect(xmD2, xmR2, g_XMSelect1110.v);
			xmM3 = g_XMIdentityR3.v;

			STORE_AND_PRINT_VECTOR(M0);
			STORE_AND_PRINT_VECTOR(M1);
			STORE_AND_PRINT_VECTOR(M2);
			STORE_AND_PRINT_VECTOR(M3);

			PRINT("\n");

			XMMATRIX xmViewMatrix = XMMatrixIdentity();
			xmViewMatrix = XMMatrixLookAtLH(xmEyePos, xmFocusPos, xmUpDirection);
			Matrix44 viewMatrix = {};
			XMStoreFloat4x4(&viewMatrix, xmViewMatrix);
			PRINT("ViewMatrixLookAtLH\n");
			PRINT("r11:%5.2f,", viewMatrix.m[0][0]);
			PRINT("r12:%5.2f,", viewMatrix.m[0][1]);
			PRINT("r13:%5.2f,", viewMatrix.m[0][2]);
			PRINT("r14:%5.2f", viewMatrix.m[0][3]);
			PRINT("\n");
			PRINT("r21:%5.2f,", viewMatrix.m[1][0]);
			PRINT("r22:%5.2f,", viewMatrix.m[1][1]);
			PRINT("r23:%5.2f,", viewMatrix.m[1][2]);
			PRINT("r24:%5.2f", viewMatrix.m[1][3]);
			PRINT("\n");
			PRINT("r31:%5.2f,", viewMatrix.m[2][0]);
			PRINT("r32:%5.2f,", viewMatrix.m[2][1]);
			PRINT("r33:%5.2f,", viewMatrix.m[2][2]);
			PRINT("r34:%5.2f", viewMatrix.m[2][3]);
			PRINT("\n");
			PRINT("r41:%5.2f,", viewMatrix.m[3][0]);
			PRINT("r42:%5.2f,", viewMatrix.m[3][1]);
			PRINT("r43:%5.2f,", viewMatrix.m[3][2]);
			PRINT("r44:%5.2f", viewMatrix.m[3][3]);
			PRINT("\n");


			PRINT("\n");

			xmViewMatrix = XMMatrixIdentity();

			xmViewMatrix.r[0] = xmM0;
			xmViewMatrix.r[1] = xmM1;
			xmViewMatrix.r[2] = xmM2;
			xmViewMatrix.r[3] = xmM3;

			xmViewMatrix = XMMatrixTranspose(xmViewMatrix);


			XMStoreFloat4x4(&viewMatrix, xmViewMatrix);
			PRINT("VectorTest\n");
			PRINT("r11:%5.2f,", viewMatrix.m[0][0]);
			PRINT("r12:%5.2f,", viewMatrix.m[0][1]);
			PRINT("r13:%5.2f,", viewMatrix.m[0][2]);
			PRINT("r14:%5.2f", viewMatrix.m[0][3]);
			PRINT("\n");
			PRINT("r21:%5.2f,", viewMatrix.m[1][0]);
			PRINT("r22:%5.2f,", viewMatrix.m[1][1]);
			PRINT("r23:%5.2f,", viewMatrix.m[1][2]);
			PRINT("r24:%5.2f", viewMatrix.m[1][3]);
			PRINT("\n");
			PRINT("r31:%5.2f,", viewMatrix.m[2][0]);
			PRINT("r32:%5.2f,", viewMatrix.m[2][1]);
			PRINT("r33:%5.2f,", viewMatrix.m[2][2]);
			PRINT("r34:%5.2f", viewMatrix.m[2][3]);
			PRINT("\n");
			PRINT("r41:%5.2f,", viewMatrix.m[3][0]);
			PRINT("r42:%5.2f,", viewMatrix.m[3][1]);
			PRINT("r43:%5.2f,", viewMatrix.m[3][2]);
			PRINT("r44:%5.2f", viewMatrix.m[3][3]);
			PRINT("\n");
		}
	};


	void MatrixVectorTestMain()
	{
		PRINT("VectorTest1\n");
		VectorTest::VectorTest1();
		PRINT("\n");

		PRINT("XMVector XMMatrixLookToLH Test\n");
		MatrixTest::MatrixTest1();
		PRINT("\n");
		
	};

	

#undef PRINT_VECTOR

#undef CREATE_XMVECTOR_FROM3

#undef CREATE_AND_LOAD_VECTOR3

#undef STORE_AND_PRINT_VECTOR

};