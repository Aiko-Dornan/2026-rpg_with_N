#include"OutRangeEnemy.h"
#include"function.h"
#include"spawnPoint.h"
#include "CollisionUpdate.h"
#include"expitem.h"
#include"player.h"
#include <cmath>

COutRangeEnemy::COutRangeEnemy(Point p, CMap* _map, CSpawnPoint* _sp)
{
	sprite.img = Resource::octrock_img;
	sprite.cutX = 0;
	sprite.cutY = 0;

	sprite.width = ImgWidth = 32;
	sprite.height = ImgHeight = 32;

	map = _map;

	pos = p;

	HP(2);//HP初期化

	MoveSpeed(10.0f);//移動速度初期化

	//スポーン位置保存
	spawn = _sp;

	ID = (int)ObjID::ENEMY;
	pri = 2;
}

int COutRangeEnemy::Action(const ObjList& base, ObjList& add_base)
{
	//無敵時間
	if (damageCoolTime > 0)
	{
		damageCoolTime--;
	}

	if (hp <= 0)
	{
		FLAG = false;
		spawn->enemyAlive = false;
		add_base.push_back(make_unique<CItemExp>(pos, map));
	}

	// プレイヤーを探す
	CPlayer* player = nullptr;

	for (int i = 0; i < base.size(); i++)
	{
		BaseVector* obj = base[i].get();

		if (obj->ID == (int)ObjID::PLAYER)
		{
			player = dynamic_cast<CPlayer*>(obj);
			break;
		}
	}

	if (player == nullptr)
		return 0;

	// プレイヤーとの距離
	float dx = player->pos.x - pos.x;
	float dy = player->pos.y - pos.y;

	float distance = std::sqrt(dx * dx + dy * dy);

	if (distance > 500.0f)
	{
		//vec.x = (dx / distance) * moveSpeed;
		//vec.y = (dy / distance) * moveSpeed;
	}
	else
	{
		if (player == nullptr)
			return 0;
	}

	pos.x += vec.x;
	pos.y += vec.y;


	return 0;
}

void COutRangeEnemy::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);
}
