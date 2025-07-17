#ifndef OFFSETS_H
#define OFFSETS_H

// Offset Finder Tool By Starkskillz

namespace Offsets {
    uintptr_t GNames;
    uintptr_t ViewMatrix;

    //Class: ShootWeaponEntity
    uintptr_t BulletFireSpeed; //float BulletFireSpeed;
	uintptr_t ShootInterval; //float ShootInterval;
	uintptr_t AutoAimingConfig; //AutoAimingConfig AutoAimingConfig;
	uintptr_t AccessoriesVRecoilFactor; //float AccessoriesVRecoilFactor;
	uintptr_t AccessoriesHRecoilFactor; //float AccessoriesHRecoilFactor;
	uintptr_t AccessoriesRecoveryFactor; //float AccessoriesRecoveryFactor;
	uintptr_t GameDeviationFactor; //float GameDeviationFactor;
	uintptr_t RecoilKickADS; //float RecoilKickADS;
	uintptr_t ExtraHitPerformScale; //float ExtraHitPerformScale;
	uintptr_t AnimationKick; //float AnimationKick;

	//Class: CharacterParachuteComponent
	uintptr_t CurrentFallSpeed; //float CurrentFallSpeed;
    
    //Class: PickUpListWrapperActor
    uintptr_t PickUpDataList; //PickUpItemData[] PickUpDataList;
    uintptr_t PickUpDataListCount; //PickUpItemData[] PickUpDataList; + 0x8

    //Class: STExtraVehicleBase
    uintptr_t VehicleCommon; //VehicleCommonComponent* VehicleCommon;

	//Class: VehicleCommonComponent
	uintptr_t VHealthMax; //float HPMax;
	uintptr_t VHealth; //float HP;
	uintptr_t VFuelMax; //float FuelMax;
	uintptr_t VFuel; //float Fuel;

    void PUBGMOBILE() {
        GNames = 0xc7d2e70;
        ViewMatrix = 0xce93a70;
        
        GameDeviationFactor = 0xb90;
        RecoilKickADS = 0xc48;
        ExtraHitPerformScale = 0xc4c;
        AnimationKick = 0xc64;
        
        CurrentFallSpeed = 0x1cc;
        
        PickUpDataList = 0x880;
        PickUpDataListCount = 0x880 + 0x8;
        
        VehicleCommon = 0xa28;
        
        VHealthMax = 0x2c0;
        VHealth = 0x2c4;
        VFuelMax = 0x334;
        VFuel = 0x33c;
    }

    void BattlegroundsIndia() {
        GNames = 0xC443e80;
        ViewMatrix = 0xcb15820;
        
        LocalUID = 0x908;
        LocalTeamID = 0x928;
        
        BreathMax = 0x16c;
        
        STPlayerController = 0x3ec8;
        
        CurBulletNumInClip = 0xea0;
        CurMaxBulletNumInOneClip = 0xec0;
        ShootWeaponEntityComp = 0xff8;
        
        CurrWeapon = 0x500;
        
        UploadInterval = 0x178;
        
        BulletFireSpeed = 0x4f8;
        ShootInterval = 0x530;
        AutoAimingConfig = 0x990;
        AccessoriesVRecoilFactor = 0xb18;
        AccessoriesHRecoilFactor = 0xb1c;
        AccessoriesRecoveryFactor = 0xb20;
        GameDeviationFactor = 0xb90;
        RecoilKickADS = 0xc48;
        ExtraHitPerformScale = 0xc4c;
        AnimationKick = 0xc64;
        
        CurrentFallSpeed = 0x1cc;
        
        PickUpDataList = 0x8a0;
        PickUpDataListCount = 0x8a0 + 0x8;
        
        VehicleCommon = 0xa18;
        
        VHealthMax = 0x2c0;
        VHealth = 0x2c4;
        VFuelMax = 0x334;
        VFuel = 0x338;
    }
}

#endif
