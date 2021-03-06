#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"
#include "../../Graphic/Graphics2D.h"
#include "../../ID/SourceID.h"
#include "../../Math/MathHelper.h"
#include "../../Sound/Sound.h"
#include "../../Actor/Player/PlayerInput.h"
#include "../../Game/WindowSetting.h"

// クラス：ゲームプレイシーン管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ============================================================
// 以下は画像スプライトの関連定数
// ============================================================

const float PlayerHPGaugePosX = 20.0f;	// プレイヤー体力ゲージのx軸座標
const float PlayerHPGaugePosY = 15.0f;	// プレイヤー体力ゲージのy軸座標
const int	PlayerHPDot = 4;			// プレイヤーHPの幅

const int	P1MessageWidth = 250;		// 雑魚戦メッセージの幅
const int	P2MessageWidth = 250;		// ボス戦メッセージの幅
const float MessagePosY = 430.0f;		// ステージ目的表示のy軸座標

const int	BossHPGaugeWidth = 590;		// ボス体力ゲージの幅
const int	BossHPGaugeHeight = 144;	// ボス体力ゲージの高さ
const float	BossHPGaugePosY = 790.0f;	// ボス体力ゲージ表示のy軸座標
const int	BossHPDot = 5;				// ボスHPの幅

// ============================================================

// コンストラクタ
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	phase_{ GamePlayPhase::Phase1 },
	enemy_defeated_{ 0 },
	boss_defeated_{ false },
	phase2_end_{ false },
	player_dead_{ false },
	game_end_{ false },
	fade_counter_{ 0 }
{
	// ゲーム開始処理を行う
	game_start();
}

// 更新
void GamePlayManager::update(float delta_time)
{
	update_phase(delta_time);	// プレイ状況を更新

	// フェイドイン/フェイドアウト用カウンターの値を制限
	fade_counter_ = (int)MathHelper::clamp((float)fade_counter_, 0.0f, 255.0f);
}

// 描画
void GamePlayManager::draw() const
{
	// 描画輝度をセットし、フェイドイン/フェイドアウト演出をする
	SetDrawBright(fade_counter_, fade_counter_, fade_counter_);

	// プレイヤーキャラの体力を表示
	draw_playerHP();

	// 現在の目的を表示
	draw_message();

	// ボスの体力を表示
	if (world_->find_actor(ActorGroup::Enemy, "DragonBoar") != nullptr)
		draw_bossHP();
}

// メッセージ処理
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	// 受け取ったメッセージの種類によって、処理を行う

	switch (message)
	{
	case EventMessage::EnemyDead:	// 雑魚敵が倒された場合
		++enemy_defeated_;			// 倒された敵の数を加算
		break;
	case EventMessage::BossDead:	// ボス敵が倒された場合
		boss_defeated_ = true;
		break;
	case EventMessage::PlayerDead:	// プレイヤーが倒れた場合
		player_dead_ = true;
		break;
	default:
		break;
	}
}

// プレイ状況の更新
void GamePlayManager::update_phase(float delta_time)
{
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		phase1(delta_time);
		break;
	case (GamePlayPhase::Phase2):
		phase2(delta_time);
		break;
	default:
		break;
	}

	// プレイヤーが死亡すると、しばらくしてゲームオーバーメッセージを送る（ゲームオーバー処理を行う）
	if (player_dead_)
	{
		// BGM再生中止
		Sound::stop_bgm();
		// ゲームオーバーメッセージを送る（ゲームオーバー処理を行う）
		if (gameover_scene_timer_.is_time_out())
		{
			world_->send_message(EventMessage::GameOver);
		}

		gameover_scene_timer_.update(delta_time);
	}
}

// ゲーム開始処理
void GamePlayManager::game_start()
{
	// 各種タイマーをリセット
	phase_change_timer_.reset();
	gameover_scene_timer_.reset();
	gameclear_scene_timer_.reset();
	// 雑魚敵3体を生成する
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 60.0f, 0.0f, -35.0f }, 160.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ -60.0f, 0.0f, -35.0f }, 200.0f));
	
	// BGM再生開始
	Sound::play_bgm(BGM_STAGE);
}

