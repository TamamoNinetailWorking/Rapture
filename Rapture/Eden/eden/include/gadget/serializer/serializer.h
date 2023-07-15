/////////////////////////////////////////////////////////////////////////////
///@file	serializer.h
///@brief	CSerializer define
////////////////////////////////////////////////////////////////////////////
#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__


EDENS_NAMESPACE_BEGIN

/** 
*	@brief データシリアライズクラス
*/
class CSerializer
{
public:

	/**
	 * @brief シリアライザーの初期化
	 * 
	 * \param[in] _Data シリアライズするデータの先頭ポインタ
	 * \param[in] _Length シリアライズするデータの長さ
	 */
	void Initialize(const uint8* _Data,uint32 _Length);

	/**
	 * @brief シリアライザーの終了処理
	 * 	 
	 */
	void Finalize();

	/**
	 * @brief シリアライザーのデータを読み取る
	 * 
	 * \param[out] _DescPtr 読み取ったデータを格納する先頭ポインタ
	 * \param[in] _DescSize 読み取ったデータを格納可能なデータサイズ
	 * \return 読み取りに成功したかどうか
	 * @details シリアライザの参照位置を進められなかった場合はfalseを返す
	 */
	bool ReadDataBlob(void* _DescPtr, uint32 _DescSize);

	/**
		 * @brief シリアライザーのデータを読み取る
		 * 	 * 
		 * 	 * \param[out] _DescPtr 読み取ったデータを格納する先頭ポインタ
		 * 	 * \param[in] _DescSize 読み取るデータを格納可能なデータサイズ
		 * 	 * \param[in] _SrcSize 読み取るデータのサイズ
		 * 	 * \return 読み取りに成功したかどうか
		 * @details シリアライザの参照位置を進められなかった場合はfalseを返す
		 * 	 */
	bool ReadDataBlob(void* _DescPtr, uint32 _DescSize, uint32 _SrcSize);

	/**
	 * @brief 1バイトデータを読み取る
	 * 
	 * \return 読み取ったデータ1バイト分
	 */
	uint8 ReadData();

	// 1Byte読み取って参照位置も進める
	// >> ポインタがむき出しになるのであまり外部に出したくない
	//const uint8* ReadDataIncRef();

	/**
	 * @brief 1バイトデータを読み取り、シリアライザの参照位置を進める
	 * 
	 * \param[out] _Dst 読み取るデータの格納ポインタ
	 * \return データを読み取れたかどうか
	 * @details	進めた先がデータの長さを超えていた場合は進めない
	 */
	bool ReadDataIncRef(uint8* _Dst);

	/**
	 * @brief シリアライザの参照位置を進める
	 * 
	 * \param[in] _Inc 進めるバイト数
	 * \return 読み進めることができたかどうか
	 * @details	進めた先がデータの長さを超えていた場合は進めない
	 */
	bool IncrementReadPosition(uint32 _Inc);

	/**
	 * @brief コンストラクタ
	 */
	CSerializer() {};

	/**
	 * @brief デストラクタ
	 */
	~CSerializer() {};

private:

	//u8* m_pDataBuffer = nullptr;

	/** シリアライズするデータの先頭ポインタ */
	ObjectPtr(const uint8) m_DataBuffer = nullptr;

	/** シリアライズするデータの長さ */
	u32 m_DataLength = 0;

	/** データの読み取り位置 */
	u32 m_CurrentPoint = 0;

};

EDENS_NAMESPACE_END

#endif