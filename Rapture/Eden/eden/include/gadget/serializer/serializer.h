/////////////////////////////////////////////////////////////////////////////
///@file	serializer.h
///@brief	CSerializer define
///@data	2020/07/11
////////////////////////////////////////////////////////////////////////////
#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__


EDENS_NAMESPACE_BEGIN

/*
*	Serializer
*/
class CSerializer
{
public:

	void Initialize(const uint8* _Data,uint32 _Length);

	void Finalize();

	bool ReadDataBlob(void* _DescPtr, uint32 _DescSize);
	bool ReadDataBlob(void* _DescPtr, uint32 _DescSize, uint32 _SrcSize);

	// 1Byte読み取る
	uint8 ReadData();

	// 1Byte読み取って参照位置も進める
	// >> ポインタがむき出しになるのであまり外部に出したくない
	//const uint8* ReadDataIncRef();
	bool ReadDataIncRef(uint8* _Dst);

	bool IncrementReadPosition(uint32 _Inc);


	CSerializer() {};
	~CSerializer() {};

private:

	//u8* m_pDataBuffer = nullptr;
	ObjectPtr(const uint8) m_DataBuffer = nullptr;
	u32 m_DataLength = 0;
	u32 m_CurrentPoint = 0;

};

EDENS_NAMESPACE_END

#endif