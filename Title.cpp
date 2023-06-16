#include "Title.h"

// コンストラクタ
Title::Title() {}

// デストラクト
Title::~Title() 
{ 
	delete spriteTitle_; // タイトル
	delete spriteEnter_; // Enter Key	
}

// 初期化
void Title::Intialize() {
	// インプットクラス
	input_ = Input::GetInstance();

	// BG(2Dスプライト）
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});
	// Hit Enter Key(2Dスプライト）
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 520});
}

// 更新
int Title::Update() {
	timer_+=1;

	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// ゲームプレイへ移行
		return 0;
	}

	// タイトルを継続
	return 1;
}

// ゲームプレイ表示2D近景
void Title::Draw2DNear() 
{
	// 背景
	spriteTitle_->Draw();
	
	// エンター表示
	if (timer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}