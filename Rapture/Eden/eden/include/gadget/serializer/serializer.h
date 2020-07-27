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

	void Initialize();


	void Finalize();




private:

	u8* m_pDataBuffer = nullptr;
	s32 m_DataLength = 0;
	s32 m_CurrentPoint = 0;

};

EDENS_NAMESPACE_END

#endif