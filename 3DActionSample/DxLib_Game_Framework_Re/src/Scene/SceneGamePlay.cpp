#include "SceneGamePlay.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../ID/EventMessage.h"
#include "../Actor/Camera/TPCamera.h"
#include "../Graphic/Light.h"
#include "../Actor/Player/Player.h"
#include "GamePlayScene/GamePlayManager.h"

#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"

#include "../Actor/Player/PlayerInput.h"
#include "../Game/WindowSetting.h"
#include "../Graphic/Graphics2D.h"
#include "../ID/SourceID.h"

// クラス：ゲームプレイシーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ============================================================
// 以下は画像スプライトの関連定数
// ============================================================

const int PauseTextWidth = 400;		// PAUSE文字画像の幅
const int PauseTextHeight = 160;	// PAUSE文字画像の高さ

// ============================================================

// 開始
void SceneGamePlay::start()
{
	// 終了フラグをfalseにする
	is_end_ = false;
	// ポーズ状態をfalseにする
	is_pause_ = false;
	// ワールドを初期化
	world_.initialize();

	// イベントメッセージリスナーを登録
	world_.add_event_message_listener([=](EventMessage message, void* param) {handle_message(message, param); });

	// フィールドを追加
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// カメラを追加
	world_.add_camera(new_actor<TPCamera>(&world_));
	// ライトを追加
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// アクターはここに追加
	world_.add_actor(ActorGroup::Player, new_actor<Player>(&world_, Vector3{ 0.0f, 0.0f, 100.0f }));	// プレイヤー
	world_.add_actor(ActorGroup::UI, new_actor<GamePlayManager>(&world_));								// ゲームプレイ管理者
}

// 更新
void SceneGamePlay::update(float delta_time)
{
	// ポーズしていなければ、ゲーム進行を更新
	if (!is_pause_)
		world_.update(delta_time);

	// ポーズ
	if (PlayerInput::pause())
		is_pause_ = (is_pause_) ? false : true;
}

// 描画
void SceneGamePlay::draw() const
{
	world_.draw();

	// ポーズ中にPAUSE画像を描画
	if (is_pause_)
	{
		Graphics2D::draw(TEXTURE_PAUSE_BG, Vector2::Zero);

		int win_width = WindowSetting::WindowWidth;
		int win_height = WindowSetting::WindowHeight;
		int text_posX = win_width / 2 - PauseTextWidth / 2;
		int text_posY = win_height / 2 - PauseTextHeight / 2;
		Vector2 pos{ (float)text_posX, (float)text_posY };
		Graphics2D::draw(TEXTURE_PAUSE_TEXT, pos);
	}
}

// 終了しているか
bool SceneGamePlay::is_end() const
{
	// 終了フラグを返す
	return is_end_;
}

// 次のシーンの取得
Scene SceneGamePlay::next() const
{
	return next_scene_;
}

// 終了
void SceneGamePlay::end()
{
	world_.clear();		// ワールドをクリア
}

// メッセージ処理
void SceneGamePlay::handle_message(EventMessage message, void* param)
{
	// 受け取ったメッセージの種類によって、処理を行う

	// ゲームオーバーの場合
	if (message == EventMessage::GameOver)
	{
		is_end_ = true;
		return;
	}

	// ゲームクリアの場合
	if (message == EventMessage::StageClear)
	{
		is_end_ = true;
		return;
	}
}