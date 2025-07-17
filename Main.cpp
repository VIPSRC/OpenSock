#include "INCLUDE/struct.h"
#include "Offsets.h"

uintptr_t UE4;
uintptr_t GNames = 0, GWorld = 0;

void getGNames(uintptr_t actor, char *result)
{
    int ids = Reads<int>(actor + 0x18);
    uintptr_t gnames = Reads<uintptr_t>(Reads<uintptr_t>(UE4 + Offsets::GNames) + SIZE) + 0x110;
    int page = ids / 16384;
    int index = ids % 16384;
    if (page > 1 || page < 30)
    {
        uintptr_t a1 = Reads<uintptr_t>(gnames + page * SIZE);
        uintptr_t a2 = Reads<uintptr_t>(a1 + index * SIZE);
        uintptr_t nameaddr = a2 + 0xC;
        VMRead((void *)(nameaddr), &var, VAR);
        memcpy(result, var, VAR);
    }
}

PlayerWeapon getPlayerWeapon(uintptr_t address) {
    PlayerWeapon p;
    uintptr_t base[3];
    VMRead((void *)Reads<uintptr_t>(address + Offsets::Children), base, sizeof(base));
    if (isValid64(base[0])) {
        p.isWeapon = true;
        p.id = Reads<int>(Reads<uintptr_t>(base[0] + Offsets::ShootWeaponEntityComp) + Offsets::UploadInterval);
        p.ammo = Reads<int>(base[0] + Offsets::CurBulletNumInClip);
    } else if (isValid64(base[1])) {
        p.isWeapon = true;
        p.id = Reads<int>(Reads<uintptr_t>(base[1] + Offsets::ShootWeaponEntityComp) + Offsets::UploadInterval);
        p.ammo = Reads<int>(base[1] + Offsets::CurBulletNumInClip);
    } else if (isValid64(base[2])) {
        p.isWeapon = true;
        p.id = Reads<int>(Reads<uintptr_t>(base[2] + Offsets::ShootWeaponEntityComp) + Offsets::UploadInterval);
        p.ammo = Reads<int>(base[2] + Offsets::CurBulletNumInClip);
    } else {
        p.isWeapon = false;
    }
    return p;
}

