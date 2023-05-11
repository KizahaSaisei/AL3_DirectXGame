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
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
	delete spriteTitle_;
	delete spriteGameOver_;
	delete spriteKey_;
}

// 　初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// BG(2Dスプライト）
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// Title BG(2Dスプライト）
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// Hit Enter Key(2Dスプライト）
	textureHandleKey_ = TextureManager::Load("enter.png");
	spriteKey_ = Sprite::Create(textureHandleKey_, {390, 520});

	// Game Over(2Dスプライト）
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 100});

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
	for (int i = 0; i < 10; i++) {
		worldTransformBeam_[i].scale_ = {0.15f, 0.15f, 0.15f};
		worldTransformBeam_[i].Initialize();
	}

	// 　敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	// 敵の数ループする
	for (int i = 0; i < 10; i++) 
	{
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}
	srand((unsigned int)time(NULL));

	// デッバクテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
}

// 　更新
void GameScene::Update() {
	// 各シーンの更新処理を呼び出す
	switch (sceneMode_) 
	{
	case 0:
		TitleUpdate();
		break;
	case 1:
		GamePlayUpdate(); // ゲームプレイ更新
		break;
	case 2:
		GameOverUpdate();
		break;
	}
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

	// 各シーンの背景2D表示を呼び出す
	switch (sceneMode_) 
	{
	case 0:
		TitleDraw2DNear();
		break;
	case 1:
		GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
		break;
	case 2:
		GameOverDraw2DBack();
		break;
	}

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

	// 各シーンの3D表示を呼び出す
	switch (sceneMode_)
	{
	case 0:
		break;
	case 1:
		GamePlayDraw3D(); // ゲームプレ3D表示
		break;
	case 2:
		GameOverDraw3D();
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	 
	debugText_->DrawAll();

	// 各シーンの近景2D表示を呼び出す
	switch (sceneMode_) 
	{
	case 0:
		    break;
	case 1:
		    GamePlayDraw2DNear(); // ゲームプレ2D近景表示
		    break;
	case 2:
		    GameOverDraw2DNear();
		    break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

// -------------------------------------
// タイトル
// -------------------------------------

void GameScene::TitleUpdate()
{
	playerLife_ = 3;
	gameScore_ = 0;
	
	for (int i = 0; i < 10; i++) {
		    enemyFlag_[i] = {};
		    beamFlag_[i] = {};
		    worldTransformEnemy_[i].translation_.z = 40.0f;
	}
	gameTimer_ += 1;
	if (input_->TriggerKey(DIK_RETURN))
	{
		    sceneMode_ = 1;
		    audio_->StopWave(voiceHandleBGM_);
		    voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);
	}
}

void GameScene::TitleDraw2DNear()
{
	// 　背景
	spriteTitle_->Draw();
	if (gameTimer_ % 40 >= 20) {
		    spriteKey_->Draw();
	}
}

// -------------------------------------
// ゲームプレイ
// -------------------------------------

// ゲームプレイ更新
void GameScene::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // ビーム更新
	EnemyUpdate();  // 敵更新
	Collision();
}

// ゲームプレイ表示3D
void GameScene::GamePlayDraw3D() {
	// 　ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// 　プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
		    // ビーム
		    if (beamFlag_[i] == 1) {
			    modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
		    }
	// 敵
		    if (enemyFlag_[i] == 1) {
			    modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		    }
	}
}

// ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	// 　背景
	spriteBG_->Draw();
}

// ゲームプレイ表示2D近景
void GameScene::GamePlayDraw2DNear() {
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 980, 10, 2);
}

// -------------------------------------
// ゲームオーバー
// -------------------------------------

