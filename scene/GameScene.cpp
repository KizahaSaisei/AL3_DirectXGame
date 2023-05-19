#include "GameScene.h"
#include "MathUtilityforText.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

// 　コンストラクタ
GameScene::GameScene() {}

// 　デストラクタ
GameScene::~GameScene() {

	// 各クラスの削除
	delete stage_; // ステージ
	delete player_; // プレイヤー
	delete beam_; // ビーム
}

// 　初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// 各クラスの生成
	stage_ = new Stage(); // ステージ
	player_ = new Player(); // プレイヤー
	beam_ = new Beam(); // ビーム

	// 各クラスの初期化
	stage_->Intialize(viewProjection_); // ステージ
	player_->Intialize(viewProjection_); // プレイヤー
	beam_->Intialize(viewProjection_, player_); // ビーム
}

// 　更新
void GameScene::Update() {
	// 各クラスの更新
	stage_->Update(); // ステージ
	player_->Update(); // プレイヤー
	beam_->Update(); // ビーム
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
	
	// 背景の描画
	stage_->Draw2DFar();

	/// </summary>

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
	
	// ステージの描画
	stage_->Draw3D();

	// プレイヤーの描画
	player_->Draw3D(); 

	// ビームの描画
	beam_->Draw3D(); 

	/// </summary>

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