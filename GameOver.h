#pragma once
#include "DirectXCommon.h"
#include "MathUtilityforText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "GamePlay.h"
#include "DebugText.h"

class GameOver {
public:
	// コンストラクタ
	GameOver();

	// デストラクト
	~GameOver();

	// 初期化
	void Intialize(ViewProjection viewProjection_);

	// 初期化処理
	void End();

	// 更新
	int Update();

	// ゲームオーバー表示2D背景
	void Draw2DFar();

	// 3D表示
	void Draw3D();

	// ゲームオーバー表示2D近景
	void Draw2DNear();

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	//// 各クラス
	//Stage* stage_ = nullptr;   // ステージ
	//Player* player_ = nullptr; // プレイヤー
	//Beam* beam_ = nullptr;     // ビーム
	//Enemy* enemy_ = nullptr;   // ビーム
	GamePlay* gamePlay_ = nullptr;   // ビーム

	//// デバッグテキスト
	//DebugText* debugText_ = nullptr;

	// インプットクラス
	Input* input_ = nullptr;

	int timer_ = 0;
	
	// GameOver
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	// Hit Enter Key（スプライト）
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	//int gameScore_; // ゲームスコア

	//int playerLife_; // プレイヤーライフ
};
