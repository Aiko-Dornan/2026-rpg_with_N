#include"slime.h"
#include"function.h"
#include"spawnPoint.h"
#include"expitem.h"
#include"player.h"
#include"enemyBullet.h"

CSlime::CSlime(Point p, CMap* _map, CSpawnPoint* _sp)
{
	sprite.img = Resource::Slime_img;
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

int CSlime::Action(const ObjList& base, ObjList& add_base)
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



	return 0;
}

void CSlime::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);
}
