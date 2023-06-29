#include "GamePlay.h"
// コンストラクタ
GamePlay::GamePlay() {}

// デストラクト
GamePlay::~GamePlay() 
{
	// 各クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	for (int i = 0; i < 10; i++) 
	{
		delete enemyTable_[i]; // 敵
		delete beamTable_[i];  // ビーム
	}
}

// 初期化
void GamePlay::Intialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;

	// 各クラスの生成
	stage_ = new Stage();   // ステージ
	player_ = new Player(); // プレイヤー
	for (int i = 0; i < 10; i++) 
	{
	enemyTable_[i] = new Enemy(); // 敵
	beamTable_[i] = new Beam(); // ビーム
	}

	// 各クラスの初期化
	stage_->Intialize(viewProjection_);         // ステージ
	player_->Intialize(viewProjection_);        // プレイヤー
	for (int i = 0; i < 10; i++) 
	{
	enemyTable_[i]->Intialize(viewProjection_); // 敵
	beamTable_[i]->Intialize(viewProjection_, player_); // ビーム
	}

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

// 初期化
void GamePlay::Start() { 
	playerLife_ = 3;
	gameScore_ = 0;
	player_->Start();
	player_->Born();
	for (int i = 0; i < 10; i++)
	{
	enemyTable_[i]->Hit();
	beamTable_[i]->Hit();
	enemyTable_[i]->Z();
	beamTable_[i]->Born();
	enemyTable_[i]->Born();
	shotTimer_[i] = {0};
	}
}

int GamePlay::Update() 
{
	// 各クラスの更新
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	for (int i = 0; i < 10; i++)
	{
	enemyTable_[i]->Update(); // 敵
	beamTable_[i]->Update();  // ビーム
	}
	if (playerLife_ > 0) {
	for (int i = 0; i < 10; i++) {
		enemyTable_[i]->Move(); // 敵
		beamTable_[i]->Move();  // ビーム
	}
	player_->Move(); // プレイヤー
	}

		// インプットクラス
	input_ = Input::GetInstance();

	// 射撃
	Shot();

	// 衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();

	// 衝突判定（ビームと敵）
	CollisionBeamEnemy();
	timer_ += 1;

	if (playerLife_ <= 0)
	{
		return 2;
	}
	return 0;
}

// 射撃
void GamePlay::Shot() {

	for (int i = 0; i < 10; i++) {
		// 射撃タイマーが0ならば
		if (shotTimer_[i] == 0) {
		// スペースキーを押したら
		if (input_->PushKey(DIK_SPACE)) {
			// ビームでループ

			// 存在しなければ
			if (beamTable_[i]->GetFlag() == 0) {
				// 発射(発生)
				beamTable_[i]->Move(); // ビーム
				beamTable_[i]->Born();

				// 射撃タイマー
				shotTimer_[i] = 1;
				break;
			}
		}
		shotTimer_[i]++;
		}

		// 発射できない状態
		else {
		// タイマー加算
		shotTimer_[i]++;
		// 一定時間で
		if (shotTimer_[i] >= 10) {
			// 発射できる状態
			shotTimer_[i] = 0;
		}
		}
	}
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
	
	for (int i = 0; i < 10; i++) 
	{
		// ビームの描画
		beamTable_[i]->Draw3D();

		// 敵の描画
		enemyTable_[i]->Draw3D();
	}
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
	for (int i = 0; i < 10; i++) {
		if (enemyTable_[i]->GetFlag() == 1) {
		// 差を求める
		float dx = abs(player_->GetX() - enemyTable_[i]->GetX());
		float dz = abs(player_->GetZ() - enemyTable_[i]->GetZ());

		// 衝突したら
		if (dx < 1 && dz < 1) {
			// 存在しない
			enemyTable_[i]->Hit();
			player_->Hit();
			playerLife_ -= 1;
		}
		}
	}
}

// 衝突判定（ビームと敵）
void GamePlay::CollisionBeamEnemy() {
	for (int i = 0; i < 10; i++) {
		// enemyTable_[i]->Born();
		if (enemyTable_[i]->GetFlag() == 1) {
		for (int e = 0; e < 10; e++) {
			// ビームが存在すれば
			if (beamTable_[e]->GetFlag() == 1) {
				// 差を求める
				float dx = abs(beamTable_[e]->GetX() - enemyTable_[i]->GetX());
				float dz = abs(beamTable_[e]->GetZ() - enemyTable_[i]->GetZ());
				// 衝突したら
				if (dx < 1 && dz < 1) {
					// 存在しない
					enemyTable_[i]->Hit();
					beamTable_[e]->Hit();
					gameScore_ += 1;
				}
			}
		}
		}
	}
}