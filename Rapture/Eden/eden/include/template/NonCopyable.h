#pragma once

//派生クラスをコピー禁止にするために
//代入演算子のオーバーロードをprivate化
//	→Singletonの基底クラスだった。
class CNonCopyable
{
protected://継承しても引き続き使用可能

	CNonCopyable() {};
	virtual ~CNonCopyable() {};

private:
	//代入処理に当たるものをprivateにして使用禁止化
	CNonCopyable(const CNonCopyable&);
	const CNonCopyable& operator=(const CNonCopyable&);
};