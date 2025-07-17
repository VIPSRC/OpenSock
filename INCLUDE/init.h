#ifndef XACODE_APP_H
#define XACODE_APP_H

    
    int myteamID = 101;
    int aimFor = 1;
    int aimBy = 1;
    int aimWhen = 1;
    uintptr_t cameraManager;
    char version[69];
	char var[32];
    char name[100];
    char weaponData[100];
    float aimRadius = 200;
	float aimDist;
    float recoil;
    float bSpeed;
    float RadarSize = 125;
    bool firing = false, ads = false, adsfiring = false;
    bool AimTriger;
    bool aimbot = false;
    bool aimKnoced = false;
    bool ignoreBot = false;
    bool isLessRecoil = false;
    bool isZeroRecoil = false;
    bool isInstantHit = false;
    bool isFastShootInterval = false;
    bool isSmallCrosshair = false;
    bool isHitX = false;
    bool isNoShake = false;
    bool isWideView = false;
    bool isAimbot = false;
    bool isFastWeapon = false;
#endif
