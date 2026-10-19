#include"octrock.h"
#include"function.h"
#include"spawnPoint.h"
#include"expitem.h"
#include"player.h"
#include"enemyBullet.h"

COctrock::COctrock(Point p, CMap* _map,CSpawnPoint* _sp)
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

int COctrock::Action(const ObjList& base, ObjList& add_base)
{
	//無敵時間
	if (damageCoolTime>0)
	{
		damageCoolTime--;
	}

	//HPがなくなった場合、オブジェクト削除
	if (hp<=0)
	{
		FLAG = false;
		spawn->enemyAlive = false;
		add_base.push_back(make_unique<CItemExp>(pos, map));
	}

	if (UpdateKnockBack())return 0;

	//ランダム移動
	if (MoveMentTime<0.0f)
	{
		TestMove();
		//移動方向からキャラクターの向きを取得
		
	}
	else
	{
		MoveMentTime--;
	}

	if (attackTime<0.0f)
	{
		Attack(add_base);
	}
	else
	{
		attackTime--;

	}


	return 0;



}

void COctrock::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);
}

void COctrock::TestMove()
{
	

	move_direction = GetRand(3);



	switch (move_direction)
	{
	case 0://右
		vec.x = move_speed;
		break;
	case 1://左
		vec.x = -move_speed;
		break;
	case 2://上
		vec.y = -move_speed;
		break;
	case 3://下
		vec.y = move_speed;
		break;
	default:
		break;
	}

	//移動ベクトル
	vec = Vector_SetLength(vec, 4.0f);
	//移動後の座標
	Point nextPos = pos;

	//x方向
	nextPos.x += vec.x;
	if (map->CanMove(nextPos, ImgWidth, ImgHeight))
		pos.x = nextPos.x;

	//y方向
	//現在の位置をもう一度コピー
	//x方向の移動結果を反映した位置からy方向を判定する
	nextPos = pos;
	nextPos.y += vec.y;
	if (map->CanMove(nextPos, ImgWidth, ImgHeight))
		pos.y = nextPos.y;

	Dir = GetDir(Dir);

	vec.x = 0.0f;
	vec.y = 0.0f;

	MoveMentTime = 10.0f;
}

void COctrock::Attack(ObjList& add_base)
{
	add_base.push_back(make_unique<CEnemyBullet>(pos,Dir, map));
	attackTime = 10.0f;

	//switch (move_direction)
	//{
	//case 0://右
	//	vec.x = move_speed;
	//	break;
	//case 1://左
	//	vec.x = -move_speed;
	//	break;
	//case 2://上
	//	vec.y = -move_speed;
	//	break;
	//case 3://下
	//	vec.y = move_speed;
	//	break;
	//default:
	//	break;
	//}

}