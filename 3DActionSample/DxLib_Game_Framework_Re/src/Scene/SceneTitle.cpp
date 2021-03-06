#include "SceneTitle.h"
#include "Scene.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Camera/TitleCamera.h"
#include "../Graphic/Light.h"
#include "../Actor/Player/PlayerInput.h"
#include "../Graphic/Graphics2D.h"
#include "../Math/MathHelper.h"
#include "../Sound/Sound.h"
#include "../Game/WindowSetting.h"

// ============================================================
// 以下は画像スプライトの関連定数
// ============================================================

const int LogoWidth = 800;		// タイトルロゴの幅
const int LogoPosY = 80;		// タイトルロゴのy軸座標

const int StartWidth = 444;		// スタートメッセージの幅
const int StartPosY = 490;		// スタートメッセージのy軸座標

// ============================================================

// 開始
void SceneTitle::start()
{
	is_end_ = false;			// 終了フラグをFalseにする
	world_.initialize();		// ワールドを初期化

	is_started_ = false;		// スタートボタンの状況を初期化
	show_start_ = false;
	show_start_timer_ = 0.0f;
	end_timer_ = 0.0f;
	fade_counter_ = 0;

	// フィールドを追加
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// カメラを追加
	world_.add_camera(new_actor<TitleCamera>(&world_, Vector3::Zero));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// BGM再生開始
	Sound::play_bgm(BGM_TITLE);
}

// 更新
void SceneTitle::update(float delta_time)
{
	world_.update(delta_time);

	// フェイドイン/フェイドアウト用カウンターの値を制限
	fade_counter_ = (int)MathHelper::clamp((float)fade_counter_, 0.0f, 255.0f);

	// フェードイン完了まで、操作を受け付けない
	if (fade_counter_ < 255 && !is_started_)
	{
		fade_counter_ += 2;
		return;
	}

	// ゲーム開始
	if (PlayerInput::game_start() && !is_started_)
	{
		Sound::play_se(SE_START);	// SEを鳴らす
		is_started_ = true;
	}

	// スタートボタンが押されたら、画面が暗転し、プレイシーンに移行する
	if (is_started_)
	{
		// フェードアウト
		if (fade_counter_ > 0) fade_counter_ -= 2;

		if (end_timer_ >= 180.0f && fade_counter_ <= 0)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}

	// スタートメッセージの点滅制御
	// スペースキーが押されたら、激しく点滅する
	if (is_started_)
	{
		if (show_start_timer_ >= 5.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}
	// 普段は1秒ごと点滅
	else
	{
		if (show_start_timer_ >= 60.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}

	show_start_timer_ += delta_time;
}

// 描画
void SceneTitle::draw() const
{
	// 描画輝度をセットし、フェードイン/フェードアウト演出をする
	SetDrawBright(fade_counter_, fade_counter_, fade_counter_);

	world_.draw();	// 背景を描画

	draw_logo();	// タイトルロゴを描画
	draw_start();	// スタートメッセージを描画（点滅付き）
}

// 終了しているか
bool SceneTitle::is_end() const
{
	// 終了フラグを返す
	return is_end_;
}

// 次のシーンの取得
Scene SceneTitle::next() const
{
	return Scene::GamePlay;
}

// 終了
void SceneTitle::end()
{
	// BGM再生終了
	Sound::stop_bgm();
}

// メッセージ処理
void SceneTitle::handle_message(EventMessage message, void* param)
{

}

// タイトルロゴの描画
void SceneTitle::draw_logo() const
{
	// 画面の幅を取得し、タイトルロゴの表示座標を計算して描画する
	int window_width = WindowSetting::WindowWidth;						// 画面の幅
	float logo_pos_X = (float)window_width / 2 - (float)LogoWidth / 2;	// タイトルロゴのx軸座標

	Graphics2D::draw(TEXTURE_TITLELOGO, Vector2(logo_pos_X, (float)LogoPosY));
}

// スタートメッセージの描画
void SceneTitle::draw_start() const
{
	if (!show_start_) return;

	// 画面の幅を取得し、スタートメッセージの表示座標を計算して描画する
	int window_width = WindowSetting::WindowWidth;							// 画面の幅
	float start_pos_X = (float)window_width / 2 - (float)StartWidth / 2;	// スタートメッセージのx軸座標

	Graphics2D::draw(TEXTURE_START, Vector2(start_pos_X, (float)StartPosY));
}