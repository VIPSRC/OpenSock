#include "INCLUDE/struct.h"
#include "Offsets.h"

using namespace Offsets;

uint64_t DecryptActorsArray(uint64_t uLevel, int Actors_Offset, int EncryptedActors_Offset) {
    if (uLevel < 0x10000000)
        return 0;
 
    if (Read<uint64_t>(uLevel + Actors_Offset) > 0)
		return uLevel + Actors_Offset;
 
    if (Read<uint64_t>(uLevel + EncryptedActors_Offset) > 0)
		return uLevel + EncryptedActors_Offset;
 
    auto AActors = Read<Actors>(uLevel + EncryptedActors_Offset + 0x10);
 
    if (AActors.Enc_1 > 0) {
        auto Enc = Read<Chunk>(AActors.Enc_1 + 0x80);
        return (((Read<uint8_t>(AActors.Enc_1 + Enc.val_1)
            | (Read<uint8_t>(AActors.Enc_1 + Enc.val_2) << 8))
            | (Read<uint8_t>(AActors.Enc_1 + Enc.val_3) << 0x10)) & 0xFFFFFF
            | ((uint64_t)Read<uint8_t>(AActors.Enc_1 + Enc.val_4) << 0x18)
            | ((uint64_t)Read<uint8_t>(AActors.Enc_1 + Enc.val_5) << 0x20)) & 0xFFFF00FFFFFFFFFF
            | ((uint64_t)Read<uint8_t>(AActors.Enc_1 + Enc.val_6) << 0x28)
            | ((uint64_t)Read<uint8_t>(AActors.Enc_1 + Enc.val_7) << 0x30)
            | ((uint64_t)Read<uint8_t>(AActors.Enc_1 + Enc.val_8) << 0x38);
    }
    else if (AActors.Enc_2 > 0) {
        auto Lost_Actors = Read<uint64_t>(AActors.Enc_2);
        if (Lost_Actors > 0) {
            return (uint16_t)(Lost_Actors - 0x400) & 0xFF00
                | (uint8_t)(Lost_Actors - 0x04)
                | (Lost_Actors + 0xFC0000) & 0xFF0000
                | (Lost_Actors - 0x4000000) & 0xFF000000
                | (Lost_Actors + 0xFC00000000) & 0xFF00000000
                | (Lost_Actors + 0xFC0000000000) & 0xFF0000000000
                | (Lost_Actors + 0xFC000000000000) & 0xFF000000000000
                | (Lost_Actors - 0x400000000000000) & 0xFF00000000000000;
        }
    }
    else if (AActors.Enc_3 > 0) {
        auto Lost_Actors = Read<uint64_t>(AActors.Enc_3);
        if (Lost_Actors > 0) {
            return (Lost_Actors >> 0x38) | (Lost_Actors << (64 - 0x38));
		}
    }
    else if (AActors.Enc_4 > 0) {
        auto Lost_Actors = Read<uint64_t>(AActors.Enc_4);
        if (Lost_Actors > 0) {
            return Lost_Actors ^ 0xCDCD00;
		}
    }
    return 0;
}

uintptr_t getEntityAddr(uintptr_t base) {
 	uintptr_t level = getA(getA(getA(getA(base + Offsets::ViewMatrix) + 0x58) + 0x78) + 0x30);
    return DecryptActorsArray(level, 0xA0, 0x448);
}

