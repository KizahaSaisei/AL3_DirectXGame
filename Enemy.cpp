#include "Enemy.h"

// コンストラクタ
Enemy::Enemy() {}

// デストラクト
Enemy::~Enemy() {
	delete modelEnemy_; // 敵
}

// 初期化
void Enemy::Intialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;

	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
	srand((unsigned int)time(NULL));
}

// 更新
void Enemy::Update() {
	// 敵発生
	Born();

	// 敵移動
	Move();

	// 　変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);
	// 　変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();

	// 　回転
	worldTransformEnemy_.rotation_.x -= 0.1f;
}

// 3D表示
void Enemy::Draw3D() {
	// 敵
	if (aliveFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

// 移動
void Enemy::Move() {
	// 敵の移動
	if (aliveFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.1f;
	}
	if (worldTransformEnemy_.translation_.z <= -2.0f) {
		aliveFlag_ = 0;
	}
}

// 発生（発射）
void Enemy::Born() {
	if (aliveFlag_ == 0) {
		if (rand() % 10 != 0) {
			aliveFlag_ = 1;
			// 乱数でX座標の指定
			int x = rand() % 80;          // 80は4の10倍の2倍
			float x2 = (float)x / 10 - 4; // 10で割り、4を引く
			worldTransformEnemy_.translation_.x = x2;
			worldTransformEnemy_.translation_.z = 40.0f;
		}
	}
}