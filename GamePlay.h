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
#include "DebugText.h"

class GamePlay {
public:
	// コンストラクタ
	GamePlay();

	// デストラクト
	~GamePlay();

	// 初期化
	void Intialize(ViewProjection viewProjection_);
	
	// 初期化処理
	void Start();


	// 更新
	int Update();
	//// 更新
	//void Update();

	// ゲームプレイ表示2D背景
	void Draw2DFar();

	// 3D表示
	void Draw3D();

	// ゲームプレイ表示2D近景
	void Draw2DNear();

	// 射撃
	void Shot();

	int Next() { return playerLife_; }

private:
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// 各クラス
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beamTable_[10] = {}; // ビーム
	Enemy* enemyTable_[10] = {}; // 敵

	// 衝突判定（ビームと敵）
	void CollisionBeamEnemy();
	// 衝突判定（プレイヤーと敵）
	void CollisionPlayerEnemy();

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	int gameScore_ = 0; // ゲームスコア

	int playerLife_ = 3; // プレイヤーライフ
	int timer_ = 0;
	float x = 0.0f;
	float z = 0.0f;
	// インプットクラス
	Input* input_ = nullptr;

	int shotTimer_[10] = {0};
};