PlayerBone getPlayerBone(uintptr_t pBase, MinimalViewInfo &POV, bool isTrainingModel, bool isMetroMode) {
    PlayerBone b;
    b.isBone = true;
    FMatrix oMatrix;
    uintptr_t boneAddr = getA(pBase + Mesh);
    FMatrix baseMatrix = getOMatrix(boneAddr + FixAttachInfoList);
    boneAddr = getA(boneAddr + MinLOD);
    int bones[16] = { 6, 5, 5, 1, 12, 33, 13, 34, 14, 35, 53, 57, 54, 58, 55, 59 };
    if (isMetroMode) {
        bones[0] = 6; bones[1] = 5; bones[2] = 5; bones[3] = 1; bones[4] = 8; bones[5] = 14; bones[6] = 9; bones[7] = 15; bones[8] = 10; bones[9] = 16; bones[10] = 19; bones[11] = 22; bones[12] = 20; bones[13] = 23; bones[14] = 21; bones[15] = 24;
    }
	else if (isTrainingModel) {
		bones[0] = 6; bones[1] = 5; bones[2] = 4; bones[3] = 1; bones[4] = 14; bones[5] = 35; bones[6] = 15; bones[7] = 36; bones[8] = 16; bones[9] = 37; bones[10] = 55; bones[11] = 59; bones[12] = 56; bones[13] = 60; bones[14] = 57; bones[15] = 61;
	}
    // head 0
    oMatrix = getHeadOMatrix(boneAddr + bones[0] * 48);
    b.head = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // neck 1
    oMatrix = getOMatrix(boneAddr + bones[1] * 48);
    b.neck = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // cheast 2
    oMatrix = getOMatrix(boneAddr + bones[2] * 48);
    b.cheast = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // pelvis 3
    oMatrix = getOMatrix(boneAddr + bones[3] * 48);
    b.pelvis = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lSh 4
    oMatrix = getOMatrix(boneAddr + bones[4] * 48);
    b.lSh = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rSh 5
    oMatrix = getOMatrix(boneAddr + bones[5] * 48);
    b.rSh = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lElb 6
    oMatrix = getOMatrix(boneAddr + bones[6] * 48);
    b.lElb = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rElb 7
    oMatrix = getOMatrix(boneAddr + bones[7] * 48);
    b.rElb = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lWr 8
    oMatrix = getOMatrix(boneAddr + bones[8] * 48);
    b.lWr = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rWr 9
    oMatrix = getOMatrix(boneAddr + bones[9] * 48);
    b.rWr = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lTh 10
    oMatrix = getOMatrix(boneAddr + bones[10] * 48);
    b.lTh = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rTh 11
    oMatrix = getOMatrix(boneAddr + bones[11] * 48);
    b.rTh = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lKn 12
    oMatrix = getOMatrix(boneAddr + bones[12] * 48);
    b.lKn = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rKn 13
    oMatrix = getOMatrix(boneAddr + bones[13] * 48);
    b.rKn = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // lAn 14 
    oMatrix = getOMatrix(boneAddr + bones[14] * 48);
    b.lAn = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // rAn 15
    oMatrix = getOMatrix(boneAddr + bones[15] * 48);
    b.rAn = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    // root 16
    oMatrix = getOMatrix(boneAddr);
    b.root = WorldToScreen(mat2Cord(oMatrix, baseMatrix), POV, width, height);
    
    return b;
}

PlayerWeapon getPlayerWeapon(uintptr_t base) {
    PlayerWeapon p;
    uintptr_t addr[3];
    pvm(getA(base + Children), addr, sizeof(addr));

    if (isValid(addr[0]) && Read<int>(addr[0] + DrawShootLineTime) == 2) {
        p.id = Read<int>(getA(addr[0] + ShootWeaponEntityComp) + UploadInterval);
        p.ammo = Read<int>(addr[0] + CurBulletNumInClip);
        p.maxammo = Read<int>(addr[0] + CurMaxBulletNumInOneClip);
    }
    else if (isValid(addr[1]) && Read<int>(addr[1] + DrawShootLineTime) == 2) {
        p.id = Read<int>(getA(addr[1] + ShootWeaponEntityComp) + UploadInterval);
        p.ammo = Read<int>(addr[1] + CurBulletNumInClip);
        p.maxammo = Read<int>(addr[1] + CurMaxBulletNumInOneClip);
    }
    else if (isValid(addr[2]) && Read<int>(addr[2] + DrawShootLineTime) == 2) {
        p.id = Read<int>(getA(addr[2] + ShootWeaponEntityComp) + UploadInterval);
        p.ammo = Read<int>(addr[2] + CurBulletNumInClip);
        p.maxammo = Read<int>(addr[2] + CurMaxBulletNumInOneClip);
    }
    return p;
}

struct ShootWeaponBase {
    uintptr_t FromBase;
    uintptr_t Base;
    uintptr_t ShootWeaponEntity;
	
    ShootWeaponBase(uintptr_t pBase) {
        FromBase = getA(pBase + WeaponManagerComponent);
        Base = getA(FromBase + CurrWeapon);
        ShootWeaponEntity = getA(Base + ShootWeaponEntityComp);
    }
	
	void setAimbot() {
        Write<float>(ShootWeaponEntity + AutoAimingConfig + 0x0, 1090519040);
        Write<float>(ShootWeaponEntity + AutoAimingConfig + 0x4c, 1090519040); 
        Write<float>(ShootWeaponEntity + AutoAimingConfig + 0x4, 1082130432);
        Write<float>(ShootWeaponEntity + AutoAimingConfig + 0x50, 1082130432);
    }

