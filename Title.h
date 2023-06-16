#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MathUtilityforText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "WorldTransform.h"

class Title {
public:
	// コンストラクタ
	Title();

	// デストラクト
	~Title();

	// 初期化
	void Intialize();

	// 更新
	int Update();

	// タイトル表示2D近景
	void Draw2DNear();

private:
	// Title
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	// Hit Enter Key（スプライト）
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	// インプットクラス
	Input* input_ = nullptr;
	
	int timer_ = 0;
};