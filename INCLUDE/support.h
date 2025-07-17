#include "socket.h"
#define PI 3.141592653589793238
ssize_t process_v33(pid_t _pid, const struct iovec *_local_iov, unsigned long _local_iov_count,
                  const struct iovec *_remote_iov, unsigned long _remote_iov_count,
                  unsigned long _flags, bool iswrite) {
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), _pid,
                   _local_iov, _local_iov_count, _remote_iov, _remote_iov_count, _flags);
}

ssize_t process_v(pid_t __pid,   struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_readv_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

ssize_t process_vm_writev(pid_t __pid,   struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_writev_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

ssize_t process_v3(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count, const struct iovec *__remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite) {
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool ProcessRead(void *address, void *buffer, size_t size, bool write = false) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (pid == -1) {
        pid = getpid();
    }

    ssize_t bytes = process_v3(pid, local, 1, remote, 1, 0, write);
    return bytes == size;
}

bool PVM_ReadAddr(void *addr, void *buffer, size_t length) {
    return ProcessRead(addr, buffer, length, false);
}

template<typename T = uintptr_t>
T ReadMemory(uintptr_t addr) {
    T result{};
    if (PVM_ReadAddr((void *) addr, &result, sizeof(T))) {
        return result;
    }
    return result;
}

bool pvm2(uintptr_t address, void *buffer, size_t size, bool iswrite = false) {
    iovec local[1];
    iovec remote[1];
    local[0].iov_base = (void *)buffer;
    local[0].iov_len = size;
    remote[0].iov_base = (void *)address;
    remote[0].iov_len = size;
    if (pid < 0) {
        return false;
    }
    ssize_t bytes = syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), pid, local, 1, remote, 1, 0);
    return bytes == size;
}

void writeValue(uintptr_t address, void *value, ssize_t size) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = value;
    local[0].iov_len = size;
    remote[0].iov_base = (void *) address;
    remote[0].iov_len = size;
    syscall(process_vm_writev_syscall, pid, local, 1, remote, 1, 0);
}


template <typename T>
T Read(uintptr_t address) {
    T data;
    pvm2(address, reinterpret_cast<void*>(&data), sizeof(T));
    return data;
}

int pvm(uintptr_t address, void* buffer,int size) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = (void*)buffer;
	local[0].iov_len = size;
	remote[0].iov_base = (void*)address;
	remote[0].iov_len = size;

    ssize_t bytes = process_v(pid, local, 1, remote, 1, 0);
	return bytes == size;
}

bool ProcessRead1(void *address, void *buffer, size_t size, bool write = false) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = (void *)buffer;
    local[0].iov_len = size;
    remote[0].iov_base = (void *)address;
    remote[0].iov_len = size;

    if (pid < 0) {
        return false;
    }

    ssize_t bytes = syscall((write ? process_vm_writev_syscall : process_vm_readv_syscall), pid, local, 1, remote, 1, 0, write);
    return bytes == size;
}


bool PVM_WriteAddr(void *addr, void *buffer, size_t length) {
    return ProcessRead1(addr, buffer, length, true);
}

template<typename T>
void Write2(uintptr_t addr, T value) {
    PVM_WriteAddr((void *) addr, &value, sizeof(T));
}


bool pvms(void* address, void* buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    if (pid < 0) {
        return false;
    }
    ssize_t bytes = syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), pid, local, 1, remote, 1, 0);
    return bytes == size;
}

bool pvme(void *address, void *buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    if (pid < 0) {
        return false;
    }
    ssize_t bytes = process_v33(pid, local, 1, remote, 1, 0, iswrite);
    if (bytes == size) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
T Reads(uintptr_t address) {
    T data;
    pvme((void *) address, reinterpret_cast<void *>(&data), sizeof(T), false);
    return data;
}

template<typename T>
void Writes(uintptr_t address, T data) {
    pvme((void *) address, reinterpret_cast<void *>(&data), sizeof(T), true);
}

float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y, float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	return (float)(sqrt(x * x + y * y + z * z));
}

