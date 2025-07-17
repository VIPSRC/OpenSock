#ifndef STRUCT_H
#define STRUCT_H

#include "support.h"
#include "init.h"
#define maxplayerCount 100
#define maxvehicleCount 50
#define maxitemsCount 400
#define maxgrenadeCount 10
#define maxboxitemsCount 150

struct Actors {
    uint64_t Enc_1, Enc_2;
    uint64_t Enc_3, Enc_4;
};

struct Chunk {
    uint32_t val_1, val_2, val_3, val_4;
    uint32_t val_5, val_6, val_7, val_8;
};

struct PlayerBone {
    bool isBone = false;
    Vec2 head;
    Vec2 neck;
    Vec2 cheast;
    Vec2 pelvis;
    Vec2 lSh;
    Vec2 rSh;
    Vec2 lElb;
    Vec2 rElb;
    Vec2 lWr;
    Vec2 rWr;
    Vec2 lTh;
    Vec2 rTh;
    Vec2 lKn;
    Vec2 rKn;
    Vec2 lAn;
    Vec2 rAn;
    Vec2 root;
};

Vec4 getPrecise(PlayerBone b, Vec2 head, Vec2 root)
{
    float minX = 3000.0f;
    float minY = 3000.0f;
    float maxX = 0.0f;
    float maxY = 0.0f;

    float x[40];
    PlayerBone n = b;

    int iter = 0;
    x[30] = root.X;
    x[31] = root.Y;
    x[32] = head.X;
    x[33] = head.Y;
    memcpy(&x, &n.neck, 120);

    for (int k = 0; k < 17; k++)
    {
        minX = std::min(minX, x[iter]);
        minY = std::min(minY, x[iter + 1]);
        maxX = std::max(maxX, x[iter]);
        maxY = std::max(maxY, x[iter + 1]);
        iter += 2;
    }

    return { minX - 5.0f, minY - 5.0f, maxX + 5.0f, maxY + 5.0f };
}
struct PlayerWeapon {
    bool isWeapon = false;
    int id;
    int ammo;
    int maxammo;
};

enum Mode {
    InitMode = 1,
    ESPMode = 2,
    HackMode = 3,
    StopMode = 4,
};

struct Options {
    int aimbotmode;
    int openState;
    bool tracingStatus;
    int priority;
    bool pour;
    int aimingRange;
    int wideView;
    bool isMetroMode;
    bool isRadar;
};

struct Memory {
    bool LessRecoil;
    bool ZeroRecoil;
    bool InstantHit;
    bool SmallCrosshair;
    bool FastParachute;
    bool NoShake;
    bool HitEffect;
    bool Aimbot;
};

struct Request {
    int Mode;
    Options options;
    Memory memory;
    int ScreenWidth;
    int ScreenHeight;
};

struct VehicleData {
    char VehicleName[50];
    float Distance;
    float Fuel;
    float Health;
    Vec2 Location;
};

struct ItemData {
    char ItemName[50];
    float Distance;
    Vec2 Location;
};

struct GrenadeData {
    int type;
    float Distance;
    Vec2 Location;
};

struct BoxItemData {
    int ItemType;
    int ItemID[40];
    int ItemCount;
    int Count[40];
    Vec2 Location;
};

struct PlayerData {
    char PlayerNameByte[100];
    int TeamID;
    float Health;
    float Distance;
    bool isBot;
    bool isKnocked;
    Vec4 Precise;
    Vec2 Location;
    Vec2 RadarLocation;
    PlayerWeapon Weapon;
    PlayerBone Bone;
};

struct Response {
    bool Success;
	bool Identified;
    int PlayerCount;
    int VehicleCount;
    int ItemsCount;
    int BoxItemsCount;
    int GrenadeCount;
    PlayerData Players[maxplayerCount];
    VehicleData Vehicles[maxvehicleCount];
    ItemData Items[maxitemsCount];
    BoxItemData BoxItems[maxboxitemsCount];
    GrenadeData Grenade[maxgrenadeCount];
};

#endif
