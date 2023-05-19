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
