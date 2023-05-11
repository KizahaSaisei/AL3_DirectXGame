#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 　BG（スプライト）
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// 　Title BG（スプライト）
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	// 　Hit Enter Key（スプライト）
	uint32_t textureHandleKey_ = 0;
	Sprite* spriteKey_ = nullptr;

	// 　GameOver（スプライト）
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	// 　ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	// 　ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;
	// 　プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	void PlayerUpdate(); // プレイヤー更新

	// 　ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[10];

	void BeamUpdate(); // ビーム更新

	void BeamMove(); // ビーム移動

	void BeamBorn(); // ビーム発生

	int beamFlag_[10] = {}; // ビーム存在フラグ（０：存在しない、1：存在する）

	void Collision();            // 衝突判定
	void CollisionPlayerEnemy(); // 衝突判定（プレイヤーと敵）

	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];

	void EnemyUpdate(); // 敵更新
	void EnemyMove();   // 敵移動

	int enemyFlag_[10] = {}; // 敵存在フラグ（０：存在しない、1：存在する）

	void EnemyBorn(); // 敵発生

	void CollisionBeamEnemy(); // 衝突判定（ビームと敵）

	DebugText* debugText_ = nullptr;

	int gameScore_ = 0; // ゲームスコア

	int playerLife_ = 3; // プレイヤーライフ

	void TitleUpdate(); // タイトル更新
	void TitleDraw2DNear(); // タイトル背景2D表示

	void GameOverUpdate();
	void GameOverDraw2DBack();
	void GameOverDraw3D();
	void GameOverDraw2DNear();

	void GamePlayUpdate(); // ゲームプレイ更新
	void GamePlayDraw3D(); // ゲームプレ3D表示
	void GamePlayDraw2DBack(); // ゲームプレイ背景2D表示
	void GamePlayDraw2DNear(); // ゲームプレイ近景2D表示

	int sceneMode_ = 0; // シーンモード（0：ゲームプレイ　1：タイトル）

	int gameTimer_ = 0;

	int beamTimer_ = 0;

	float enemySpeed_[10] = {}; // 敵のスピード

	uint32_t soundDataHandleTitleBGM_ = 0;
	uint32_t soundDataHandleGamePlayBGM_ = 0;
	uint32_t soundDataHandleGameOverBGM_ = 0;
	uint32_t soundDataHandleEnemyHitSE_ = 0;
	uint32_t soundDataHandlePlayerHitSE_ = 0;
	uint32_t voiceHandleBGM_ = 0;
};