struct D3DMatrix ToMatrixWithScale(struct Vec3 translation,struct Vec3 scale,struct Vec4 rot)
 {
struct D3DMatrix m;
 m._41 = translation.X;
 m._42 = translation.Y;
 m._43 = translation.Z;
            

float x2 = rot.X + rot.X;
float y2 = rot.Y + rot.Y;
float z2 = rot.Z + rot.Z;


float xx2 = rot.X * x2;
float yy2 = rot.Y * y2;
float zz2 = rot.Z * z2;

m._11 = (1.0f - (yy2 + zz2)) * scale.X;
m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
m._33 = (1.0f - (xx2 + yy2)) * scale.Z;


float yz2 = rot.Y * z2;
float wx2 = rot.W * x2;

m._32 = (yz2 - wx2) * scale.Z;
m._23 = (yz2 + wx2) * scale.Y;

float xy2 = rot.X * y2;
float wz2 = rot.W * z2;

m._21 = (xy2 - wz2) * scale.Y;
m._12 = (xy2 + wz2) * scale.X;


float xz2 = rot.X * z2;
float wy2 = rot.W * y2;

m._31 = (xz2 + wy2) * scale.Z;
m._13 = (xz2 - wy2) * scale.X;


m._14 = 0.0f;
m._24 = 0.0f;
m._34 = 0.0f;
m._44 = 1.0f;

return m;
}
struct Vec3 mat2Cord(struct D3DMatrix pM1,struct D3DMatrix pM2){
struct  Vec3 pOut;

pOut.X = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
pOut.Y = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
pOut.Z = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
           
return pOut;
}
typedef int8_t BYTE;
uintptr_t getBase(){
    FILE *fp;
    uintptr_t addr = 0;
    char filename[40], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "libUE4.so")) {
                addr = (uintptr_t)strtoull(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

pid_t getPid(char * name){
    char text[69];
    pid_t pid = 0;
    sprintf(text,"pidof %s",name);
FILE *chkRun = popen(text, "r");
    if (chkRun){
        char output[10];
        fgets(output ,10,chkRun);
        pclose(chkRun);
        pid= atoi(output);
        }
    if (pid < 10) {
        DIR* dir = NULL;
        struct dirent* ptr = NULL;
        FILE* fp = NULL;
        char filepath[256];
        char filetext[128];
        dir = opendir("/proc");
        if (NULL != dir)
        {
            while ((ptr = readdir(dir)) != NULL)
            {
                if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                    continue;
                if (ptr->d_type != DT_DIR)
                    continue;
                sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
                fp = fopen(filepath, "r");
                if (NULL != fp)
                {
                    fgets(filetext, sizeof(filetext), fp);

                    if (strcmp(filetext, name) == 0)
                    {
                        fclose(fp);
                        break;
                    }
                    fclose(fp);
                }
            }
        }
        if (readdir(dir) == NULL)
        {
            closedir(dir);
            return 0;
        }
        closedir(dir);
        pid= atoi(ptr->d_name);
    }
        return pid;
}

int isapkrunning(char * name) {
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    FILE *fp = NULL;
    char filepath[50];
    char filetext[128];
    dir = opendir("/proc/");
    if (dir != NULL) {
        while ((ptr = readdir(dir)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (ptr->d_type != DT_DIR)
                continue;
            sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp) {
                fgets(filetext, sizeof(filetext), fp);
                if (strcmp(filetext, name) == 0) {
                    closedir(dir);
                    return 1;
                }
                fclose(fp);
            }
        }
    }
    closedir(dir);
    return 0;
}

int isValidItem(int id) {
	if (id >= 100000 && id < 999999)
		return 1;
    return 0;
}
float getF(uintptr_t address) {
	float buff;
	pvm(address, &buff, 4);
	return buff;
}

bool vm_readv(void* address, void* buffer, size_t size) {
    return pvms(address, buffer, size, false);
}

bool vm_writev(void* address, void* buffer, size_t size) {
    return pvms(address, buffer, size, true);
}

enum type {
    TYPE_DWORD,
    TYPE_FLOAT,
    TYPE_BYTE
};

float getFloat(uintptr_t address) {
   float var[1] = { 0 };
   vm_readv(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&var), 8);
   return var[0];
 }

void WriteDword(uintptr_t address, int value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void WriteFloat(uintptr_t address, float value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void WriteByte(uintptr_t address, BYTE value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void Write(uintptr_t address, const char *value, type TYPE){
    switch (TYPE) {
        case TYPE_DWORD:
            WriteDword(address, atoi(value));
            break;
        case TYPE_FLOAT:
            WriteFloat(address, atof(value));
            break;
        case TYPE_BYTE:
            WriteFloat(address, atof(value));
            break;    
    }
}

uintptr_t getA(uintptr_t address) {
	uintptr_t buff;
    pvm(address, &buff, SIZE);
    return buff;
}
int getI(uintptr_t address) {
	int buff;
	pvm(address, &buff, 4);
	return buff;
}
Vec3 getVec3(uintptr_t addr) {
    Vec3 vec;
    pvm(addr, &vec, sizeof(vec));
    return vec;
}
int isValid64(uintptr_t addr) {
    if (addr < 0x1000000000 || addr>0xefffffffff || addr % SIZE != 0)
        return 0;
    return 1;
}
int isValid32(uintptr_t addr) {
    if (addr < 0x10000000 || addr>0xefffffff || addr % SIZE != 0)
        return 0;
    return 1;
}
float getDistance(struct Vec3 mxyz,struct Vec3 exyz){
return sqrt ((mxyz.X-exyz.X)*(mxyz.X-exyz.X)+(mxyz.Y-exyz.Y)*(mxyz.Y-exyz.Y)+(mxyz.Z-exyz.Z)*(mxyz.Z-exyz.Z))/100;
}

float Distance(struct Vec3 myPos, struct Vec3 enPos)
{ 
    return sqrt((myPos.X - enPos.X) * (myPos.X - enPos.X) + (myPos.Y - enPos.Y) * (myPos.Y - enPos.Y) + (myPos.Z - enPos.Z) * (myPos.Z - enPos.Z));
}

struct Vec3 World2Screen(struct D3DMatrix viewMatrix, struct Vec3 pos) {
	struct Vec3 screen;
	float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z) + viewMatrix._44;

	if (screenW < 0.01f)
		screen.Z = 1;
	else
		screen.Z = 0;


	float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z) + viewMatrix._41;
	float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * pos.Z) + viewMatrix._42;
	screen.Y = (height / 2) - (height / 2) * screenY / screenW;
	screen.X = (width / 2) + (width / 2) * screenX / screenW;


	return screen;

}
struct Vec2 World2ScreenMain(struct D3DMatrix viewMatrix, struct Vec3 pos) {
	struct Vec2 screen;
	float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z) + viewMatrix._44;

	float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z) + viewMatrix._41;
	float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * pos.Z) + viewMatrix._42;
	screen.Y = (height / 2) - (height / 2) * screenY / screenW;
	screen.X = (width / 2) + (width / 2) * screenX / screenW;
	return screen;

}
struct D3DMatrix getOMatrix(uintptr_t boneAddr){
    float oMat[11];
    pvm(boneAddr,&oMat,sizeof(oMat));
    rot.X=oMat[0];
	rot.Y=oMat[1];
	rot.Z=oMat[2];
	rot.W=oMat[3];
			
