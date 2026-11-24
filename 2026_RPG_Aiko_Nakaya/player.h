#pragma once
#include"objBase.h"
#include"map.h"
#include"damageObj.h"

class CPlayer :public CDamageObj
{
public:
	CPlayer(CMap*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//リスポーン処理
	void Respawn();

	//攻撃中フレーム
	int att_flame{ 0 };

	//経験値
	int exp{ 0 };

	//HP
	int player_hp{5};

	//敵消去
	void DeleteAllEnemy(const ObjList&);

private:
	Point stratPos{ 208,308 };
};