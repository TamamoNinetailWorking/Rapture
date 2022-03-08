#pragma once

#include <array>
#include <string>
#include <functional>

EDENS_NAMESPACE_BEGIN

struct FSHA1
{
public:

	FSHA1() {};
	~FSHA1() {};

	FSHA1 operator=(const FSHA1& _right)
	{
		UpperByte = _right.UpperByte;
		MiddleByte = _right.MiddleByte;
		UnderByte = _right.UnderByte;
	}

	bool operator==(const FSHA1& _right)
	{
		return (UpperByte == _right.UpperByte) &&
			(MiddleByte == _right.MiddleByte) &&
			(UnderByte == _right.UnderByte);
	}

	bool operator !=(const FSHA1& _right)
	{
		return !(*this == _right);
	}

	bool operator >(const FSHA1& _right)
	{
		return UnderByte > _right.UnderByte;
	}

	bool operator >=(const FSHA1& _right)
	{
		return UnderByte >= _right.UnderByte;
	}

	bool operator <(const FSHA1& _right)
	{
		return UnderByte < _right.UnderByte;
	}

	bool operator <=(const FSHA1& _right)
	{
		return UnderByte <= _right.UnderByte;
	}

private:

	friend class CSHA1;

	u64 UpperByte = 0;
	u64 MiddleByte = 0;
	u32 UnderByte = 0;

};

class CSHA1
{
public:

	CSHA1() {};
	explicit CSHA1(const char* _string);
	explicit CSHA1(const std::string& _string);
	~CSHA1() {};

	CSHA1 operator=(const CSHA1& _right)
	{
		m_Hash = _right.m_Hash;
	}

	bool operator==(const CSHA1& _right)
	{
		return m_Hash == _right.m_Hash;
	}

	bool operator !=(const CSHA1& _right)
	{
		return m_Hash != _right.m_Hash;
	}

	bool operator >(const CSHA1& _right)
	{
		return m_Hash > _right.m_Hash;
	}

	bool operator >=(const CSHA1& _right)
	{
		return m_Hash >= _right.m_Hash;
	}

	bool operator <(const CSHA1& _right)
	{
		return m_Hash < _right.m_Hash;
	}

	bool operator <=(const CSHA1& _right)
	{
		return m_Hash <= _right.m_Hash;
	}

private:

	// �N���X���Ŏg�p����֐���`
	typedef std::function<bool(u32)> SequenceCheck;
	typedef std::function<u32(u32, u32, u32)> Logic;

	// �n�b�V���{��
	FSHA1 m_Hash = {};


	// �n�b�V�����̃��C������
	void SHA1Main(const std::string& _string);

	// SHA1�̃n�b�V�����H����
	void SHA1Process(struct FSHAProxy* _proxy);

	// SHA1�̃n�b�V���o��
	void SHA1Result(struct FSHAProxy* _proxy);


	// 0 - 19�Ŏg�p
	static bool Check00(u32 _Num);
	// 20 -39
	static bool Check20(u32 _Num);
	// 40 - 59
	static bool Check40(u32 _Num);
	// 60 -79
	static bool Check60(u32 _Num);

	// 0 -19�Ŏg�p
	static u32 Logic00(u32 _B, u32 _C, u32 _D);;
	// 20 -39	Logic
	static u32 Logic20(u32 _B, u32 _C, u32 _D);
	// 40 - 59	Logic
	static u32 Logic40(u32 _B, u32 _C, u32 _D);
	// 60 -79	Logic
	static u32 Logic60(u32 _B, u32 _C, u32 _D);

	// SHA1�ϊ��V�[�P���X��
	static const u8 SequenceNum = 4;

	// �`�F�b�N�֐��Q
	static const std::array<SequenceCheck, SequenceNum> m_CheckFunction;

	// �n�b�V���v�Z�Ŏg�p����萔�l
	static const std::array<u32, SequenceNum> m_HashConstant;

	// �n�b�V���v�Z�Ŏg�p����_���֐�
	static const std::array<Logic, SequenceNum> m_LogicFunction;

};


#define StaticSHA1(string) static const CSHA1(string);



EDENS_NAMESPACE_END