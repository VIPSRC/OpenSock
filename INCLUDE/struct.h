#include "Support.h"
#include "init.h"
#include "Calculation.h"

#include <string>
#define maxplayerCount 100
#define maxvehicleCount 50
#define maxitemsCount 400
#define maxgrenadeCount 10
#define maxzonesCount 10
using namespace std;


struct PlayerBone {
	bool isBone = false;
	Vec3 neck;
    Vec3 cheast;
    Vec3 pelvis;
    Vec3 lSh;
    Vec3 rSh;
    Vec3 lElb;
    Vec3 rElb;
    Vec3 lWr;
    Vec3 rWr;
    Vec3 lTh;
    Vec3 rTh;
    Vec3 lKn;
    Vec3 rKn;
    Vec3 lAn;
    Vec3 rAn;
	Vec3 root;
};

struct PlayerWeapon {
    bool isWeapon=false;
    int id;
    int ammo;
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
    int aimingState;
    bool tracingStatus;
    int priority;
    bool pour;
    int aimingRange;
	int aimingDist;
    int recCompe;
    bool ignoreBot;
    int aimingSpeed;
	int lineTarget;
};

struct Memory {
    bool LessRecoil;
    bool ZeroRecoil;
    bool InstantHit;
    bool FastShootInterval;
    bool HitX;
    bool SmallCrosshair;
    bool NoShake;
    bool WideView;
    bool Aimbot;
    bool FastWeapon;
};

struct Request {
    int Mode;
    Options options;
    Memory memory;
    int ScreenWidth;
    int ScreenHeight;
	Vec2 radarPos;
    float radarSize;
};

struct VehicleData {
    char VehicleName[50];
    float Distance;
    float Health;
    float Fuel;
    Vec3 Location;
};


struct ItemData {
    char ItemName[50];
    float Distance;
    Vec3 Location;
};

struct GrenadeData {
    int type;
    float Distance;
    Vec3 Location;
};

struct ZoneData {
    float Distance;
    Vec3 Location;
};

struct PlayerData {
    char PlayerNameByte[100];
    int TeamID;
    float Health;
    float HealthMax;
    float HealthKnock;
    float Distance;
    bool isBot;
    Vec3 HeadLocation;
    Vec2 RadarLocation;
    PlayerWeapon Weapon;
    PlayerBone Bone;
};

struct Response {
    bool Success;
    bool InLobby;
    int PlayerCount;
    int VehicleCount;
    int ItemsCount;
    int GrenadeCount;
    int ZoneCount;
    float fov;
    PlayerData Players[maxplayerCount];
    VehicleData Vehicles[maxvehicleCount];
    ItemData Items[maxitemsCount];
    GrenadeData Grenade[maxgrenadeCount];
    ZoneData Zones[maxzonesCount];
};