void GameScene::GameOverUpdate() 
{
	gameTimer_ += 1;
	if (input_->TriggerKey(DIK_RETURN) && sceneMode_ == 2) 
	{
	sceneMode_ = 0;
		    audio_->StopWave(voiceHandleBGM_);
		    voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
	}
}
void GameScene::GameOverDraw2DBack() 
{
	spriteBG_->Draw();
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 980, 10, 2);
	spriteGameOver_->Draw();
}
void GameScene::GameOverDraw3D() {
	// 　ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// 　プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}
void GameScene::GameOverDraw2DNear() {
	if (gameTimer_ % 40 >= 20) {
	spriteKey_->Draw();
	}
}
// -------------------------------------
// プレイヤー
// -------------------------------------

    // 　プレイヤー更新
void GameScene::PlayerUpdate()
{
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

	if (playerLife_ == 0)
	{
	sceneMode_ = 2;
	audio_->StopWave(voiceHandleBGM_);
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGameOverBGM_, true);
	}

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
	for (int i = 0; i < 10; i++) {
	
	// 　変換行列を更新
	worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		worldTransformBeam_[i].translation_);
	// 　変換行列を定数バッファに転送
	worldTransformBeam_[i].TransferMatrix();

	// 　回転
	worldTransformBeam_[i].rotation_.x += 0.1f;
	}
}
// 　ビーム移動
void GameScene::BeamMove() {
	for (int i = 0; i < 10; i++) {
	if (beamFlag_[i] == 1) {
			    worldTransformBeam_[i].translation_.z += 0.1f;
	}
	if (worldTransformBeam_[i].translation_.z >= 40.0f) {
			    beamFlag_[i] = 0;
	}
	if (beamFlag_[i] == 0) {
			    worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
			    worldTransformBeam_[i].translation_.y = worldTransformPlayer_.translation_.y;
			    worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;
	}
	}
}

// 　ビーム発生（発射）
void GameScene::BeamBorn() {
	for (int i = 0; i < 10; i++) {

	if (beamTimer_ == 0) {
			    if (input_->TriggerKey(DIK_SPACE)) {
				    if (beamFlag_[i] == 0) {
					    beamFlag_[i] = 1;
				    }
			    }
			    beamTimer_++;

	} else {
			    // 発射たいむーが1以上
			    // 10を超えると再び発射が可能
			    beamTimer_++;
			    if (beamTimer_ > 10) {
				    beamTimer_ = 0;
			    }
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
	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
		// 　変換行列を更新
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);
		// 　変換行列を定数バッファに転送
		worldTransformEnemy_[i].TransferMatrix();

		// 　回転
		worldTransformEnemy_[i].rotation_.x -= 0.1f;
	}
}

// 敵移動
void GameScene::EnemyMove() {
	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			worldTransformEnemy_[i].translation_.z -= 0.1f;
		}
		if (worldTransformEnemy_[i].translation_.z <= 0.0f) {
			enemyFlag_[i] = 0;
		}
	}
}

// 敵発生
void GameScene::EnemyBorn() {
	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 0) {
			if (rand() % 10 != 0) {
				    enemyFlag_[i] = 1;
				    // 乱数でX座標の指定
				    int x = rand() % 80;          // 80は4の10倍の2倍
				    float x2 = (float)x / 10 - 4; // 10で割り、4を引く
				    worldTransformEnemy_[i].translation_.x = x2;
				    worldTransformEnemy_[i].translation_.z = 40.0f;
			}
			break;
		}
		    if (rand() % 2 == 0) {
			enemySpeed_[i] = 0.1f;
		    } else {
			enemySpeed_[i] = -0.1f;
		    }
		
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
void GameScene::CollisionPlayerEnemy() 
{
	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
	// 敵が存在すれば
		if (enemyFlag_[i] == 1) {
			// 差を求める
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				enemyFlag_[i] = 0;
				gameScore_ += 0;
				playerLife_ -= 1;
				audio_->PlayWave(soundDataHandlePlayerHitSE_);
			}
		}
	}
}

// 衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {
	EnemyBorn();
	// 敵の数ループする
	for (int i = 0; i < 10; i++) {
		if (enemyFlag_[i] == 1) {
			for (int e = 0; e < 10; e++) {
			// ビームが存在すれば
				if (beamFlag_[e] == 1) {
					// 差を求める
					float dx =
					    abs(worldTransformBeam_[e].translation_.x -
					        worldTransformEnemy_[i].translation_.x);
					float dz =
					    abs(worldTransformBeam_[e].translation_.z -
					        worldTransformEnemy_[i].translation_.z);
					// 衝突したら
					if (dx < 1 && dz < 1) {
						// 存在しない
						enemyFlag_[i] = 0;
						beamFlag_[e] = 0;
						gameScore_ += 1;
						beamTimer_++;
						audio_->PlayWave(soundDataHandleEnemyHitSE_);
					}
				}
			}
		}
	}
}