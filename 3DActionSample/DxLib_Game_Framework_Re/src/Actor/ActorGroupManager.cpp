#include "ActorGroupManager.h"

// クラス：アクターグループ管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// グループの追加
void ActorGroupManager::add_group(ActorGroup group)
{
	actor_group_map_[group].clear();
}

// アクターの追加
void ActorGroupManager::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actor_group_map_[group].add(actor);
}

// 更新
void ActorGroupManager::update(float delta_time)
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.update(delta_time);
	}
}

// 描画
void ActorGroupManager::draw() const
{
	for (auto& pair : actor_group_map_)
	{
		draw(pair.first);
	}
}

// 描画
void ActorGroupManager::draw(ActorGroup group) const
{
	actor_group_map_.at(group).draw();
}

// 消去
void ActorGroupManager::clear()
{
	actor_group_map_.clear();
}

// アクターの取得
ActorPtr ActorGroupManager::find(ActorGroup group, const std::string& name) const
{
	return actor_group_map_.at(group).find(name);
}

// アクター数の取得
unsigned int ActorGroupManager::count(ActorGroup group) const
{
	return actor_group_map_.at(group).count();
}

// アクターリストの巡回
void ActorGroupManager::each(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actor_group_map_.at(group).each(fn);
}

// 衝突判定
void ActorGroupManager::collide(ActorGroup group1, ActorGroup group2)
{
	actor_group_map_[group1].collide(actor_group_map_[group2]);
}

// 同じグループ内の衝突判定
void ActorGroupManager::collide(ActorGroup group)
{
	actor_group_map_[group].collide();
}

// 削除
void ActorGroupManager::remove()
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.remove();
	}
}

// メッセージ処理
void ActorGroupManager::handle_message(EventMessage message, void* param)
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.handle_message(message, param);
	}
}