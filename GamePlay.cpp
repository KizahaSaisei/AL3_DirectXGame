#include "GamePlay.h"
// コンストラクタ
GamePlay::GamePlay() {}

// デストラクト
GamePlay::~GamePlay() 
{
	// 各クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	delete beam_;   // ビーム
	delete enemy_;  // 敵
}

// 初期化
void GamePlay::Intialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;

	// 各クラスの生成
	stage_ = new Stage();   // ステージ
	player_ = new Player(); // プレイヤー
	beam_ = new Beam();     // ビーム
	enemy_ = new Enemy();   // ビーム

	// 各クラスの初期化
	stage_->Intialize(viewProjection_);         // ステージ
	player_->Intialize(viewProjection_);        // プレイヤー
	beam_->Intialize(viewProjection_, player_); // ビーム
	enemy_->Intialize(viewProjection_);         // 敵

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

// 初期化
void GamePlay::Start() { 
	playerLife_ = 3;
	gameScore_ = 0;
	player_->GetFlag();
	enemy_->GetFlag();
	beam_->GetFlag();
}

int GamePlay::Update() 
{
	// 各クラスの更新
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	beam_->Update();   // ビーム
	enemy_->Update();  // 敵
	if (playerLife_ > 0) {
		enemy_->Move(); // 敵
		beam_->Move(); // ビーム
		player_->Move(); // プレイヤー
	}


	// 衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();

	// 衝突判定（ビームと敵）
	CollisionBeamEnemy();
	timer_ += 1;

	if (playerLife_ <= 0) {
		playerLife_ = 0;
		player_->Hit();
		enemy_->Hit();
		beam_->Hit();
		return 2;
	}
	return 0;
}

// ゲームプレイ表示2D背景
void GamePlay::Draw2DFar() 
{
	// 背景の描画
	stage_->Draw2DFar();
}

// 3D表示
void GamePlay::Draw3D() 
{
	// ステージの描画
	stage_->Draw3D();

	// プレイヤーの描画
	player_->Draw3D();

	// ビームの描画
	beam_->Draw3D();

	// 敵の描画
	enemy_->Draw3D();
}

// ゲームプレイ表示2D近景
void GamePlay::Draw2DNear() 
{
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 980, 10, 2);

	debugText_->DrawAll();
}

// 衝突判定（プレイヤーと敵）
void GamePlay::CollisionPlayerEnemy() {
	// 敵が存在すれば
	if (enemy_->GetFlag() == 1) {
		// 差を求める
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemy_->Hit();
			player_->Hit();
			playerLife_ -= 1;
		}
	}
}

// 衝突判定（ビームと敵）
void GamePlay::CollisionBeamEnemy() {
	enemy_->Born();
	if (enemy_->GetFlag() == 1) {
		// ビームが存在すれば
		if (beam_->GetFlag() == 1) {
			// 差を求める
			float dx = abs(beam_->GetX() - enemy_->GetX());
			float dz = abs(beam_->GetZ() - enemy_->GetZ());
			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				enemy_->Hit();
				beam_->Hit();
				gameScore_ += 1;
			}
		}
	}
}