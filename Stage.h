#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityforText.h"

class Stage 
{
public:
	// コンストラクタ
	Stage();

	// デストラクト
	~Stage();

	// 初期化
	void Intialize(ViewProjection viewProjection_);

	// 更新
	void Update();

	// 2D背景描画
	void Draw2DFar();

	// 3D表示
	void Draw3D();

private:
	// BG（スプライト）
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;
	
};