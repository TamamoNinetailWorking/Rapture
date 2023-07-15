/*****************************************************************//**
 * \file   pointer.h
 * \brief  ポインター定義
 * 
 *********************************************************************/
#pragma once

/*
*    CTextureで露見したunique_ptrの弱点
    スマートポインタとして唯一そのポインタの管理ができるが、vectorなどの動的配列で持てない
        Ptr周りは独自定義で持ち直したほうがよさそう
        stdで取り扱っているvectorなども書き換えておきたいかもしれない
            ただこれをやりだすと独自エンジン作りが加速してしまうので、やめておきたい
        Ptr周りだけはコンパイルエラーも誘発してしまうので整頓しておく必要がある
*/

#define ObjectPtr(_type) _type*
#define UniquePtr(_type) std::unique_ptr<_type>
#define SharedPtr(_type) std::shared_ptr<_type>
#define WeakPtr(_type) std::weak_ptr<_type>