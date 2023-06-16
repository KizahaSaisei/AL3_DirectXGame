#include "GameScene.h"
#include "MathUtilityforText.h"
#include "TextureManager.h"
#include "time.h"
#include <cassert>

// 　コンストラクタ
GameScene::GameScene() {}

// 　デストラクタ
GameScene::~GameScene() {
	delete gamePlay_;
	delete title_;
	delete gameOver_;
}

// 　初期化
void GameScene::Initialize() 
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	gamePlay_ = new GamePlay(); // ゲームプレイ
	title_ = new Title();       // タイトル
	gameOver_ = new GameOver(); // ゲームオーバー

	gamePlay_->Intialize(viewProjection_); // ゲームプレイ初期化
	title_->Intialize(); // タイトル初期化
	gameOver_->Intialize(viewProjection_); // ゲームオーバー初期化
}

// 　更新
void GameScene::Update() {
	// 現在のモードを記録
	int oldSceneMode = sceneMode_;

	// 各シーン更新
	switch (sceneMode_) {
	case 1:
		sceneMode_ = title_->Update(); // タイトル更新
		break;
	case 0:
		sceneMode_ = gamePlay_->Update(); // ゲームプレイ更新
	case 2:
		sceneMode_ = gameOver_->Update();
		break;
	}

	if (oldSceneMode != 0) {
		switch (sceneMode_) {
		case 0:
			gamePlay_->Start(); // ゲームプレイ
			break;
		}
	}
	if (oldSceneMode != 2) {
		switch (sceneMode_) {
		case 2:
			break;
		}
	}
	if (oldSceneMode != 1) {
		switch (sceneMode_) {
		case 1:
			break;
		}
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
	
	// 各シーン2D背景描画
	switch (sceneMode_) 
	{
	case 1:
		break;
	case 0:
		gamePlay_->Draw2DFar(); // ゲームプレイ表示2D背景
		break;
	case 2:
		gameOver_->Draw2DFar();
		break;
	}

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
	
	// 各シーン3D描画
	switch (sceneMode_) {
	case 1:
		break;
	case 0:
		gamePlay_->Draw3D(); // ゲームプレイ3D表示
		break;
	case 2:
		gameOver_->Draw3D();
		break;
	}
	
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	
	// 各シーン2D前景描画
	switch (sceneMode_) {
	case 1:
		title_->Draw2DNear();
		break;
	case 0:
		gamePlay_->Draw2DNear(); // ゲームプレイ表示2D近景
		break;
	case 2:
		gameOver_->Draw2DNear();
		break;
	}

	/// </summary>[^

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}