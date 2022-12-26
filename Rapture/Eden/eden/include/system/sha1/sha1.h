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
		return *this;
	}

	bool operator==(const FSHA1& _right) const
	{
		return (UpperByte == _right.UpperByte) &&
			(MiddleByte == _right.MiddleByte) &&
			(UnderByte == _right.UnderByte);
	}

	bool operator!=(const FSHA1& _right) const
	{
		return !(*this == _right);
	}

	bool operator>(const FSHA1& _right) const
	{
		if (UpperByte > _right.UpperByte)
		{
			return true;
		}
		else if (UpperByte == _right.UpperByte)
		{
			if (MiddleByte > _right.MiddleByte)
			{
				return true;
			}
			else if (MiddleByte == _right.MiddleByte)
			{
				if (UnderByte > _right.UnderByte)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		return false;
	}

	bool operator>=(const FSHA1& _right) const
	{
		if (*this == _right) { return true; };
		if (*this > _right) { return true; }
		return false;
	}

	bool operator<(const FSHA1& _right) const
	{
		if (UpperByte < _right.UpperByte)
		{
			return true;
		}
		else if (UpperByte == _right.UpperByte)
		{
			if (MiddleByte < _right.MiddleByte)
			{
				return true;
			}
			else if (MiddleByte == _right.MiddleByte)
			{
				if (UnderByte < _right.UnderByte)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		return false;
	}

	bool operator<=(const FSHA1& _right) const
	{
		if (*this == _right) { return true; };
		if (*this < _right) { return true; }
		return false;
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
	CSHA1(const CSHA1& _sha1) { m_Hash = _sha1.m_Hash; };
	CSHA1(const FSHA1& _sha1) { m_Hash = _sha1; };

	~CSHA1() {};

	FSHA1 operator=(const CSHA1& _right)
	{
		m_Hash = _right.m_Hash;
		return m_Hash;
	}

	FSHA1 operator=(const FSHA1& _right)
	{
		m_Hash = _right;
		return m_Hash;
	}

	bool operator==(const CSHA1& _right) const
	{
		return m_Hash == _right.m_Hash;
	}

	bool operator!=(const CSHA1& _right) const
	{
		return m_Hash != _right.m_Hash;
	}

	bool operator>(const CSHA1& _right) const
	{
		return m_Hash > _right.m_Hash;
	}

	bool operator>=(const CSHA1& _right) const
	{
		return m_Hash >= _right.m_Hash;
	}

	bool operator<(const CSHA1& _right) const
	{
		return m_Hash < _right.m_Hash;
	}

	bool operator<=(const CSHA1& _right) const
	{
		return m_Hash <= _right.m_Hash;
	}

private:

	// クラス内で使用する関数定義
	typedef std::function<bool(u32)> SequenceCheck;
	typedef std::function<u32(u32, u32, u32)> Logic;

	// ハッシュ本体
	FSHA1 m_Hash = {};


	// ハッシュ化のメイン処理
	void SHA1Main(const std::string& _string);

	// SHA1のハッシュ加工処理
	void SHA1Process(struct FSHAProxy* _proxy);

	// SHA1のハッシュ出力
	void SHA1Result(struct FSHAProxy* _proxy);


	//// 0 - 19で使用
	//static bool Check00(u32 _Num);
	//// 20 -39
	//static bool Check20(u32 _Num);
	//// 40 - 59
	//static bool Check40(u32 _Num);
	//// 60 -79
	//static bool Check60(u32 _Num);

	//// 0 -19で使用
	//static u32 Logic00(u32 _B, u32 _C, u32 _D);
	//// 20 -39	Logic
	//static u32 Logic20(u32 _B, u32 _C, u32 _D);
	//// 40 - 59	Logic
	//static u32 Logic40(u32 _B, u32 _C, u32 _D);
	//// 60 -79	Logic
	//static u32 Logic60(u32 _B, u32 _C, u32 _D);

	// SHA1変換シーケンス数
	static const u8 SequenceNum = 4;

	// チェック関数群
	//static const std::array<SequenceCheck, SequenceNum> m_CheckFunction;

	// ハッシュ計算で使用する定数値
	static const std::array<u32, SequenceNum> m_HashConstant;

	// ハッシュ計算で使用する論理関数
	//static const std::array<Logic, SequenceNum> m_LogicFunction;

};

//typedef FSHA1 Hash160;

EDENS_NAMESPACE_END


typedef EDENS_NAMESPACE::CSHA1 Hash160;
