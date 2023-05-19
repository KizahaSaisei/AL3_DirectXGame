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

	// 3D表示
	void Draw3D();

	// x座標の獲得
	float GetX() { return worldTransformPlayer_.translation_.x; }
	// y座標の獲得
	float GetY() { return worldTransformPlayer_.translation_.y; }
	// z座標の獲得
	float GetZ() { return worldTransformPlayer_.translation_.z; }

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	// インプットクラス
	Input*input_ = nullptr;

};
