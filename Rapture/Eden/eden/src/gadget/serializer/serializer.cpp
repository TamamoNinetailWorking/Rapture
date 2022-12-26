#include "eden/include/gadget/serializer/serializer.h"

#include "eden/include/utility/StringUtility.h"

EDENS_NAMESPACE_USING;

void CSerializer::Initialize(const uint8* _Data, uint32 _Length)
{
	if (_Length <= 0)
	{
		return;
	}
	m_DataBuffer = _Data;
	m_DataLength = _Length;
	m_CurrentPoint = 0;
}

void CSerializer::Finalize()
{
	m_CurrentPoint = 0;
	m_DataLength = 0;
	m_DataBuffer = nullptr;
}

bool CSerializer::ReadDataBlob(void* _DescPtr, uint32 _DescSize)
{
	return ReadDataBlob(_DescPtr,_DescSize,_DescSize);
}

bool CSerializer::ReadDataBlob(void* _DescPtr, uint32 _DescSize, uint32 _SrcSize)
{
	if (_DescPtr == nullptr) { return false; };
	if (m_CurrentPoint + _SrcSize >= m_DataLength) { return false; };

	const void* srcPtr = m_DataBuffer + m_CurrentPoint;
	errno_t erro = memcpy_s(_DescPtr, _DescSize, srcPtr, _SrcSize);
	
	if (erro != 0)
	{
		StringUtility::ErrorPrint(erro);
		return false;
	}

	m_CurrentPoint += _SrcSize;

	return true;
}

uint8 CSerializer::ReadData()
{
	return m_DataBuffer[m_CurrentPoint];
}

//const uint8* CSerializer::ReadDataIncRef()
//{
//	if (m_CurrentPoint + 1 > m_DataLength)
//	{
//		return nullptr;
//	}
//
//	const uint8* dataPtr = m_DataBuffer;
//	++m_CurrentPoint;
//	return dataPtr;
//}

bool CSerializer::ReadDataIncRef(uint8* _Dst)
{
	if (m_CurrentPoint + 1 >= m_DataLength)
	{
		return false;
	}

	*_Dst = m_DataBuffer[m_CurrentPoint++];
	return true;
}

bool CSerializer::IncrementReadPosition(uint32 _Inc)
{
	if (_Inc + m_CurrentPoint >= m_DataLength)
	{
		return false;
	}

	m_CurrentPoint += _Inc;

	return true;
}
