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

	//攻撃中フレーム
	int att_flame{ 0 };

	//経験値
	int exp{ 0 };

	//HP
	int player_hp{5};

};