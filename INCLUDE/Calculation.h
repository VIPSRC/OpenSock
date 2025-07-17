#ifndef CALCULATION_H
#define CALCULATION_H

#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Offsets.h"

//Calculation BoneID
enum BoneID {
    Root,//0
    pelvis,//1
    spine_01,//2
    spine_02,//3
    spine_03,//4
    neck_01,//5
    Head,//6
    hair_01,//7
    hair_02,//8
    hair_03,//9
    hair_04,//10
    clavicle_l,//11
    upperarm_l,//12
    lowerarm_l,//13
    hand_l,//14
    thumb_01_l,//15
    thumb_02_l,//16
    thumb_03_l,//17
    index_01_l,//18
    index_02_l,//19
    index_03_l,//20
    middle_01_l,//21
    middle_02_l,//22
    middle_03_l,//23
    ring_01_l,//24
    ring_02_l,//25
    ring_03_l,//26
    pinky_01_l,//27
    pinky_02_l,//28
    pinky_03_l,//29
    item_l,//30
    lowerarm_twist_01_l,//31
    clavicle_r,//32
    upperarm_r,//33
    lowerarm_r,//34
    hand_r,//35
    thumb_01_r,//36
    thumb_02_r,//37
    thumb_03_r,//38
    index_01_r,//39
    index_02_r,//40
    index_03_r,//41
    middle_01_r,//42
    middle_02_r,//43
    middle_03_r,//44
    ring_01_r,//45
    ring_02_r,//46
    ring_03_r,//47
    pinky_01_r,//48
    pinky_02_r,//49
    pinky_03_r,//50
    item_r,//51
    lowerarm_twist_01_r,//52
    thigh_l,//53
    calf_l,//54
    foot_l,//55
    ball_l,//56
    thigh_r,//57
    calf_r,//58
    foot_r,//59
    ball_r,//60
    ik_hand_root,//61
    ik_hand_gun,//62
    ik_hand_r,//63
    ik_hand_l//64
};


struct FMatrix {
    float M[4][4];
};

struct Vec4 rot;

struct Vec3 scale, tran;

struct D3DMatrix {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

struct FRotator {
    float Pitch;
    float Yaw;
    float Roll;
};

struct MinimalViewInfo {
    Vec3 Location;
    Vec3 LocationLocalSpace;
    FRotator Rotation;
    float FOV;
};

struct CameraCacheEntry {
    float TimeStamp;
    char chunks[0xC];
    MinimalViewInfo POV;
};

struct RepMovement {
    Vec3 LinearVelocity;
	Vec3 AngularVelocity;
	Vec3 Location;
	FRotator Rotation;
};

struct FTransform {
    Vec4 Rotation;
    Vec3 Translation;
    char pad[0x4];
    Vec3 Scale3D;
    D3DMatrix ToMatrixWithScale()
    {
        D3DMatrix m;
        m._41 = Translation.X;
        m._42 = Translation.Y;
        m._43 = Translation.Z;

        float x2 = Rotation.X + Rotation.X;
        float y2 = Rotation.Y + Rotation.Y;
        float z2 = Rotation.Z + Rotation.Z;

        float xx2 = Rotation.X * x2;
        float yy2 = Rotation.Y * y2;
        float zz2 = Rotation.Z * z2;
        m._11 = (1.0f - (yy2 + zz2)) * Scale3D.X;
        m._22 = (1.0f - (xx2 + zz2)) * Scale3D.Y;
        m._33 = (1.0f - (xx2 + yy2)) * Scale3D.Z;

        float yz2 = Rotation.Y * z2;
        float wx2 = Rotation.W * x2;
        m._32 = (yz2 - wx2) * Scale3D.Z;
        m._23 = (yz2 + wx2) * Scale3D.Y;

        float xy2 = Rotation.X * y2;
        float wz2 = Rotation.W * z2;
        m._21 = (xy2 - wz2) * Scale3D.Y;
        m._12 = (xy2 + wz2) * Scale3D.X;

        float xz2 = Rotation.X * z2;
        float wy2 = Rotation.W * y2;
        m._31 = (xz2 + wy2) * Scale3D.Z;
        m._13 = (xz2 - wy2) * Scale3D.X;

        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }
};







Vec3 MatrixToVector(FMatrix matrix) {
    return {matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]};
}

