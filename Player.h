#pragma once
#include "DirectXCommon.h"
#include "MathUtilityforText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"


class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクト
	~Player();

	// 初期化
	void Intialize(ViewProjection viewProjection_);

	// 更新
	void Update();

	// 移動
	void Move();

	// 3D表示
	void Draw3D();
	
	void Born();

	// x座標の獲得
	float GetX() { return worldTransformPlayer_.translation_.x; }
	// z座標の獲得
	float GetZ() { return worldTransformPlayer_.translation_.z; }

	// ライフの獲得
	int GetFlag() { return aliveFlag_; }

	// 衝突判定
	void Hit() { aliveFlag_ -= 1; }
	void End() { aliveFlag_ = 0; }
	void Start() { aliveFlag_ = 3; }

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	// インプットクラス
	Input*input_ = nullptr;

	// 存在フラグ
	int aliveFlag_ = 3;

};
