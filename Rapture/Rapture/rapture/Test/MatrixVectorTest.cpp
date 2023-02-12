
#include <DirectXMath.h>

//#include <Atlantis/DirectX12/DirectXPaste.h>
#include <Atlantis/DirectX12/GlueMath.h>


using namespace DirectX;
using namespace ATLANTIS_NAMESPACE::Glue;


namespace Test
{

#define PRINT_VECTOR(name,v) (PRINT("%s x:%.2f, y:%.2f, z:%.2f\n",name,v.x,v.y,v.z))
#define PRINT_FLOAT4(v) (PRINT("x:%.2f, y:%.2f, z:%.2f, w:%.2f\n",v[0],v[1],v[2],v[3]))

#define PRINT_MATRIX44(matrix) \
	for(uint32 i = 0; i < 4; ++i) \
	{\
		PRINT_FLOAT4(matrix.m[i]);\
	}\

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


		void ViewMatrixTest()
		{
			FMatrix xmViewMatrix = {};

			// ビュー行列の作成
			float eyeHeight = 16.5f;
			Vector3 eyePos(0.f, eyeHeight, -8.f);
			Vector3 focusPos(0.f, eyeHeight, 0.f);
			Vector3 upDir(0.f, 1.f, 0.f);

			FVector eyePosVec = XMLoadFloat3(&eyePos);
			FVector focusPosVec = XMLoadFloat3(&focusPos);
			FVector upDirVec = XMLoadFloat3(&upDir);

			xmViewMatrix = XMMatrixLookAtLH(eyePosVec, focusPosVec, upDirVec);

			Matrix44 viewMatrix = {};
			XMStoreFloat4x4(&viewMatrix, xmViewMatrix);

			PRINT("ViewMatrix XMMatrixLookAtLH\n");
			//PRINT_FLOAT4(viewMatrix.m[0]);
			PRINT_MATRIX44(viewMatrix);

			Vector3 restEyePos = {};
			restEyePos.x = -XMVectorGetX(XMVector3Dot(xmViewMatrix.r[3], xmViewMatrix.r[0]));
			restEyePos.y = -XMVectorGetX(XMVector3Dot(xmViewMatrix.r[3], xmViewMatrix.r[1]));
			restEyePos.z = -XMVectorGetX(XMVector3Dot(xmViewMatrix.r[3], xmViewMatrix.r[2]));

			PRINT_VECTOR("RestEyePos", restEyePos);

			// 座標をマイナスかけて反転させて、スケールは1固定
			// 回転行列部分を転置すれば通常のTransformの取り扱いからViewが生成できるか？

			// 視点位置と視線ベクトルさえあれば行列は作れそう
			// >> 奥行きZ方向の回転0と考えれば、残りの回転角度はY軸とX軸との内積で回転度合いを求めることができるから、3D空間上の姿勢を求めることができるはず
			// 軸に対して内積を取ると当然、その軸との角度差を求めることになるので・・・
			// >>上記の案だと今の条件では90度だということしかわからない・・・


			// 視線ベクトルをY軸と外積を取る（=X軸回転ベクトル）
			// X軸回転ベクトルと視線ベクトルの外積を取る（=Y軸回転ベクトル）
			// 生まれた3軸回転ベクトルからそれぞれの軸との内積を取りArcCosで回転角度を求める
			// そうすれば3軸回転変数と、ViewMatrixが生まれるはず

			Vector3 xAxis(1.f, 0.f, 0.f);
			FVector xmXAxis = XMLoadFloat3(&xAxis);
			Vector3 yAxis(0.f, 1.f, 0.f);
			FVector xmYAxis = XMLoadFloat3(&yAxis);

			FVector eyeDir = focusPosVec - eyePosVec;
			eyeDir = XMVector3Normalize(eyeDir);
			
			float xDot = XMVectorGetX(XMVector3Dot(eyeDir, xmXAxis));
			float yDot = XMVectorGetY(XMVector3Dot(eyeDir, xmYAxis));

			FVector zyCross = XMVector3Cross(xmYAxis,eyeDir);
			FVector xzCross = XMVector3Cross(eyeDir, zyCross);

			FMatrix xmInvRotateMatrix = {};

#if 0
			float yaw = XMScalarACos(xDot);
			float pitch = XMScalarACos(yDot);

			xmInvRotateMatrix = XMMatrixRotationRollPitchYaw(pitch, 0.f, 0.f);
#endif

			xmInvRotateMatrix.r[0] = zyCross;
			xmInvRotateMatrix.r[1] = xzCross;
			xmInvRotateMatrix.r[2] = eyeDir;

			{
				FVector m1 = {}, m2 = {}, m3 = {};
				m1 = xmInvRotateMatrix.r[0];
				m2 = xmInvRotateMatrix.r[1];
				m3 = xmInvRotateMatrix.r[2];

				Vector3 lm1 = {}, lm2 = {}, lm3 = {};
				XMStoreFloat3(&lm1, m1);
				XMStoreFloat3(&lm2, m2);
				XMStoreFloat3(&lm3, m3);

				PRINT("Before\n");

				PRINT_VECTOR("m1", lm1);
				PRINT_VECTOR("m2", lm2);
				PRINT_VECTOR("m3", lm3);

				PRINT("\n");

				// Transpose
				{
					float temp = lm1.y;
					
					lm1.y = lm2.x;
					lm2.x = temp;

					temp = lm1.z;

					lm1.z = lm3.x;
					lm3.x = temp;

					temp = lm2.z;

					lm2.z = lm3.y;
					lm3.y = temp;
				}

				PRINT("After\n");

				m1 = XMLoadFloat3(&lm1);
				m2 = XMLoadFloat3(&lm2);
				m3 = XMLoadFloat3(&lm3);

				PRINT_VECTOR("m1", lm1);
				PRINT_VECTOR("m2", lm2);
				PRINT_VECTOR("m3", lm3);

				PRINT("\n");

				xmInvRotateMatrix.r[0] = m1;
				xmInvRotateMatrix.r[1] = m2;
				xmInvRotateMatrix.r[2] = m3;

			}
			//xmInvRotateMatrix = XMMatrixTranspose(xmInvRotateMatrix);

			//FMatrix xmTransMatrix = {};
			//xmTransMatrix = XMMatrixTranslation(-eyePos.x, -eyePos.y, -eyePos.z);

			FMatrix xmAltViewMatrix = {};

			//xmAltViewMatrix = xmInvRotateMatrix * xmTransMatrix;

			xmAltViewMatrix = xmInvRotateMatrix;

			FVector invEyePos = eyePosVec * -1;
			Vector4 eyePos4 = {};
			XMStoreFloat4(&eyePos4, invEyePos);
			eyePos4.w = 1.f;

			FVector eye4Vec = XMLoadFloat4(&eyePos4);

			xmAltViewMatrix.r[3] = eye4Vec;

			Matrix44 altViewMatrix = {};
			XMStoreFloat4x4(&altViewMatrix, xmAltViewMatrix);

			PRINT("AltViewMatrix\n");
			PRINT_MATRIX44(altViewMatrix);
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

		PRINT("ViewMatrixTest\n");
		MatrixTest::ViewMatrixTest();
		PRINT("\n");
		
	};

	

#undef PRINT_VECTOR

#undef CREATE_XMVECTOR_FROM3

#undef CREATE_AND_LOAD_VECTOR3

#undef STORE_AND_PRINT_VECTOR

};