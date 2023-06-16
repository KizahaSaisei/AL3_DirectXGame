#include "Beam.h"

// コンストラクタ
Beam::Beam() {}

// デストラクト
Beam::~Beam() {
	delete modelBeam_; // ビーム
}

// 初期化
void Beam::Intialize(ViewProjection viewProjection, Player* player) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;
	player_ = player;

	// インプットクラス
	input_ = Input::GetInstance();

	// 　ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f / 3, 0.5f / 3, 0.5f / 3};
	worldTransformBeam_.Initialize();
}

// 更新
void Beam::Update() {
	// 発生（発射）
	Born();

	// 　変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);
	// 　変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();

	
}

// 3D表示
void Beam::Draw3D() {
	// ビーム
	if (aliveFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
}

// 移動
void Beam::Move() {
	// ビームの移動
	if (aliveFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.1f;
	}
	if (worldTransformBeam_.translation_.z >= 40.0f) {
		aliveFlag_ = 0;
	}
	// 　回転
	worldTransformBeam_.rotation_.x += 0.1f;
}

// 発生（発射）
void Beam::Born() {
	// 発射
	if (input_->TriggerKey(DIK_SPACE)) {
		if (aliveFlag_ == 0) {
			aliveFlag_ = 1;
		}
	}
	if (aliveFlag_ == 0) {
		worldTransformBeam_.translation_.x = player_->GetX();
		worldTransformBeam_.translation_.z = player_->GetZ();
	}
}