FMatrix MatrixMultiplication(FMatrix m1, FMatrix m2) {
    FMatrix matrix = FMatrix();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
            }
        }
    }
    return matrix;
}

D3DMatrix MatrixMultiplication(D3DMatrix pM1, D3DMatrix pM2)
{
    D3DMatrix pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return pOut;
}

struct Vec3 mat2Cord(struct D3DMatrix pM1,struct D3DMatrix pM2)
{
	struct  Vec3 pOut;

	pOut.X = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut.Y = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut.Z = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;

	return pOut;
}



FMatrix TransformToMatrix(FTransform transform) {
    FMatrix matrix;

    matrix.M[3][0] = transform.Translation.X;
    matrix.M[3][1] = transform.Translation.Y;
    matrix.M[3][2] = transform.Translation.Z;

    float x2 = transform.Rotation.X + transform.Rotation.X;
    float y2 = transform.Rotation.Y + transform.Rotation.Y;
    float z2 = transform.Rotation.Z + transform.Rotation.Z;

    float xx2 = transform.Rotation.X * x2;
    float yy2 = transform.Rotation.Y * y2;
    float zz2 = transform.Rotation.Z * z2;

    matrix.M[0][0] = (1.0f - (yy2 + zz2)) * transform.Scale3D.X;
    matrix.M[1][1] = (1.0f - (xx2 + zz2)) * transform.Scale3D.Y;
    matrix.M[2][2] = (1.0f - (xx2 + yy2)) * transform.Scale3D.Z;

    float yz2 = transform.Rotation.Y * z2;
    float wx2 = transform.Rotation.W * x2;
    matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
    matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;

    float xy2 = transform.Rotation.X * y2;
    float wz2 = transform.Rotation.W * z2;
    matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
    matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;

    float xz2 = transform.Rotation.X * z2;
    float wy2 = transform.Rotation.W * y2;
    matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
    matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;

    matrix.M[0][3] = 0;
    matrix.M[1][3] = 0;
    matrix.M[2][3] = 0;
    matrix.M[3][3] = 1;

    return matrix;
}

FMatrix RotatorToMatrix(FRotator rotation)
{
    float radPitch = rotation.Pitch * ((float) M_PI / 180.0f);
    float radYaw = rotation.Yaw * ((float) M_PI / 180.0f);
    float radRoll = rotation.Roll * ((float) M_PI / 180.0f);

    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);

    FMatrix matrix;

    matrix.M[0][0] = (CP * CY);
    matrix.M[0][1] = (CP * SY);
    matrix.M[0][2] = (SP);
    matrix.M[0][3] = 0;

    matrix.M[1][0] = (SR * SP * CY - CR * SY);
    matrix.M[1][1] = (SR * SP * SY + CR * CY);
    matrix.M[1][2] = (-SR * CP);
    matrix.M[1][3] = 0;

    matrix.M[2][0] = (-(CR * SP * CY + SR * SY));
    matrix.M[2][1] = (CY * SR - CR * SP * SY);
    matrix.M[2][2] = (CR * CP);
    matrix.M[2][3] = 0;

    matrix.M[3][0] = 0;
    matrix.M[3][1] = 0;
    matrix.M[3][2] = 0;
    matrix.M[3][3] = 1;

    return matrix;
}

Vec3 W2S(Vec3 worldLocation, MinimalViewInfo camViewInfo, int screenWidth, int screenHeight) {
    FMatrix tempMatrix = RotatorToMatrix(camViewInfo.Rotation);
    Vec3 vAxisX(tempMatrix.M[0][0], tempMatrix.M[0][1], tempMatrix.M[0][2]);
    Vec3 vAxisY(tempMatrix.M[1][0], tempMatrix.M[1][1], tempMatrix.M[1][2]);
    Vec3 vAxisZ(tempMatrix.M[2][0], tempMatrix.M[2][1], tempMatrix.M[2][2]);
    Vec3 vDelta = worldLocation - camViewInfo.Location;
    Vec3 vTransformed(Vec3::Dot(vDelta, vAxisY), Vec3::Dot(vDelta, vAxisZ), Vec3::Dot(vDelta, vAxisX));
    float fov = camViewInfo.FOV;
    float screenCenterX = (screenWidth / 2.0f);
    float screenCenterY = (screenHeight / 2.0f);
    float X = (screenCenterX + vTransformed.X * (screenCenterX / tanf(fov * ((float)M_PI / 360.0f))) / vTransformed.Z);
    float Y = (screenCenterY - vTransformed.Y * (screenCenterX / tanf(fov * ((float)M_PI / 360.0f))) / vTransformed.Z);
    float Z = vTransformed.Z;

    return {X, Y, Z};
}