int main(int argc, char *argv[]) {
    
	if (!Create()) {
		perror("Creation Failed");
		return 0;
	}
	if (!Connect()) {
		perror("Connection Failed");
		return 0;
	}

	pid = getPid("com.tencent.ig");
	if (pid == 0) {
		pid = getPid("com.pubg.krmobile");
		if (pid == 0) {
			pid = getPid("com.vng.pubgmobile");
			if (pid == 0) {
				pid = getPid("com.rekoo.pubgm");
				if (pid == 0) {
					pid = getPid("com.pubg.imobile");
					if (pid == 0) {
						printf("\nGame is not running");
						Close();
						std::this_thread::sleep_for(std::chrono::milliseconds(5000));
						return 0;
					}
				}
			}
		}
	}
    UE4 = getUE4();
    struct Vec2 RadarPosition = {width / 2, height / 2};
	struct Vec3 AimRotation;
	struct Vec3 HeadPosition;
	struct Vec3 TargetPos;
	MinimalViewInfo POV = MinimalViewInfo();
    CameraCacheEntry CameraCache = CameraCacheEntry();
	Request request{};
    Response response{};
	
    while ((receive((void*)&request) > 0)) {

            height = request.ScreenHeight;
            width = request.ScreenWidth;
        
        if (request.Mode == InitMode) {
            aimRadius = (float)request.options.aimingRange;
            recoil = (float)request.options.recCompe;
            bSpeed = (float)request.options.aimingSpeed;
            aimDist = (float) request.options.aimingDist;
			aimFor = request.options.aimbotmode;
            aimbot = request.options.openState == 0;
            aimWhen = request.options.aimingState;
            aimBy = request.options.priority;
            aimKnoced = request.options.pour;
            ignoreBot = request.options.ignoreBot;
            isLessRecoil = request.memory.LessRecoil;
            isZeroRecoil = request.memory.ZeroRecoil;
            isInstantHit = request.memory.InstantHit;
            isFastShootInterval = request.memory.FastShootInterval;
            isHitX = request.memory.HitX;
            isNoShake = request.memory.NoShake;
            isSmallCrosshair = request.memory.SmallCrosshair;
            isWideView = request.memory.WideView;         
            isAimbot = request.memory.Aimbot;         
            isFastWeapon = request.memory.FastWeapon;
            RadarPosition = request.radarPos;
            RadarSize = request.radarSize;
        }
        response.Success = false;
        response.InLobby = false;
        response.PlayerCount = 0;
        response.VehicleCount = 0;
        response.ItemsCount = 0;
        response.GrenadeCount = 0;
        response.ZoneCount = 0;


        uintptr_t uWorld = Reads<uintptr_t>(Reads<uintptr_t>(Reads<uintptr_t>(Reads<uintptr_t>(UE4 + Offsets::GWorld) + 0x58) + 0x78) + 0x30);
        uintptr_t uLevel = Reads<uintptr_t>(uWorld + 0x30);
        uintptr_t PlayerController = Reads<uintptr_t>(Reads<uintptr_t>(Reads<uintptr_t>(uWorld + 0x38) + 0x78) + 0x30);
        uintptr_t uMyObject = Reads<uintptr_t>(PlayerController + Offsets::ThirdPersonCameraComponent);
        uintptr_t PlayerCameraManager = Reads<uintptr_t>(PlayerController + Offsets::PlayerCameraManager);
        
		MinimalViewInfo POV = MinimalViewInfo();
        CameraCacheEntry CameraCache = CameraCacheEntry();
		
		if (PlayerCameraManager) {
        CameraCache = Reads<CameraCacheEntry>(PlayerCameraManager + Offsets::CameraCache);
        POV = CameraCache.POV;
        }
		
		Vec2 RadarPosition = {width / 4, height / 4};
        Vec3 AimRotation;
        Vec3 TargetPos;
		
		Vec3 myPos;
        myPos = POV.Location;

        uintptr_t entityPtr = DecryptActorsArray(uLevel, 0xA0, 0x448);
        uintptr_t ULevelToAActors = Reads<uintptr_t>(entityPtr);
        int ULevelToAActorsCount = Reads<int>(ULevelToAActors + SIZE);
        if (ULevelToAActorsCount < 0)
        {
            ULevelToAActorsCount = 0;
        }
        else if (ULevelToAActorsCount > 1024)
        {
            ULevelToAActorsCount = 1024;
        }

        response.fov = tanf(CameraCache.POV.FOV * ((float)PI / 360.0f));

        float nearest = -1.0f;

        for (int i = 0; i < ULevelToAActorsCount; i++)
        {
            uintptr_t actor = Reads<uintptr_t>(ULevelToAActors + i * SIZE);
            if (!isValid64(actor))
                continue;

            getGNames(actor, name);

            if (IsPlayerPawn(name))
            {

                if (Reads<int>(actor + Offsets::bDead))
                    continue;
					/*
				if (!Reads<uintptr_t>(actor + Offsets::SkeletalMeshComponent))
                    continue;
					*/
                if (!Reads<uintptr_t>(actor + Offsets::STCharacterMovement))
                    continue;	

                if (uMyObject != 0)
                {
                    ShootWeaponBase shootWeaponBase(uMyObject);
                    if (shootWeaponBase.isValid())
                    {
                        if (isLessRecoil || isZeroRecoil)
                        {
                            shootWeaponBase.setLessRecoil();
                        }
                        if (isZeroRecoil)
                        {
                            shootWeaponBase.setZeroRecoil();
                        }
                        if (isInstantHit)
                        {
                            shootWeaponBase.setInstantHit();
                        }
                        if (isFastShootInterval)
                        {
                            shootWeaponBase.setFastShootInterval();
                        }
                        if (isHitX)
                        {
                            shootWeaponBase.setHitX();
                        }
                        if (isSmallCrosshair)
                        {
                            shootWeaponBase.setSmallCrosshair();
                        }
                        if (isNoShake)
                        {
                            shootWeaponBase.setNoShake();
                        }
                        if (isAimbot)
                        {
                            shootWeaponBase.setAimbot();
                        }
                        if (isFastWeapon)
                        {
                            Write(uMyObject + Offsets::SwitchWeaponSpeedScale, "100", TYPE_FLOAT);
                        }
                    }
                    SceneComponent sceneComponent(uMyObject);
                    if (sceneComponent.isValid())
                    {
                        if (isWideView)
                        {
                            sceneComponent.setWideView();
                        }
                    }
                }

                PlayerData *data = &response.Players[response.PlayerCount];
                data->Health = Reads<float>(actor + Offsets::Health);
                data->HealthMax = Reads<float>(actor + Offsets::VHealthMax);
                data->HealthKnock = Reads<float>(actor + Offsets::NearDeathBreath);
                data->TeamID = Reads<int>(actor + Offsets::TeamID);

                if (actor == uMyObject)
                {

                    if (aimbot)
                    {
                        uintptr_t control = Reads<uintptr_t>(actor + Offsets::STPlayerController);
                        cameraManager = Reads<uintptr_t>(control + Offsets::PlayerCameraManager);
                        ads = Reads<bool>(actor + Offsets::bIsGunADS);
                        firing = Reads<bool>(actor + Offsets::bIsWeaponFiring);
                        adsfiring = (ads && firing);
                        if (aimWhen == 3)
                        {
                            AimTriger = (ads || firing);
                        }
                        if (aimWhen == 2)
                        {
                            AimTriger = firing;
                        }
                        if (aimWhen == 1)
                        {
                            AimTriger = ads;
                        }
                    }

                    myteamID = data->TeamID;
                    continue;
                }

                if (data->TeamID == myteamID && myteamID <= 100)
                    continue;

				int IsBot = Reads<int>(actor + Offsets::bIsAI);
				bool isBot = (IsBot != 0);
				
			    HeadPosition = GetBoneHead(actor);
                data->HeadLocation = W2S(GetBoneHead(actor), POV, width, height);
                data->RadarLocation = World2RadarRound(CameraCache, HeadPosition, RadarPosition, RadarSize);
                data->Distance = Vec3::Distance(GetBoneRoot(actor), POV.Location) / 100.0f;
                data->isBot = isBot;
				if (data->Distance > 600.0f)
                    continue;

                strcpy(data->PlayerNameByte, "66:79:84:");
                if (data->HeadLocation.Z > 0)
                {
					PlayerBone Bone;
                    Bone.isBone = true;
                    Bone.neck = W2S(GetBonePosScreen(actor, BoneID::neck_01), POV, width, height);
                    Bone.cheast = W2S(GetBonePosScreen(actor, BoneID::spine_02), POV, width, height);
                    Bone.pelvis = W2S(GetBonePosScreen(actor, BoneID::pelvis), POV, width, height);
                    Bone.lSh = W2S(GetBonePosScreen(actor, BoneID::upperarm_l), POV, width, height);
                    Bone.rSh = W2S(GetBonePosScreen(actor, BoneID::upperarm_r), POV, width, height);
                    Bone.lElb = W2S(GetBonePosScreen(actor, BoneID::lowerarm_l), POV, width, height);
                    Bone.rElb = W2S(GetBonePosScreen(actor, BoneID::lowerarm_r), POV, width, height);
                    Bone.lWr = W2S(GetBonePosScreen(actor, BoneID::hand_l), POV, width, height);
                    Bone.rWr = W2S(GetBonePosScreen(actor, BoneID::hand_r), POV, width, height);
                    Bone.lTh = W2S(GetBonePosScreen(actor, BoneID::thigh_l), POV, width, height);
                    Bone.rTh = W2S(GetBonePosScreen(actor, BoneID::thigh_r), POV, width, height);
                    Bone.lKn = W2S(GetBonePosScreen(actor, BoneID::calf_l), POV, width, height);
                    Bone.rKn = W2S(GetBonePosScreen(actor, BoneID::calf_r), POV, width, height);
                    Bone.lAn = W2S(GetBonePosScreen(actor, BoneID::foot_l), POV, width, height);
                    Bone.rAn = W2S(GetBonePosScreen(actor, BoneID::foot_r), POV, width, height);
                    Bone.root = W2S(GetBonePosScreen(actor, BoneID::Root), POV, width, height);
                    data->Bone = Bone;
                    data->Weapon = getPlayerWeapon(actor);
                    if (!data->isBot)
                    {
                        strcpy(data->PlayerNameByte, getNameByte(Reads<uintptr_t>(actor + Offsets::PlayerName)));
                        if (strlen(data->PlayerNameByte) < 4)
                            continue;
                    }
                }
                if (response.PlayerCount >= maxplayerCount)
                {
                    continue;
                }

                if (data->Distance < aimDist)
                {
                    if (data->HeadLocation.Z > 0 && (!aimKnoced || data->Health > 0) && (!ignoreBot || !data->isBot) && aimbot)
                    {
                        float centerDist = sqrt((data->HeadLocation.X - width / 2) * (data->HeadLocation.X - width / 2) + (data->HeadLocation.Y - height / 2) * (data->HeadLocation.Y - height / 2));
                        if (centerDist < aimRadius)
                        {
                            if (aimBy != 1)
                                centerDist = data->Distance;
                            if (nearest > centerDist || nearest < 0)
                            {
                                nearest = centerDist;
                                if (aimFor == 1)
                                {
                                    TargetPos = GetBoneHead(actor);
                                }
                                else if (aimFor == 2)
                                {
                                    TargetPos = GetBoneNeck(actor);
                                }
                                else
                                {
                                    TargetPos = GetBonePelvis(actor);
                                }
                                Vec3 myCam = POV.Location;
                                auto MovementComponent = Reads<uintptr_t>(actor + Offsets::STCharacterMovement);
                                auto CurrentVehicle = Reads<uintptr_t>(actor + Offsets::CurrentVehicle);
                                float Dist = Vec3::Distance(myCam, TargetPos) / 100.0f;
                                if (CurrentVehicle)
                                {
                                    Vec3 LinearVelocity = Reads<Vec3>(CurrentVehicle + Offsets::ReplicatedMovement);
                                    float Dist = Vec3::Distance(myCam, TargetPos) / 100.0f;
                                    auto timeToTravel = Dist / bSpeed;
                                    TargetPos = Add_VectorVector(TargetPos, Multiply_VectorFloat(LinearVelocity, timeToTravel));
                                }
                                else
                                {
                                    Vec3 Velocity = Reads<Vec3>(MovementComponent + Offsets::Velocity);
                                    float Dist = Vec3::Distance(myCam, TargetPos) / 100.0f;
                                    auto timeToTravel = Dist / bSpeed;
                                    TargetPos = Add_VectorVector(TargetPos, Multiply_VectorFloat(Velocity, timeToTravel));
                                }
                                AimRotation = CalcMousePos(TargetPos, myCam);
                                if (recoil > 1)
                                {
                                    if (adsfiring)
                                        AimRotation.Y -= recoil / 100.f;
                                }
                            }
                        }
                    }
                }
                response.PlayerCount++;
            }
            else if (IsVehiclesPawn(name))
            {
                uintptr_t RootComponent = Reads<uintptr_t>(actor + Offsets::RootComponent);
                uintptr_t Vehicle = Reads<uintptr_t>(actor + Offsets::VehicleCommon);
                auto Fuel = Reads<float>(Vehicle + Offsets::VFuel) * 100 / Reads<float>(Vehicle + Offsets::VFuelMax);
                auto Health = Reads<float>(Vehicle + Offsets::VHealth) * 100 / Reads<float>(Vehicle + Offsets::VHealthMax);
                Vec3 RelativePosition = Reads<Vec3>(RootComponent + Offsets::RelativeLocation);
                float Distance = Vec3::Distance(RelativePosition, myPos) / 100.0f;
                Vec3 Location = W2S(RelativePosition, POV, width, height);
                if (Location.Z > 0)
                {
                    VehicleData *data = &response.Vehicles[response.VehicleCount];
                    data->Location = Location;
                    data->Distance = Distance;
                    data->Fuel = Fuel;
                    data->Health = Health;
                    strcpy(data->VehicleName, name);
                    if (response.VehicleCount >= maxvehicleCount)
                    {
                        continue;
                    }
                    response.VehicleCount++;
                }
            }
            else if (IsItemsPawn(name))
            {
                uintptr_t RootComponent = Reads<uintptr_t>(actor + Offsets::RootComponent);
                Vec3 RelativePosition = Reads<Vec3>(RootComponent + Offsets::RelativeLocation);
                float Distance = Vec3::Distance(RelativePosition, myPos) / 100.0f;
                Vec3 Location = W2S(RelativePosition, POV, width, height);
                if (Distance < 0.0f || Distance > 200.0f)
                    continue;
                if (Location.Z > 0)
                {
                    ItemData *data = &response.Items[response.ItemsCount];
                    data->Location = Location;
                    data->Distance = Distance;
                    strcpy(data->ItemName, name);
                    if (response.ItemsCount >= maxitemsCount)
                    {
                        continue;
                    }
                    response.ItemsCount++;
                }
            }
            else if (IsItems2Pawn(name))
            {
                uintptr_t RootComponent = Reads<uintptr_t>(actor + Offsets::RootComponent);
                Vec3 RelativePosition = Reads<Vec3>(RootComponent + Offsets::RelativeLocation);
                float Distance = Vec3::Distance(RelativePosition, myPos) / 100.0f;
                Vec3 Location = W2S(RelativePosition, POV, width, height);
                if (Location.Z > 0)
                {
                    ItemData *data = &response.Items[response.ItemsCount];
                    data->Location = Location;
                    data->Distance = Distance;
                    strcpy(data->ItemName, name);
                    if (response.ItemsCount >= maxitemsCount)
                    {
                        continue;
                    }
                    response.ItemsCount++;
                }
            }
            else if (GrenadesSpawn(name))
            {
                uintptr_t RootComponent = Reads<uintptr_t>(actor + Offsets::RootComponent);
                Vec3 RelativePosition = Reads<Vec3>(RootComponent + Offsets::RelativeLocation);
                float Distance = Vec3::Distance(RelativePosition, myPos) / 100.0f;
                Vec3 Location = W2S(RelativePosition, POV, width, height);
                if (Distance < 0.0f || Distance > 150.0f)
                    continue;
                if (Location.Z > 0)
                {
                    GrenadeData *data = &response.Grenade[response.GrenadeCount];
                    data->type = GrenadesSpawn(name);
                    data->Location = Location;
                    data->Distance = Distance;
                    if (response.GrenadeCount >= maxgrenadeCount)
                    {
                        continue;
                    }
                    response.GrenadeCount++;
                }
            }
			
            else if (IsRedZoneSpawn(name))
            {
                uintptr_t RootComponent = Reads<uintptr_t>(actor + Offsets::RootComponent);
                Vec3 RelativePosition = Reads<Vec3>(RootComponent + Offsets::RelativeLocation);
                float Distance = Vec3::Distance(RelativePosition, myPos) / 100.0f;
                Vec3 Location = W2S(RelativePosition, POV, width, height);
                if (Location.Z > 0)
                {
                    ZoneData *data = &response.Zones[response.ZoneCount];
                    data->Distance = Distance;
                    data->Location = Location;
                    if (response.ZoneCount >= maxzonesCount)
                    {
                        continue;
                    }
                    response.ZoneCount++;
                }
            }
			
			else if (IsLobbyPawn(name)){
                    response.InLobby = true;
                }
            }
         
            
        if (response.PlayerCount + response.ItemsCount + response.VehicleCount + response.GrenadeCount + response.ZoneCount > 0)
            response.Success = true;
			
       
        send((void*)&response, sizeof(response));
        
        if ((AimTriger) && nearest > 0)
        {
            Writes(uintptr_t(cameraManager) + Offsets::ViewPitchMin, AimRotation.Y);
            Writes(uintptr_t(cameraManager) + Offsets::ViewPitchMax, AimRotation.Y);
            Writes(uintptr_t(cameraManager) + Offsets::ViewYawMin, AimRotation.X);
            Writes(uintptr_t(cameraManager) + Offsets::ViewYawMax, AimRotation.X);
        }
        else
        {
            Writes(uintptr_t(cameraManager) + Offsets::ViewPitchMin, -70.f);
            Writes(uintptr_t(cameraManager) + Offsets::ViewPitchMax, 75.f);
            Writes(uintptr_t(cameraManager) + Offsets::ViewYawMin, 0.01f);
            Writes(uintptr_t(cameraManager) + Offsets::ViewYawMax, 360.f);
        }
    }
}
