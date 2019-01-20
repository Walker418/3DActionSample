#include "Player.h"
#include "GamePad.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
}

// 更新
void Player::update(float delta_time)
{
	/*
	// モーションの切り替えテスト
	if (GamePad::trigger(GamePad::Up))
		motion_ = (motion_ + 1) % 25;
	if (GamePad::trigger(GamePad::Down))
		motion_ = ((motion_ - 1) + 25) % 25;

	// モーションを変更
	mesh_.change_motion(motion_);
	// アニメーションを更新
	mesh_.update(delta_time);
	// 行列を設定
	mesh_.transform(pose());
	*/

	// プレーヤーの状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
	}
}

// 描画
void Player::draw() const
{
	mesh_.draw();							// メッシュを描画
	body_->translate(position_)->draw();	// コライダーを描画（デバッグモードのみ、調整用）
}

// 衝突リアクション
void Player::react(Actor& other)
{
	
}

// メッセージ処理
void Player::handle_message(EventMessage message, void* param)
{

}

// 状態の更新
void Player::update_state(float delta_time)
{
	// 現在の状態に応じて、プレイヤーを更新
	switch (state_)
	{
	case PlayerState::Normal:
		normal(delta_time);
		break;
	case PlayerState::Slash1:
		slash1(delta_time);
		break;
	case PlayerState::Slash2:
		slash2(delta_time);
		break;
	case PlayerState::Slash3:
		slash3(delta_time);
		break;
	case PlayerState::Damage:
		damage(delta_time);
		break;
	case PlayerState::GuardBegin:
		guard(delta_time);
		break;
	case PlayerState::Blocking:
		blocking(delta_time);
		break;
	case PlayerState::GuardAttack:
		guard_attack(delta_time);
		break;
	case PlayerState::GuardEnd:
		guard_end(delta_time);
		break;
	case PlayerState::Death:
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーの加算
}

// 状態の変更
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// 通常状態での更新
void Player::normal(float delta_time)
{
	// Xキーが押されると、攻撃する
	if (GamePad::trigger(GamePad::X))
	{
		// 攻撃状態に移行
		change_state(PlayerState::Slash1, MOTION_SLASH_1);

		return;
	}

	// ============================================================
	// 以下は移動処理
	int motion{ MOTION_IDLE };		// 何もしなければ、待機モーションに変更
	velocity_ = Vector3::Zero;		// 移動量をリセット
	float forward_speed{ 0.0f };	// 前向き速度
	float left_speed{ 0.0f };		// 左向き速度

	// 前後移動
	if (GamePad::state(GamePad::Up))
	{
		forward_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Down))
	{
		forward_speed = -WalkSpeed;
	}
	// 左右移動
	if (GamePad::state(GamePad::Left))
	{
		left_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Right))
	{
		left_speed = -WalkSpeed;
	}
	// 移動していれば、歩行モーションに変更
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_WALK;

	// 状態を更新
	change_state(PlayerState::Normal, motion);
	// 移動量を計算し、プレイヤーを移動させる
	velocity_ += rotation_.Forward() * forward_speed;
	velocity_ += rotation_.Left() * left_speed;
	position_ += velocity_ * delta_time;

	// 移動処理終了
	// ============================================================
}

// 攻撃（1段目）での更新
void Player::slash1(float delta_time)
{
	// 


	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// 攻撃（2段目）での更新
void Player::slash2(float delta_time)
{


	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// 攻撃（3段目）での更新
void Player::slash3(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// 被弾状態での更新
void Player::damage(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// ガード開始時の更新
void Player::guard(float delta_time)
{

}

// ガード時の更新
void Player::blocking(float delta_time)
{

}

// ガード攻撃での更新
void Player::guard_attack(float delta_time)
{
	// モーション終了後、ガード状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		guard(delta_time);
	}
}

// ガード終了時の更新
void Player::guard_end(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// 死亡状態での更新
void Player::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time())
	{
		die();
	}
}