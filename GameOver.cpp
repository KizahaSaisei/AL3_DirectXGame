#include "GameOver.h"

// コンストラクタ
GameOver::GameOver() {}

// デストラクト
GameOver::~GameOver() 
{
	delete gamePlay_;
}

// 初期化
void GameOver::Intialize(ViewProjection viewProjection) {
	// メンバー変数に代入
	viewProjection_ = viewProjection;

	// インプットクラス
	input_ = Input::GetInstance();

	// BG(2Dスプライト）
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});
	// Hit Enter Key(2Dスプライト）
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 520});

	gamePlay_ = new GamePlay();

	gamePlay_->Intialize(viewProjection_);
}

void GameOver::End() {  }

// 更新
int GameOver::Update()
{
	timer_ += 1;
	gamePlay_->Update();

	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN) && gamePlay_->Next() == 0) {
		gamePlay_->Start();
		// ゲームタイトルへ移行
		return 1;
	}
	// ゲームオーバーを継続
	return 2;
}

// ゲームオーバー表示2D背景
void GameOver::Draw2DFar() 
{
	gamePlay_->Draw2DFar();	
}

// 3D表示
void GameOver::Draw3D() 
{
	gamePlay_->Draw3D();
}

// ゲームオーバー表示2D近景
void GameOver::Draw2DNear()
{
	if (gamePlay_->Next() == 0) {
		// 背景
		spriteGameOver_->Draw();

		// エンター表示
		if (timer_ % 40 >= 20) {
			spriteEnter_->Draw();
		}
	}
	gamePlay_->Draw2DNear();
}