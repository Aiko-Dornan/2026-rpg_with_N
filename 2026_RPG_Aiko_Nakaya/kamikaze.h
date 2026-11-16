#pragma once

#include"damageObj.h"
#include"map.h"

class CSpawnPoint;

class CKamikaze : public CDamageObj
{
private:

    //向き
    enum DIR { DOWN, LEFT, RIGHT, UP };;

    CMap* map = nullptr;
    CSpawnPoint* spawn = nullptr;
    
    float moveSpeed = 4.0f;

   
    CDamageObj* target = nullptr;//追尾するターゲット

public:
    CKamikaze(Point p, CMap* _map, CSpawnPoint* _sp);

    int Action(const ObjList& base, ObjList& add_base) override;
    void Draw() override;

    void UpdateDir(Point target);//方向
};