    void setLessRecoil(uintptr_t ue4base) {
        Write<bool>(ue4base + 0xB82A590, false);
        Write<float>(ShootWeaponEntity + AccessoriesVRecoilFactor, 0);
		Write<float>(ShootWeaponEntity + AccessoriesHRecoilFactor, 0);
		Write<float>(ShootWeaponEntity + AccessoriesRecoveryFactor, 0);
    }
	
	void setZeroRecoil() {
		Write<float>(ShootWeaponEntity + RecoilKickADS, 0);
	}

	void setNoShake() {
		Write<float>(ShootWeaponEntity + AnimationKick, 0);
	}
	
    void setSmallCrosshair() {
        Write<float>(ShootWeaponEntity + GameDeviationFactor, 0);
    }

    void setInstantHit() {
        Write<float>(ShootWeaponEntity + BulletFireSpeed, 910000);
    }

	void setFastShoot() {
		Write<float>(ShootWeaponEntity + ShootInterval, 0.048000);
	}
	
    void setHitX() {
        Write<float>(ShootWeaponEntity + ExtraHitPerformScale, 50);
    }
    
    float getBulletFireSpeed() {
        return Read<float>(ShootWeaponEntity + BulletFireSpeed);
    }

    bool isValid() {
        return (Base != 0);
    }
};

struct CameraComponent {
    uintptr_t Base;
    
    CameraComponent(uintptr_t uMyObject) {
        Base = getA(uMyObject + ThirdPersonCameraComponent);
    }
    
    void bandIpadView() {
        Write<float>(Base + FieldOfView, 85);
    }

    void setIpadView1() {
        Write<float>(Base + FieldOfView, 100);
    }
    
    void setIpadView2() {
        Write<float>(Base + FieldOfView, 120);
    }
    
    void setIpadView3() {
        Write<float>(Base + FieldOfView, 130);
    }

    void setIpadView4() {
        Write<float>(Base + FieldOfView, 140);
    }
    
    void setIpadView5() {
        Write<float>(Base + FieldOfView, 150);
    }

    bool isValid() {
        return (Base != 0);
    }
};
/*
struct CharacterMovementComponent {
	uintptr_t CharacterMovement;
	
	CharacterMovementComponent(uintptr_t pBase) {
		CharacterMovement = getA(pBase + CharacterMovement);
	}

	void setSpeed() {
		Write(CharacterMovement + MaxWalkSpeed, "650.0", TYPE_FLOAT);
		Write(CharacterMovement + MaxWalkSpeedCrouched, "500000", TYPE_FLOAT);
	}

	bool isValid() {
		return (CharacterMovement != 0);
	}
};
*/

struct CharacterParachuteComponent {
	uintptr_t FastParachuteComponent;
	
	CharacterParachuteComponent(uintptr_t pBase) {
		FastParachuteComponent = getA(pBase + ParachuteComponent);
	}

	void setFastParachute() {
		Write(FastParachuteComponent + CurrentFallSpeed, "9276.9");
	}

	bool isValid() {
		return (ParachuteComponent != 0);
	}
};

