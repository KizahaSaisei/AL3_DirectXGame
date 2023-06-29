#pragma once
#include "Player.h"
#include "DirectXCommon.h"
#include "MathUtilityforText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"

class Beam {
public:
	// コンストラクタ
	Beam();

	// デストラクト
	~Beam();

	// 初期化
	void Intialize(ViewProjection viewProjection_, Player* player);

	// 更新
	void Update();

	// 3D表示
	void Draw3D();

	// 移動
	void Move();

	// 発生（発射）
	void Born();

	// x座標の獲得
	float GetX() { return worldTransformBeam_.translation_.x; }
	// y座標の獲得
	float GetY() { return worldTransformBeam_.translation_.y; }
	// z座標の獲得
	float GetZ() { return worldTransformBeam_.translation_.z; }
	
	// ライフの獲得
	int GetFlag() { return aliveFlag_; }

	// 衝突判定
	void Hit() { aliveFlag_ = 0; }

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	// インプットクラス
	Input* input_ = nullptr;
	// 存在フラグ
	int aliveFlag_ = 0;

	// プレイヤー
	Player* player_ = nullptr;

};