	tran.X=oMat[4];
	tran.Y=oMat[5];
	tran.Z=oMat[6];
			
	scale.X=oMat[8];
	scale.Y=oMat[9];
	scale.Z=oMat[10];
			
	return ToMatrixWithScale(tran,scale,rot);
}

D2DVector getPointingAngle(uintptr_t uMyObject, uintptr_t ObjectAddrPointer, float object_x,float object_y, float object_z, float Self_x, float Self_y, float Self_z)
{
	D2DVector PointingAngle;

	
	float bulletVelocity = getF(getA(getA(uMyObject + 5124) + 0x900) + 0x31C);
	float ypjl = get_3D_Distance(object_x, object_y, object_z, Self_x, Self_y, Self_z);
	float xtime = ypjl / 88000;
	float ytime = ypjl / 88000;	// bulletVelocity

	uintptr_t playaddr = getA(ObjectAddrPointer + 0x140);
	float ypx = getF( playaddr + 0x1a0);
	float ypy = getF( playaddr + 0x1a0 +4);
    float ypz = getF( playaddr + 0x1a0 +8);


	
	object_x = object_x + ypx * xtime;
	object_y = object_y + ypy * xtime;
	object_z = object_z + 360 * ytime * ytime;


	float zbcx = object_x - Self_x;
	float zbcy = object_y - Self_y;
	float zbcz = object_z - Self_z;

	PointingAngle.X = atan2(zbcy, zbcx) * 180 / PI;	// 57.3
	PointingAngle.Y = atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI;
	return PointingAngle;

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

struct Vec2 World2RadarRectangle(struct FCameraCacheEntry CameraCache, struct Vec3 EnemyPos, struct Vec2 RadarPos, float RadarSize) {
    //Agak pusing dikit jir 2 jam bikin kode ini doang
    struct Vec2 results;
    FRotator qAngle = CameraCache.POV.Rotation;
    Vec3 CameraLoc = CameraCache.POV.Location;
    float flDeltaX = EnemyPos.X - CameraLoc.X;
	float flDeltaY = EnemyPos.Y - CameraLoc.Y;
	float flAngle = qAngle.Yaw;
	float flYaw = (flAngle) * (3.14159265358979323846f / 180.0);
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

struct Vec2 World2RadarRound(struct FCameraCacheEntry CameraCache, struct Vec3 EnemyPos, struct Vec2 RadarPos, float RadarSize) {
    //Remake the code for rounded minimap, Lumayan bikin pusing muter muter walaupun terlihat simpel hasilnya
    struct Vec2 results;
    FRotator qAngle = CameraCache.POV.Rotation;
    Vec3 CameraLoc = CameraCache.POV.Location;
    float flDeltaX = EnemyPos.X - CameraLoc.X;
	float flDeltaY = EnemyPos.Y - CameraLoc.Y;
	float flAngle = qAngle.Yaw;
	float flYaw = (flAngle) * (3.14159265358979323846f / 180.0);
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
        // Bikin Kode gini doang gw berjam jam mikir teorinya :)
        x = (x / vectorLength) * clipAngle;
        y = (y / vectorLength) * clipAngle;
    }
    results = {RadarPos.X + int(x / clipAngle * RadarSize), RadarPos.Y - int(y / clipAngle * RadarSize)};
    return results;//Code credits @SubaktiKesumaGMG & Unkowncheats for lil bit theory
}

char* getText(uintptr_t addr) {
	static char txt[42];
	memset(txt, 0, 42);
	char buff[41];
	pvm(addr + 4+SIZE, &buff, 41);
	int i;
	for (i = 0; i < 41; i++) {
		if (buff[i] == 0)
			break;

		txt[i] = buff[i];

		if (buff[i] == 67 && i > 10)
			break;

	}
	txt[i + 1] = '\0';
	return txt;
}

char* getNameByte(uintptr_t address)
{
    char static lj[64];
    memset(lj, 0, 64);
    unsigned short int nameI[32];
    pvm(address, nameI, sizeof(nameI));
    char s[10];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (nameI[i] == 0)
            break;
        sprintf(s, "%d:", nameI[i]);
        strcat(lj, s);
    }
    lj[63] = '\0';

    return lj;
}

std::string exec(std::string command) {
    char buffer[128];
    std::string result;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void dump(const uintptr_t gaddr, const int gsize, char* name) {
	char buff[0x100000];
	uintptr_t addr = gaddr;
	int size = gsize;
	FILE* fp = fopen(name, "w");
	while (size > 0) {
		if (size < 0x100000) {
			pvm(addr, buff, size);

			for (int i = 0; i < size; i++)
				fwrite(&buff[i], 1, 1, fp);

		}
		else {
			pvm(addr, buff, 0x100000);

			for (int i = 0; i < 0x100000; i++)
				fwrite(&buff[i], 1, 1, fp);
		}

		addr += 0x100000;
		size -= 0x100000;
	}
	fclose(fp);

}