Vec3 TransformToLocation(FTransform c2w, FTransform transform) {
    return MatrixToVector(MatrixMultiplication(TransformToMatrix(transform), TransformToMatrix(c2w)));
}

//Calculate BonePos

FTransform GetBoneTransform(uintptr_t actor, int idx) {
    auto Mesh = Read<uintptr_t>(actor + Offsets::FixAttachInfoList);
    uintptr_t Bones = Read<uintptr_t>(Mesh + Offsets::FixAttachInfoList - 0x20);
    if (!Bones) {
        Bones = Read<uintptr_t>(Mesh + Offsets::FixAttachInfoList + 0x10);
    }
    return Read<FTransform>(Bones + (idx * 0x30));
}

FTransform GetComponentToWorld(uintptr_t actor) {
    uintptr_t Mesh = Read<uintptr_t>(actor + Offsets::FixAttachInfoList);
        return Read<FTransform>(Mesh + 0x1A0);
}

Vec3 GetBonePosScreen(uintptr_t actor, int boneIdx) {
        return TransformToLocation(GetComponentToWorld(actor), GetBoneTransform(actor, boneIdx));
}

Vec3 GetBoneHead(uintptr_t actor) {
    return GetBonePosScreen(actor, BoneID::Head);
}

Vec3 GetBoneNeck(uintptr_t actor) {
    return GetBonePosScreen(actor, BoneID::neck_01);
}

Vec3 GetBonePelvis(uintptr_t actor) {
    return GetBonePosScreen(actor, BoneID::spine_02);
}

Vec3 GetBoneRoot(uintptr_t actor) {
    return GetBonePosScreen(actor, BoneID::Root);
}

Vec3 CalcMousePos(Vec3 headPos, Vec3 uMyobejctPos) {
    Vec3 AimPos;
    float x = headPos.X - uMyobejctPos.X;
    float y = headPos.Y - uMyobejctPos.Y;
    float z = headPos.Z - uMyobejctPos.Z;
    AimPos.X = atan2(y, x) * 180.f / M_PI;
    AimPos.Y = atan2(z, sqrt(x * x + y * y)) * 180.f / PI;
    return AimPos;
}

Vec3 Multiply_VectorFloat(const Vec3& Vec, float Scale)
{
    Vec3 multiply = {Vec.X * Scale, Vec.Y * Scale, Vec.Z * Scale};
    return multiply;
}

Vec3 Add_VectorVector(struct Vec3 Vect1, struct Vec3 Vect2)
{
    Vec3 add;
    add.X = Vect1.X + Vect2.X;
    add.Y = Vect1.Y + Vect2.Y;
    add.Z = Vect1.Z + Vect2.Z;
    return add;
}

struct Vec2 World2RadarRectangle(struct CameraCacheEntry CameraCache, struct Vec3 EnemyPos, struct Vec2 RadarPos, float RadarSize) {
    struct Vec2 results;
    FRotator qAngle = CameraCache.POV.Rotation;
    Vec3 CameraLoc = CameraCache.POV.Location;
    float flDeltaX = EnemyPos.X - CameraLoc.X;
	float flDeltaY = EnemyPos.Y - CameraLoc.Y;
	float flAngle = qAngle.Yaw;
	float flYaw = (flAngle) * (M_PI / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);
	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;
	y = -y;
	x = -x;
    float clipAngle = 10000; // 3000 = 30m ini tuh maksudnya untuk jangkauan dari radarnya mau berapa meter biar keliatan di map
	if (fabs(x) > clipAngle || fabs(y) > clipAngle){
        x = (y > -x ? (y > x ? clipAngle * x / y : clipAngle) : (y > x ? -clipAngle * x / y : -clipAngle));
        y = (y > -x ? (y > x ? clipAngle : clipAngle * y / x) : (y > x ? -clipAngle : -clipAngle * y / x));
    }
    results = {RadarPos.X + int(x / clipAngle * RadarSize), RadarPos.Y - int(y / clipAngle * RadarSize)};
    return results;//Code credits @SubaktiKesumaGMG & Unkowncheats for lil bit theory
}