int main(int argc, char *argv[]) {
    if (!Create()) {
        perror("Creation Failed");
        return 0;
    }
    if (!Connect()) {
        perror("Connection Failed");
        return 0;
    }
    
    if (isapkrunning("com.tencent.ig") == 1) {
        Offsets::PUBGMOBILE();
        strcpy(version, "com.tencent.ig");
    } else if (isapkrunning("com.pubg.krmobile") == 1) {
        Offsets::PUBGMOBILE();
        strcpy(version, "com.pubg.krmobile");
    } else if (isapkrunning("com.vng.pubgmobile") == 1) {
        Offsets::PUBGMOBILE();
        strcpy(version, "com.vng.pubgmobile");
    } else if (isapkrunning("com.rekoo.pubgm") == 1) {
        Offsets::PUBGMOBILE();
        strcpy(version, "com.rekoo.pubgm");
    } else if (isapkrunning("com.pubg.imobile") == 1) {
        Offsets::BattlegroundsIndia();
        strcpy(version, "com.pubg.imobile");
    }

    pid = getPid(version);
    if (pid < 10) {
        printf("\nGame is not running");
        Close();
        return 0;
    }

    isPremium = true;
    uintptr_t base = getBase();
	
	Vec3 pointingAngle;
	uintptr_t yawPitch = 0;
    float bulletFireSpeed;
	Vec3 location, targetAimPos,cam;
	int myteamID = 101;
	bool aimKnoced = false;
    bool aimAI = true;
	char name[100];
	uintptr_t gname_buff[30];
	uintptr_t gname = getA(getA(base + Offsets::GNames) + 0x110);
	pvm(gname, &gname_buff, sizeof(gname_buff));
	uint64_t cameraManager;
	int aimBy = 1, aimFor = 1, aimWhen = 3;
	float aimRadius = 200, aimDist = 350, aimSpeed = 660 , recCom = 60;
	int firing = 0, ads = 0;
    Request request {};
    Response response {};

    while (receive((void *) &request) > 0) {
        height = request.ScreenHeight;
        width = request.ScreenWidth;
        if (request.Mode == InitMode) {
            aimRadius = (float)request.options.aimingRange;
			aimFor = request.options.aimbotmode;
			aimbot = request.options.openState == 0;
			aimBy = request.options.priority;
			aimKnoced = request.options.pour;
			isMetro = request.options.isMetroMode;
            isRadar = request.options.isRadar;
            wideView = request.options.wideView;

            isLessRecoil = request.memory.LessRecoil;
			isZeroRecoil = request.memory.ZeroRecoil;
			isNoShake = request.memory.NoShake;
			isSmallCrosshair = request.memory.SmallCrosshair;
			isFastParachute = request.memory.FastParachute;
			isInstantHit = request.memory.InstantHit;
            isHitX = request.memory.HitEffect;   
			//isFastShootInterval = request.memory.FastShootInterval;
            //isFastSwitchWeapon = request.memory.FastSwitchWeapon;
			//isIpadView = request.memory.IpadView;
			//isSpeedKnock = request.memory.SpeedKnock;
            isAimbot = request.memory.Aimbot;
        }
        response.Success = false;
		response.Identified = false;
        response.PlayerCount = 0;
        response.VehicleCount = 0;
        response.ItemsCount = 0;
        response.GrenadeCount = 0;
        response.BoxItemsCount = 0;
		
        MinimalViewInfo POV = MinimalViewInfo();
        
        Ulevel ulevel;
        uintptr_t enAddrPntr = getEntityAddr(base);
        pvm(enAddrPntr, &ulevel, sizeof(ulevel));
        if (ulevel.size < 1 || ulevel.size > 0x1000 || !isValid(ulevel.addr)) {
            send((void *) &response, sizeof(response));
            continue;
        }
        
        float nearest = -1.0f;
        bool foundCameraManager = false;
        for (int i = 0; i < ulevel.size; i++) {
            uintptr_t pBase = getA(ulevel.addr + i * SIZE);
            if (!isValid(pBase)) {
                continue;
			}
			
            int ids = Read<int>(pBase + 8 + 2 * SIZE);
            int page = ids / 0x4000;
            int index = ids % 0x4000;
			
            if (page < 1 || page > 30) {
                continue;
			}
			
                    CameraComponent cameraComponent(pBase);
                    if (cameraComponent.isValid()) {
                        if (wideView == 0 && isWideView) {
                            isWideView = false;
                            cameraComponent.bandIpadView();
                        }                                                       
                        else if (wideView == 1) {
                            isWideView = true;
                            cameraComponent.setIpadView1();
                        }
                        else if (wideView == 2) {
                            isWideView = true;
                            cameraComponent.setIpadView2();
                        }
                        else if (wideView == 3) {
                            isWideView = true;
                            cameraComponent.setIpadView3();
                        }
                        else if (wideView == 4) {
                            isWideView = true;
                            cameraComponent.setIpadView4();
                        }
                        else if (wideView == 5) {
                            isWideView = true;
                            cameraComponent.setIpadView5();
                        }
                    }
                
                if (isRadar) {
                    data->RadarLocation = WorldToRadar(POV.Rotation.Yaw, location, POV.Location);
                }
                
				if (isValidPosition(data->Location)) {
                    data->Bone = getPlayerBone(pBase, POV, isTrainingModel, isMetroMode);
					uintptr_t boneAddr = getA(pBase + Mesh);
					Vec3 head = mat2Cord(getOMatrix2(getA(boneAddr + MinLOD) + 6 * 48, 1.15f), getOMatrix2(boneAddr + FixAttachInfoList, 1.15f));
					Vec2 precise_Head = WorldToScreen(head, POV, width, height);
                    Vec3 root = mat2Cord(getOMatrix(getA(boneAddr + MinLOD)), getOMatrix(boneAddr + FixAttachInfoList));
                    Vec2 precise_root = WorldToScreen(root, POV, width, height);
					data->Precise = getPrecise(data->Bone, precise_Head, precise_root);
					//data->HeadLocation = World2Screen(cameraView, mat2Cord(oMatrix, baseMatrix));
				
                  
                    
                    if (data->Precise.Z != 1.0f && (aimKnoced || data->Health > 0) && data->Distance <= aimDist && (aimAI || !data->isBot) && aimbot) {
                        float centerDist = sqrt((data->Bone.head.X - width / 2) * (data->Bone.head.X - width / 2) + (data->Bone.head.Y - height / 2) * (data->Bone.head.Y - height / 2));
                        if (centerDist < aimRadius) {
                            if (aimBy != 1) {
                                centerDist = data->Distance;
                            }
                        
                            if (nearest > centerDist || nearest < 0) {
                                nearest = centerDist;
                                if (aimFor == 1) {
                                    FMatrix baseMatrix = getOMatrix(boneAddr + FixAttachInfoList);
                                    boneAddr = getA(boneAddr + MinLOD);
                                    FMatrix oMatrix = getOMatrix(boneAddr + 6 * 48);
                                    targetAimPos = mat2Cord(oMatrix, baseMatrix);
                                }    
                                else if (aimFor == 2) {
                                    FMatrix baseMatrix = getOMatrix(boneAddr + FixAttachInfoList);
                                    boneAddr = getA(boneAddr + MinLOD);
                                    FMatrix oMatrix = getOMatrix(boneAddr + 4 * 48);
                                    targetAimPos = mat2Cord(oMatrix, baseMatrix);
                                }
                                else {
                                    FMatrix baseMatrix = getOMatrix(boneAddr + FixAttachInfoList);
                                    boneAddr = getA(boneAddr + MinLOD);
                                    FMatrix oMatrix = getOMatrix(boneAddr + 2 * 48);
                                    targetAimPos = mat2Cord(oMatrix, baseMatrix);
                                }
                                cam = getVec3(uint64_t(cameraManager) + CameraCache2 + 0x0);
                                uintptr_t MovementComponent = getA(pBase + Offsets::CharacterMovement);
                                uintptr_t CurrentVehicle = getA(pBase + CurrentVehicle);
                                if (CurrentVehicle) {
                                    Vec3 LinearVelocity = getVec3(CurrentVehicle + ReplicatedMovement);
                                    float Dist = getDistance(POV.Location, targetAimPos);
                                    auto timeToTravel = Dist / aimSpeed;
                                    targetAimPos = Add_VectorVector(targetAimPos,Multiply_VectorFloat(LinearVelocity, timeToTravel));
                                } else {
                                    auto Velocity = getVec3(MovementComponent + Offsets::Velocity);
                                    float Dist = getDistance(POV.Location, targetAimPos);
                                    auto timeToTravel = Dist / aimSpeed;
                                    targetAimPos = Add_VectorVector(targetAimPos,Multiply_VectorFloat(Velocity, timeToTravel));
                                }
                                
								pointingAngle = CalcMousePos(targetAimPos,cam);
                                pointingAngle.Y -= recCom / 100;
                            }
                        }
                    }
                }
                
				if (response.PlayerCount >= maxplayerCount) {
                    continue;
				}
                response.PlayerCount++;
            }
			else if (strstr(name, "VH") || (strstr(name, "PickUp_") && !strstr(name, "BP")) || strstr(name, "Rony") || strstr(name, "Mirado") || strstr(name, "LadaNiva") || strstr(name, "AquaRail")) {
                if (!isPremium) {
                    continue;
				}
				
                VehicleData *data = &response.Vehicles[response.VehicleCount];
                pvm(getA(pBase + RootComponent) + ParachuteEquipItems, &location, sizeof(location));
                data->Location = WorldToScreen(location, POV, width, height);
                if (!isValidPosition(data->Location)) {
                    continue;
				}
				uintptr_t Vehicle = getA(pBase + VehicleCommon);
                data->Fuel = Read<float>(Vehicle + VFuel) * 100 / Read<float>(Vehicle + VFuelMax);
                data->Health = Read<float>(Vehicle + VHealth) * 100 / Read<float>(Vehicle + VHealthMax);
                data->Distance = getDistance(POV.Location, location);
                strcpy(data->VehicleName, name);
				
                if (response.VehicleCount >= maxvehicleCount) {
                    continue;
				}
                response.VehicleCount++;
            }
          

                ItemData *data = &response.Items[response.ItemsCount];
                pvm(getA(pBase + RootComponent) + ParachuteEquipItems, &location, sizeof(location));
                data->Location = WorldToScreen(location, POV, width, height);
                if (!isValidPosition(data->Location)) {
                    continue;
                }
                data->Distance = getDistance(POV.Location, location);
                strcpy(data->ItemName, name);
                
                if (response.ItemsCount >= maxitemsCount) {
                    continue;
                }
                response.ItemsCount++;
            }
			else if (strstr(name, "BP_Grenade_Shoulei_C") || strstr(name, "BP_Grenade_Burn_C") || strstr(name, "BP_Grenade_Stun_C")) {
                if (!isPremium)
                    continue;

                GrenadeData *data = &response.Grenade[response.GrenadeCount];
                pvm(getA(pBase + RootComponent) + ParachuteEquipItems, &location, sizeof(location));
                data->Location = WorldToScreen(location, POV, width, height);
                data->Distance = getDistance(POV.Location, location);

                if (data->Distance > 150.0f) {
                    continue;
				}

                if (strstr(name, "Shoulei")) {
                    data->type = 1;
				}
                else if (strstr(name, "Burn")) {
                    data->type = 2;
				}
                else if (strstr(name, "Stun")) {
                    data->type = 3;
				}

                if (response.GrenadeCount >= maxgrenadeCount) {
                    continue;
                }
                response.GrenadeCount++;
            }
            if (strstr(name, "PickUpListWrapperActor") || strstr(name, "AirDropListWrapperActor")) {
                if (!isPremium) {
                    continue;
                }
                
                BoxItemData *data = &response.BoxItems[response.BoxItemsCount];
                pvm(getA(pBase + RootComponent) + ParachuteEquipItems, &location, sizeof(location));
                data->Location = WorldToScreen(location, POV, width, height);
				
                if (!isValidPosition(data->Location)) {
                    continue;
                }
                
                if (strstr(name, "AirDropListWrapperActor")) {
                    data->ItemType = 1;
                }
                else {
					float Distance = getDistance(POV.Location, location);
					if (Distance > 150.0f) {
                    	continue;
					}
                	data->ItemType = 2;
                }
                
                uintptr_t pickUpDataList = getA(pBase + PickUpDataList);
                data->ItemCount = Read<int>(pBase + PickUpDataListCount);
                
                if (data->ItemCount > 40) data->ItemCount = 40;
                
                for (int k = 0; k < data->ItemCount; k++) {
                    data->ItemID[k] = Read<int>(pickUpDataList + (k * 0x38) + 0x4);
                    data->Count[k] = Read<int>(pickUpDataList + (k * 0x38) + 0x18);
                }
                
                if (response.BoxItemsCount >= maxboxitemsCount) {
                    continue;
                }
                response.BoxItemsCount++;
            }
        }
        if (response.PlayerCount + response.ItemsCount + response.VehicleCount + response.GrenadeCount > 0) {
            response.Success = true;
        }
        send((void *) &response, sizeof(response));
        if ((firing || ads) && nearest > 0) {
                Write(uint64_t(cameraManager) + Offsets::ViewPitchMin, pointingAngle.Y);
                Write(uint64_t(cameraManager) + Offsets::ViewPitchMax, pointingAngle.Y);
                Write(uint64_t(cameraManager) + Offsets::ViewYawMin, pointingAngle.X);
                Write(uint64_t(cameraManager) + Offsets::ViewYawMax, pointingAngle.X);
            } else {
                Write(uint64_t(cameraManager) + Offsets::ViewPitchMin, -70.f);
                Write(uint64_t(cameraManager) + Offsets::ViewPitchMax, 75.f);
                Write(uint64_t(cameraManager) + Offsets::ViewYawMin, 0.01f);
                Write(uint64_t(cameraManager) + Offsets::ViewYawMax, 360.f);
            }
    }
}

