/*****************************************************************//**
 * \file   sha1.h
 * \brief  SHA1ハッシュ関数の定義
 *********************************************************************/
#pragma once

#include <array>
//#include <string>
#include <vector>
#include <functional>

EDENS_NAMESPACE_BEGIN

/**
 * @brief SHA1ハッシュクラス
 */
struct FSHA1
{
public:

	/**
	 * @brief コンストラクタ
	 */
	FSHA1() {};

	/**
	 * @brief デストラクタ
	 */
	~FSHA1() {};

	/**
	 * @brief ハッシュ値の代入
	 * 
	 * \param[in] _right 代入するハッシュ値
	 * \return 自身の値
	 */
	FSHA1 operator=(const FSHA1& _right)
	{
		UpperByte = _right.UpperByte;
		MiddleByte = _right.MiddleByte;
		UnderByte = _right.UnderByte;
		return *this;
	}

	/**
	 * @brief ハッシュ値の一致比較
	 * 
	 * \param[in] _right　比較するハッシュ値
	 * \return 同一のハッシュ値かどうか
	 */
	bool operator==(const FSHA1& _right) const
	{
		return (UpperByte == _right.UpperByte) &&
			(MiddleByte == _right.MiddleByte) &&
			(UnderByte == _right.UnderByte);
	}

	/**
	 * @brief ハッシュ値の不一致比較
	 * 
	 * \param[in] _right 比較するハッシュ値
	 * \return 同一のハッシュ値でないかどうか
	 */
	bool operator!=(const FSHA1& _right) const
	{
		return !(*this == _right);
	}

	/**
	 * @brief ハッシュ値の大小比較
	 * 
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が大きいかどうか
	 */
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

	/**
	 * @brief ハッシュ値の大小等価比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が大きいもしくは等価かどうか
	 */
	bool operator>=(const FSHA1& _right) const
	{
		if (*this == _right) { return true; };
		if (*this > _right) { return true; }
		return false;
	}

	/**
	 * @brief ハッシュ値の大小比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が小さいかどうか
	 */
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

	/**
	 * @brief ハッシュ値の大小等価比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が小さいもしくは等価かどうか
	 */
	bool operator<=(const FSHA1& _right) const
	{
		if (*this == _right) { return true; };
		if (*this < _right) { return true; }
		return false;
	}

private:

	friend class CSHA1;

	/** 160バイトのうちの上位64バイト  */
	u64 UpperByte = 0;

	/** 160バイトのうちの中間64バイト（65～128) */
	u64 MiddleByte = 0;

	/** 160バイトのうちの下位32バイト（129～160) */
	u32 UnderByte = 0;

};


/**
 * @brief SHA1ハッシュ値を計算するクラス
 */
class CSHA1
{
public:

	using DataBlob = std::vector<uint8>;

	/**
	 * @brief コンストラクタ
	 * @details 空のハッシュ値を保持する
	 */
	CSHA1() {};

	/**
	 * @brief コンストラクタ
	 * @param[in] _Data ハッシュ値を計算するデータ
	 * @param[in] _Size データのサイズ
	 */
	explicit CSHA1(const uint8* _Data, uint64 _Size);

	/**
	 * @brief コンストラクタ
	 * 
	 * \param[in] _string ハッシュ値を計算する文字列
	 */
	explicit CSHA1(const std::string& _string);
	
	/**
	 * @brief コンストラクタ
	 * 
	 * \param[in] _string ハッシュ値を計算する文字列
	 * @param[in] _size 文字列のサイズ
	 */
	explicit CSHA1(const char* _string);

	/**
	 * @brief コンストラクタ
	 * 
	 * \param[in] _string ハッシュ値を計算する文字列
	 */
	explicit CSHA1(const DataBlob& _string);

	/**
	 * @brief コピーコンストラクタ
	 * @param[in] _sha1 コピー元のハッシュ値
	 */
	CSHA1(const CSHA1& _sha1) { m_Hash = _sha1.m_Hash; };

	/**
	 * @brief コピーコンストラクタ
	 * @param[in] _sha1 コピー元のハッシュ値
	 */
	CSHA1(const FSHA1& _sha1) { m_Hash = _sha1; };

	~CSHA1() {};

	/**
	 * @brief ハッシュ値を代入する
	 * 
	 * \param[in] _right 代入するハッシュ値
	 * \return 自身のハッシュ値
	 */
	FSHA1 operator=(const CSHA1& _right)
	//CSHA1 operator=(const CSHA1& _right)
	{
		m_Hash = _right.m_Hash;
		return m_Hash;
		//return *this;
	}

	/**
	 * @brief ハッシュ値を代入する
	 *
	 * \param[in] _right 代入するハッシュ値
	 * \return 自身のハッシュ値
	 */
	FSHA1 operator=(const FSHA1& _right)
	//CSHA1 operator=(const FSHA1& _right)
	{
		m_Hash = _right;
		return m_Hash;
		//return *this;
	}

	/**
	 * @brief ハッシュ値の一致比較
	 *
	 * \param[in] _right　比較するハッシュ値
	 * \return 同一のハッシュ値かどうか
	 */
	bool operator==(const CSHA1& _right) const
	{
		return m_Hash == _right.m_Hash;
	}

	/**
	 * @brief ハッシュ値の不一致比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 同一のハッシュ値でないかどうか
	 */
	bool operator!=(const CSHA1& _right) const
	{
		return m_Hash != _right.m_Hash;
	}

	/**
	 * @brief ハッシュ値の大小比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が大きいかどうか
	 */
	bool operator>(const CSHA1& _right) const
	{
		return m_Hash > _right.m_Hash;
	}

	/**
	 * @brief ハッシュ値の大小等価比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が大きいもしくは等価かどうか
	 */
	bool operator>=(const CSHA1& _right) const
	{
		return m_Hash >= _right.m_Hash;
	}

	/**
	 * @brief ハッシュ値の大小比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が小さいかどうか
	 */
	bool operator<(const CSHA1& _right) const
	{
		return m_Hash < _right.m_Hash;
	}

	/**
	 * @brief ハッシュ値の大小等価比較
	 *
	 * \param[in] _right 比較するハッシュ値
	 * \return 自身の保持するハッシュ値が小さいもしくは等価かどうか
	 */
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
	//void SHA1Main(const std::string& _string);
	void SHA1Main(const DataBlob& _Data);

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
