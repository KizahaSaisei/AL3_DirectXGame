#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityforText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
public:
	// コンストラクタ
	Enemy();

	// デストラクト
	~Enemy();

	// 初期化
	void Intialize(ViewProjection viewProjection_);

	// 更新
	void Update();

	// 3D表示
	void Draw3D();

	// 移動
	void Move();

	// 発生（発射）
	void Born();

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;

	// 存在フラグ
	int aliveFlag_ = 0;
};
