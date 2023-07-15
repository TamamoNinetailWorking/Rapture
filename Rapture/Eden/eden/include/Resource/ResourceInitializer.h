/*****************************************************************//**
 * \file   ResourceInitializer.h
 * \brief  リソースの初期化情報を格納する構造体定義
 *********************************************************************/
#pragma once


EDENS_NAMESPACE_BEGIN

/**
 * @brief リソースの初期化情報を格納する構造体
 */
struct FResourceInitializerBase
{
	/** リソースの名前 */
	Hash160 Name = Hash160();
};

EDENS_NAMESPACE_END