struct Vec2 World2RadarRound(struct CameraCacheEntry CameraCache, struct Vec3 EnemyPos, struct Vec2 RadarPos, float RadarSize) {
    struct Vec2 results;
    FRotator qAngle = CameraCache.POV.Rotation;
    Vec3 CameraLoc = CameraCache.POV.Location;
    float flDeltaX = EnemyPos.X - CameraLoc.X;
	float flDeltaY = EnemyPos.Y - CameraLoc.Y;
	float flAngle = qAngle.Yaw;
	float flYaw = (flAngle) * (M_PI / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);
	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;
	y = -y;
	x = -x;
    double vectorLength = sqrt((x * x) + (y * y));
    float clipAngle = 10000;
    if (vectorLength > clipAngle) {
        x = (x / vectorLength) * clipAngle;
        y = (y / vectorLength) * clipAngle;
    }
    results = {RadarPos.X + int(x / clipAngle * RadarSize), RadarPos.Y - int(y / clipAngle * RadarSize)};
    return results;//Code credits @SubaktiKesumaGMG & Unkowncheats for lil bit theory
}

//Calculation Actor Names
bool IsPlayerPawn(char *classname) {
    if (strstr(classname, "PlayerPawn") || strstr(classname, "PlayerCharacter") || strstr(classname, "CharacterModelTaget") || strstr(classname, "PlanET_FakePlayer") || strstr(classname, "PlayerPawn_Infec"))
          return true;
    return false;
}

bool IsVehiclesPawn(char *classname){
	if (strstr(classname, "VH") || (strstr(classname, "PickUp_") && !strstr(classname, "BP")) || strstr(classname, "Rony") || strstr(classname, "Mirado") || strstr(classname, "LadaNiva") || strstr(classname, "AquaRail"))
		return true;
	return false;	
}


bool IsItemsPawn(char *classname){
    if (strstr(classname, "Pickup_C") || strstr(classname, "PickUp") || strstr(classname, "BP_Ammo") || strstr(classname, "BP_QK") || strstr(classname, "Wrapper"))
        return true;
    return false;
}

bool IsItems2Pawn(char *classname){
	if (strstr(classname, "AirDropPlane") || strstr(classname, "PlayerDeadInventoryBox") || strstr(classname, "AirDropBox"))
        return true;
    return false;
}

int GrenadesSpawn(char *classname) {
    if (strstr(classname, "Grenade_Shoulei")) {
        return 1;
    }
    if (strstr(classname, "Grenade_Burn")) {
        return 2;
    }
    if (strstr(classname, "Grenade_Stun")) {
        return 3;
    }
    if (strstr(classname, "Grenade_Smoke")) {
        return 4;
    }
    return 0; // Return 0 for an unknown type or no match
}


bool IsRedZoneSpawn(char *classname){
	if (strstr(classname, "AirAttackBomb"))
		return true;
	return false;
}

bool IsLobbyPawn(char *classname){
	if (strstr(classname, "Lobby"))
		return true;
	return false;
}

struct ShootWeaponBase {
    uintptr_t FromBase;
    uintptr_t Base;
    uintptr_t ShootWeaponEntity;
    int bIsWeaponFiring;
    ShootWeaponBase (uintptr_t pBase) {
        FromBase = Read<uintptr_t>(pBase + Offsets::WeaponManagerComponent);
        Base = Read<uintptr_t>(FromBase + Offsets::CurrWeapon);
        ShootWeaponEntity = Read<uintptr_t>(Base + Offsets::ShootWeaponEntityComp);
        bIsWeaponFiring = Read<int>(pBase + Offsets::bIsWeaponFiring);
    }

    void setInstantHit() {
        Write(ShootWeaponEntity + Offsets::BulletFireSpeed, "900000", TYPE_FLOAT);
    }

    void setLessRecoil() {
        Write(ShootWeaponEntity + Offsets::AccessoriesVRecoilFactor,"0.2", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets::AccessoriesHRecoilFactor, "0.2", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets::AccessoriesRecoveryFactor, "0.2", TYPE_FLOAT);
    }

    void setZeroRecoil() {
        Write(ShootWeaponEntity + Offsets::RecoilKickADS, "0", TYPE_FLOAT);
    }

