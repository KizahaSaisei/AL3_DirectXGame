#include "GameScene.h"
#include "MathUtilityforText.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

// 　コンストラクタ
GameScene::GameScene() {}

// 　デストラクタ
GameScene::~GameScene() {
	delete spriteBG_;   // BG
	delete modelStage_; // ステージ
}

// 　初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// BG(2Dスプライト）
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// 　ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 　ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	// 　ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	// 　変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	// 　変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();
                                        // 　プレイヤー
    textureHandlePlayer_ = TextureManager::Load("player.png");
modelPlayer_ = Model::Create();
worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
worldTransformPlayer_.Initialize();

// 　ビーム
textureHandleBeam_ = TextureManager::Load("beam.png");
modelBeam_ = Model::Create();
worldTransformBeam_.scale_ = {0.15f, 0.15f, 0.15f};
worldTransformBeam_.Initialize();

// 　敵
textureHandleEnemy_ = TextureManager::Load("enemy.png");
modelEnemy_ = Model::Create();
worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
worldTransformEnemy_.Initialize();

srand((unsigned int)time(NULL));

// デッバクテキスト
debugText_ = DebugText::GetInstance();
debugText_->Initialize();
}

// 　更新
void GameScene::Update() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // ビーム更新
	EnemyUpdate();  // 敵更新
	Collision();
}

// 　描画
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// 　背景
	spriteBG_->Draw();

	debugText_->DrawAll();

	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	char str2[100];
	sprintf_s(str2, "LIFE %d", playerLife_);
	debugText_->Print(str2, 980, 10, 2);

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 　ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);


	             // 　プレイヤー
	    modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// 　ビーム
	modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);

	// 　プレイヤー
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
             // -------------------------------------
             // プレイヤー
             // -------------------------------------

    // 　プレイヤー更新
    void GameScene::PlayerUpdate() {
	// 　移動

	// 　右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4.0f);

	// 　左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4.0f);

	// 　変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// 　変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}
// -------------------------------------
// ビーム
// -------------------------------------

// 　ビーム更新
void GameScene::BeamUpdate() {
	// ビーム発生
	BeamBorn();

	// 　移動
	BeamMove();

	// 　変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);
	// 　変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();

	// 　回転
	worldTransformBeam_.rotation_.x += 0.1f;
}
// 　ビーム移動
void GameScene::BeamMove() {
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 0.1f;
	}
	if (worldTransformBeam_.translation_.z >= 40.0f) {
		beamFlag_ = 0;
	}
	if (beamFlag_ == 0) {
		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		worldTransformBeam_.translation_.y = worldTransformPlayer_.translation_.y;
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
	}
}

// 　ビーム発生（発射）
void GameScene::BeamBorn() {
	if (input_->PushKey(DIK_SPACE)) {
		if (beamFlag_ == 0) {
			beamFlag_ = 1;
		}
	}
}
// -------------------------------------
// 敵
// -------------------------------------

// 敵更新
void GameScene::EnemyUpdate() {
	// 敵発生
	EnemyBorn();

	// 敵移動
	EnemyMove();

	// 　変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);
	// 　変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();

	// 　回転
	worldTransformEnemy_.rotation_.x -= 0.1f;
}

// 敵移動
void GameScene::EnemyMove() {
	if (enemyFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.1f;
	}
	if (worldTransformEnemy_.translation_.z <= 0.0f) {
		enemyFlag_ = 0;
	}
}

// 敵発生
void GameScene::EnemyBorn() {
	if (enemyFlag_ == 0) {
		enemyFlag_ = 1;
		// 乱数でX座標の指定
		int x = rand() % 80;          // 80は4の10倍の2倍
		float x2 = (float)x / 10 - 4; // 10で割り、4を引く
		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40.0f;
	}
}

// -------------------------------------
// 衝突判定
// -------------------------------------

// 衝突判定
void GameScene::Collision() {
	// 衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();
	// 衝突判定（ビームと敵）
	CollisionBeamEnemy();
}

// 衝突判定（プレイヤーと敵）
void GameScene::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (enemyFlag_ == 1) {
		// 差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemyFlag_ = 0;
			gameScore_ += 0;
			playerLife_ -= 1;
		}
	}
}

// 衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {
	EnemyBorn();
	// ビームが存在すれば
	if (beamFlag_ == 1) {
		// 差を求める
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemyFlag_ = 0;
			beamFlag_ = 0;
			gameScore_ += 1;
		}
	}
}