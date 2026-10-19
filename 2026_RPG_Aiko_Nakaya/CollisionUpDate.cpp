//当たり判定関数
#include "CollisionUpdate.h"
#include"player.h"
#include"sword.h"
#include"expitem.h"

//当たり判定関数
//この関数を使用する場合、baseに登録しているオブジェクトの順番についてソートが必要
// 使用する場合の注意
//　①priの値を設定して、オブジェクトは、必ずソートしていること
//　②判定の条件を描く場合、aオブジェクトのpriが必ずbオブジェクトより小さくなるように指定すること
void CollisionUpDate(ObjList& base) {
	for (int i = 0; i < base.size(); i++) {
		BaseVector* a = base[i].get();
		for(int j = i + 1; j < base.size(); j++) 
		{
			BaseVector* b = base[j].get();

			if (a->ID>b->ID)
			{
				swap(a, b);
			}

			//ID順を統一
			//if(a==b)とif(b==a)は同じオブジェクトの判定になるので、ID順を統一する

			if (a->ID==(int)ObjID::PLAYER&&b->ID==(int)ObjID::ENEMY)
			{
				//CPlayer* player = dynamic_cast<CPlayer>(a);
				Player_Enemy_Collision(a, b);

			}

			//プレイヤーと敵の弾
			if (a->ID == (int)ObjID::PLAYER && b->ID == (int)ObjID::ENEMYBULLET)
			{
				//CPlayer* player = dynamic_cast<CPlayer>(a);
				Player_EnemyBullet_Collision(a, b);

			}

			if (a->ID == (int)ObjID::ENEMY && b->ID == (int)ObjID::SWORD)
			{
				//CPlayer* player = dynamic_cast<CPlayer>(a);
				Enemy_Sword_Collision(a, b);

			}
			//プレイヤーとアイテム
			if (a->ID == (int)ObjID::PLAYER && b->ID == (int)ObjID::ITEM)
			{
				//CPlayer* player = dynamic_cast<CPlayer>(a);
				Player_Item_Collision(a, b);

			}
			

		}
	}
}

void Player_Enemy_Collision(BaseVector* a, BaseVector* b)
{
	if (HitCheck_Box2(a, b))
	{
		//a->FLAG = false;
		CDamageObj* player = dynamic_cast<CDamageObj*>(a);

		Vector v{
		a->pos.x - b->pos.x,
		a->pos.y - b->pos.y
		};
		v = Vector_SetLength(v, 15.0f);
		//player->KnockBack(v);
		player->Damage(1, v);

		

	}
}

//プレイヤーと敵の弾
void Player_EnemyBullet_Collision(BaseVector* a, BaseVector* b)
{
	if (HitCheck_Box2(a, b))
	{
		//a->FLAG = false;
		CDamageObj* player = dynamic_cast<CDamageObj*>(a);

		Vector v{
		a->pos.x - b->pos.x,
		a->pos.y - b->pos.y
		};
		v = Vector_SetLength(v, 15.0f);
		//player->KnockBack(v);
		player->Damage(1, v);



	}
}

//敵と剣
void Enemy_Sword_Collision(BaseVector* a, BaseVector* b)
{


	//敵を円、剣を線分として判定を行う。

	//敵の当たり判定用半径
	float enemyRadius = 16;

	//剣の始点と終点を求め、剣の線分を求める。

	//剣オブジェクトを取得
	CSword* s = dynamic_cast<CSword*>(b);

	//剣の始点
	Point swordStart{ s->pos.x,s->pos.y };

	//剣の角度と長さから終点を計算

	Point swordEnd{
		s->pos.x + cos(s->sprite.angle) * s->length,
		s->pos.y + sin(s->sprite.angle) * s->length
	};

	//敵の中心から剣の線分までの最短距離を求める。

	//敵の中心から、剣の線分上で最も近い点を取得
	Point lp = Position_Closest_Line(a->pos, swordStart, swordEnd);

	//求めた点と敵の円判定

	float dx = a->pos.x - lp.x;
	float dy = a->pos.y - lp.y;
	float dist = sqrt(dx * dx + dy * dy);

	//敵の中心から剣までの距離が半径以内なら当たり
	if (dist<=enemyRadius)
	{
		//敵を削除
		CDamageObj* enemy = dynamic_cast<CDamageObj*>(a);
		//ノックバックするベクトル(剣の最近点と敵の中心)
		Vector v
		{
		enemy->pos.x - s->pos.x,
		enemy->pos.y - s->pos.y
		};
		v = Vector_SetLength(v, 15);
		enemy->Damage(1,v);

	}

}

//プレイヤーとアイテム
void Player_Item_Collision(BaseVector* a, BaseVector* b)
{
	if (HitCheck_Box2(a,b))
	{
		//アイテムオブジェクトにキャスト
		CItem* item = dynamic_cast<CItem*>(b);
		//アイテムにゲット処理
		item->GetItem(a);

	}


}