// フェーズ移行
void GamePlayManager::change_phase()
{
	// ボスを生成し、ボス戦に移行する
	Sound::play_bgm(BGM_BOSS);	// ボス戦BGMを再生
	world_->add_actor(ActorGroup::Enemy, new_actor<DragonBoar>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	phase_ = GamePlayPhase::Phase2;
}

// 第1段階の処理
void GamePlayManager::phase1(float delta_time)
{
	// ゲーム開始時のフェードイン
	if (fade_counter_ < 255) fade_counter_ += 4;

	if (phase1_end())
	{
		Sound::stop_bgm();		// ステージBGM再生中止
		if (phase_change_timer_.is_time_out())
		{
			change_phase();		// ボス戦に移行
			return;
		}

		phase_change_timer_.update(delta_time);
	}

	// Tキーが押されたら、雑魚戦をスキップしてボス戦に移行
	if (PlayerInput::skip_phase1())
		skip_phase1();
}

// 第2段階の処理
void GamePlayManager::phase2(float delta_time)
{
	if (boss_defeated_)
	{
		// BGM再生中止
		Sound::stop_bgm();
		if (gameclear_scene_timer_.is_time_out())
		{
			// ゲームクリアメッセージを送る（ゲームクリア処理を行う）
			world_->send_message(EventMessage::StageClear);
			return;
		}

		gameclear_scene_timer_.update(delta_time);
	}
}

// プレイヤーの体力ゲージを表示
void GamePlayManager::draw_playerHP() const
{
	Vector2 pos{ PlayerHPGaugePosX, PlayerHPGaugePosY };	// 描画する座標

	Graphics2D::draw(TEXTURE_PLAYER_HPGAUGE, pos);			// 体力ゲージを描画

	// プレイヤーの体力を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	int player_hp = (player != nullptr) ? player->get_HP() : 0;

	// ゲージの中身を描画
	if (player_hp > 0)
	{
		Vector2 draw_pos = pos + Vector2(70.0f, 24.0f);	// 最初の描画位置

		for (int i = 0; i < player_hp; ++i)
		{
			Graphics2D::draw(TEXTURE_PLAYER_HP, Vector2(draw_pos.x + PlayerHPDot * i, draw_pos.y));
		}
	}
}

// 現在の目的を表示
void GamePlayManager::draw_message() const
{
	Vector2 pos;

	// 進行状況に応じて、現在の目的を表示
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		pos = Vector2(WindowSetting::WindowWidth - P1MessageWidth, MessagePosY);
		Graphics2D::draw(TEXTURE_P1MESSAGE, pos);
		break;
	case (GamePlayPhase::Phase2):
		pos = Vector2(WindowSetting::WindowWidth - P2MessageWidth, MessagePosY);
		Graphics2D::draw(TEXTURE_P2MESSAGE, pos);
		break;
	default:
		break;
	}
}

// ボスの体力ゲージを表示
void GamePlayManager::draw_bossHP() const
{
	int win_center = WindowSetting::WindowWidth / 2;	// ウィンドウの中心点
	int gauge_half = BossHPGaugeWidth / 2;				// ボス体力ゲージの幅の半分
	int gauge_bottom = WindowSetting::WindowHeight - BossHPGaugeHeight;

	Vector2 pos((float)(win_center - gauge_half), (float)gauge_bottom);
	Graphics2D::draw(TEXTURE_BOSS_HPGAUGE, pos);		// 体力ゲージを描画

	// ボスの体力を取得
	auto boss = world_->find_actor(ActorGroup::Enemy, "DragonBoar");
	int boss_hp = (boss != nullptr) ? boss->get_HP() : 0;

	// ゲージの中身を描画
	if (boss_hp > 0)
	{
		Vector2 draw_pos = pos + Vector2(86.0f, 56.0f);	// 最初の描画位置

		for (int i = 0; i < boss_hp; ++i)
		{
			Graphics2D::draw(TEXTURE_BOSS_HP, Vector2(draw_pos.x + BossHPDot * i, draw_pos.y));
		}
	}
}

// 第1段階は終了しているか
bool GamePlayManager::phase1_end() const
{
	// 雑魚敵が全部倒されたら、Trueを返す
	return enemy_defeated_ >= EnemyPopNo;
}

// 第1段階のスキップ
void GamePlayManager::skip_phase1()
{
	// フィールド上の全ての雑魚敵を消去
	world_->send_message(EventMessage::EnemyDestroy);
}