    void setFastShootInterval() {
        Write(ShootWeaponEntity + Offsets::ShootInterval, "0.048000", TYPE_FLOAT);
    }

    void setSmallCrosshair() {
        Write(ShootWeaponEntity + Offsets::GameDeviationFactor,"0.5", TYPE_FLOAT);
    }

    void setHitX() {
        Write(ShootWeaponEntity + Offsets::ExtraHitPerformScale, "50", TYPE_FLOAT);
    }

    void setNoShake() {
        Write(ShootWeaponEntity + Offsets::AnimationKick, "0.5", TYPE_FLOAT);
    }
    
    
    void setAimbot() {
		Write(ShootWeaponEntity + Offsets::AutoAimingConfig + 0x0, "8.0", TYPE_FLOAT);
		Write(ShootWeaponEntity + Offsets::AutoAimingConfig + 0x4c, "8.0", TYPE_FLOAT);
		Write(ShootWeaponEntity + Offsets::AutoAimingConfig + 0x4, "4.0", TYPE_FLOAT); 
		Write(ShootWeaponEntity + Offsets::AutoAimingConfig + 0x50, "4.0", TYPE_FLOAT);
    }

    bool isFiring() {
        return (bIsWeaponFiring != 0);
    }
    bool isValid() {
        return (Base != 0);
    }
};

struct SceneComponent {
    uintptr_t CameraComponent;
    SceneComponent (uintptr_t pBase) {
        CameraComponent = Read<uintptr_t>(pBase + Offsets::ThirdPersonCameraComponent);
    }

    void setWideView() {
        Write(CameraComponent + Offsets::FieldOfView, "110", TYPE_FLOAT);
    }
    bool isValid() {
        return (CameraComponent != 0);
    }
};


struct Actors {
    uint64_t Enc_1, Enc_2;
    uint64_t Enc_3, Enc_4;
};

struct Chunk {
    uint32_t val_1, val_2, val_3, val_4;
    uint32_t val_5, val_6, val_7, val_8;
};

uint64_t DecryptActorsArray(uint64_t uLevel, int Actors_Offset, int EncryptedActors_Offset) {
    if (uLevel < 0x10000000)
        return 0;
 
    if (Reads<uint64_t>(uLevel + Actors_Offset) > 0)
        return uLevel + Actors_Offset;
 
    if (Reads<uint64_t>(uLevel + EncryptedActors_Offset) > 0)
        return uLevel + EncryptedActors_Offset;
 
    auto AActors = Reads<Actors>(uLevel + EncryptedActors_Offset + 0x10);
 
    if (AActors.Enc_1 > 0) {
        auto Enc = Reads<Chunk>(AActors.Enc_1 + 0x80);
        return (((Reads<uint8_t>(AActors.Enc_1 + Enc.val_1)
            | (Reads<uint8_t>(AActors.Enc_1 + Enc.val_2) << 8))
            | (Reads<uint8_t>(AActors.Enc_1 + Enc.val_3) << 0x10)) & 0xFFFFFF
            | ((uint64_t)Reads<uint8_t>(AActors.Enc_1 + Enc.val_4) << 0x18)
            | ((uint64_t)Reads<uint8_t>(AActors.Enc_1 + Enc.val_5) << 0x20)) & 0xFFFF00FFFFFFFFFF
            | ((uint64_t)Reads<uint8_t>(AActors.Enc_1 + Enc.val_6) << 0x28)
            | ((uint64_t)Reads<uint8_t>(AActors.Enc_1 + Enc.val_7) << 0x30)
            | ((uint64_t)Reads<uint8_t>(AActors.Enc_1 + Enc.val_8) << 0x38);
    }
    else if (AActors.Enc_2 > 0) {
        auto Lost_Actors = Reads<uint64_t>(AActors.Enc_2);
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
        auto Lost_Actors = Reads<uint64_t>(AActors.Enc_3);
        if (Lost_Actors > 0) {
            return (Lost_Actors >> 0x38) | (Lost_Actors << (64 - 0x38));
        }
    }
    else if (AActors.Enc_4 > 0) {
        auto Lost_Actors = Reads<uint64_t>(AActors.Enc_4);
        if (Lost_Actors > 0) {
            return Lost_Actors ^ 0xCDCD00;
        }
    }
    return 0;
}

#endif //CALCULATION_H BY @XACODE_APP
