#pragma once

// ゲームマネージャーを初期化する際の初期化用構造体
struct FGameManagerInitializer
{
	HWND WindowHandle = 0;
	u32 ViewportWidth = 0;
	u32 ViewportHeight = 0;
};