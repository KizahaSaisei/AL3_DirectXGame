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

	// ライフの獲得
	int GetFlag() { return aliveFlag_; }

	// 衝突判定
	void Hit() { aliveFlag_ = 0; }

	// x座標の獲得
	float GetX() { return worldTransformEnemy_.translation_.x; }
	// z座標の獲得
	float GetZ() { return worldTransformEnemy_.translation_.z; }

	void Z() { worldTransformEnemy_.translation_.z = 40.0f; }

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;

	// 存在フラグ
	int aliveFlag_ = 0;

	// 横方向スピード
	float xSpeed_ = 0;
};
