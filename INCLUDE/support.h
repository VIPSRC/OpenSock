#ifndef SUPPORT_H
#define SUPPORT_H

#include "socket.h"
#define PI 3.141592653589793238

ssize_t process_v(pid_t __pid, struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_readv_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

ssize_t process_v2(pid_t __pid, struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_writev_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
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

void pvm2(uintptr_t address, void* buffer,int size) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = (void*)buffer;
	local[0].iov_len = size;
	remote[0].iov_base = (void*)address;
	remote[0].iov_len = size;

	process_v2(pid, local, 1, remote, 1, 0);
}



FMatrix ToMatrixWithScale(const Vec3 &translation, const Vec3 &scale, const Vec4 &rot) {
	FMatrix m;

	m.M[3][0] = translation.X;
	m.M[3][1] = translation.Y;
	m.M[3][2] = translation.Z;

	float x2 = rot.X + rot.X;
	float y2 = rot.Y + rot.Y;
	float z2 = rot.Z + rot.Z;

	float xx2 = rot.X * x2;
	float yy2 = rot.Y * y2;
	float zz2 = rot.Z * z2;

	m.M[0][0] = (1.0f - (yy2 + zz2)) * scale.X;
	m.M[1][1] = (1.0f - (xx2 + zz2)) * scale.Y;
	m.M[2][2] = (1.0f - (xx2 + yy2)) * scale.Z;

	float wx2 = rot.W * x2;
	float yz2 = rot.Y * z2;
	m.M[1][2] = (yz2 + wx2) * scale.Y;
	m.M[2][1] = (yz2 - wx2) * scale.Z;

	float xy2 = rot.X * y2;
	float wz2 = rot.W * z2;
	m.M[0][1] = (xy2 + wz2) * scale.X;
	m.M[1][0] = (xy2 - wz2) * scale.Y;

	float xz2 = rot.X * z2;
	float wy2 = rot.W * y2;
	m.M[0][2] = (xz2 - wy2) * scale.X;
	m.M[2][0] = (xz2 + wy2) * scale.Z;

	m.M[0][3] = 0.0f;
	m.M[1][3] = 0.0f;
	m.M[2][3] = 0.0f;
	m.M[3][3] = 1.0f;

	return m;
}

Vec3 mat2Cord(const FMatrix &pM1, const FMatrix &pM2) {
	return Vec3(
        (pM1.M[3][0] * pM2.M[0][0] + pM1.M[3][1] * pM2.M[1][0] + pM1.M[3][2] * pM2.M[2][0] + pM1.M[3][3] * pM2.M[3][0]),
        (pM1.M[3][0] * pM2.M[0][1] + pM1.M[3][1] * pM2.M[1][1] + pM1.M[3][2] * pM2.M[2][1] + pM1.M[3][3] * pM2.M[3][1]),
        (pM1.M[3][0] * pM2.M[0][2] + pM1.M[3][1] * pM2.M[1][2] + pM1.M[3][2] * pM2.M[2][2] + pM1.M[3][3] * pM2.M[3][2])
    );
}

uintptr_t getBase() {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[40], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "libUE4.so")) {
                addr = (uintptr_t)strtoul(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

pid_t getPid(char * name) {
	char text[69];
	pid_t pid = 0;
	sprintf(text,"pidof %s",name);
	FILE *chkRun = popen(text, "r");
	if (chkRun) {
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
		if (NULL != dir) {
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
						fclose(fp);
						break;
					}
					fclose(fp);
				}
			}
		}
		if (readdir(dir) == NULL) {
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

bool isValidPlayer(int oType) {
    return (oType == 0x3ec2a00 || oType == 0x3ec2800);
}

uintptr_t getA(uintptr_t address) {
	uintptr_t buff;
    pvm(address, &buff, SIZE);
    return buff;
}

Vec3 getVec3(uintptr_t addr) {
    Vec3 vec;
    pvm(addr, &vec, sizeof(vec));
    return vec;
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
Vec3 CalcMousePos(Vec3 headPos, Vec3 uMyobejctPos) {
    Vec3 AimPos;
    float x = headPos.X - uMyobejctPos.X;
    float y = headPos.Y - uMyobejctPos.Y;
    float z = headPos.Z - uMyobejctPos.Z;
    AimPos.X = atan2(y, x) * 180.f / M_PI;
    AimPos.Y = atan2(z, sqrt(x * x + y * y)) * 180.f / PI;
    return AimPos;
}

template<typename T>
T Read(uintptr_t address) {
    T data;
    pvm(address, &data, sizeof(T));
    return data;
}

int isValid(uintptr_t addr) {
    return (addr >= 0x1000000000 || addr <= 0xEFFFFFFFFF || addr % SIZE == 0);
}

bool isValidPosition(const Vec2 &location) {
    return (location.X < width && location.Y < height && location.X > 0 && location.Y > 0);
}

float getDistance(const Vec3 &mxyz, const Vec3 &exyz) {
	return sqrt((mxyz.X - exyz.X) * (mxyz.X - exyz.X) + (mxyz.Y - exyz.Y) * (mxyz.Y - exyz.Y) + (mxyz.Z - exyz.Z) * (mxyz.Z - exyz.Z)) / 100;
}

float getDistance3d(const Vec3 &mxyz, const Vec3 &exyz) {
    return sqrt((mxyz.X - exyz.X) * (mxyz.X - exyz.X) + (mxyz.Y - exyz.Y) * (mxyz.Y - exyz.Y) + (mxyz.Z - exyz.Z) * (mxyz.Z - exyz.Z));
}

FMatrix RotToMatrix(const FRotator &rotation) {
    float radPitch = rotation.Pitch * ((float)M_PI / 180.0f);
    float radYaw = rotation.Yaw * ((float)M_PI / 180.0f);
    float radRoll = rotation.Roll * ((float)M_PI / 180.0f);

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
    matrix.M[0][3] = 0.0f;

    matrix.M[1][0] = (SR * SP * CY - CR * SY);
    matrix.M[1][1] = (SR * SP * SY + CR * CY);
    matrix.M[1][2] = (-SR * CP);
    matrix.M[1][3] = 0.0f;

    matrix.M[2][0] = (-(CR * SP * CY + SR * SY));
    matrix.M[2][1] = (CY * SR - CR * SP * SY);
    matrix.M[2][2] = (CR * CP);
    matrix.M[2][3] = 0.0f;

    matrix.M[3][0] = 0.0f;
    matrix.M[3][1] = 0.0f;
    matrix.M[3][2] = 0.0f;
    matrix.M[3][3] = 1.0f;

    return matrix;
}

Vec2 WorldToScreen(const Vec3 &worldLocation, const MinimalViewInfo &camViewInfo, int width, int height) {
    FMatrix tempMatrix = RotToMatrix(camViewInfo.Rotation);

    Vec3 vAxisX(tempMatrix.M[0][0], tempMatrix.M[0][1], tempMatrix.M[0][2]);
    Vec3 vAxisY(tempMatrix.M[1][0], tempMatrix.M[1][1], tempMatrix.M[1][2]);
    Vec3 vAxisZ(tempMatrix.M[2][0], tempMatrix.M[2][1], tempMatrix.M[2][2]);

    Vec3 vDelta = worldLocation - camViewInfo.Location;

    Vec3 vTransformed(Vec3::Dot(vDelta, vAxisY), Vec3::Dot(vDelta, vAxisZ), Vec3::Dot(vDelta, vAxisX));

    if (vTransformed.Z < 1.0f) {
        vTransformed.Z = 1.0f;
    }

    float fov = camViewInfo.FOV;
    float screenCenterX = (width / 2.0f);
    float screenCenterY = (height / 2.0f);

    return Vec2(
        (screenCenterX + vTransformed.X * (screenCenterX / tanf(fov * ((float)M_PI / 360.0f))) / vTransformed.Z),
        (screenCenterY - vTransformed.Y * (screenCenterX / tanf(fov * ((float)M_PI / 360.0f))) / vTransformed.Z)
    );
}

Vec2 WorldToRadar(float yaw, const Vec3 &enemyPos, const Vec3 &localPos) {
    float theta = (yaw - 90) * ((float)M_PI / 180.0f);
    float s = (float) std::sin(theta);
    float c = (float) std::cos(theta);
    
    float radarObjectX = (localPos.X - enemyPos.X) / 170;
    float radarObjectY = (localPos.Y - enemyPos.Y) / 170;
    
    return Vec2(
        (radarObjectX * c + radarObjectY * s),
        (-radarObjectX * s + radarObjectY * c)
    );
}

FMatrix getOMatrix(uintptr_t boneAddr) { 
	float oMat[11];
    pvm(boneAddr, &oMat, sizeof(oMat));
	
    rot.X = oMat[0];
	rot.Y = oMat[1];
	rot.Z = oMat[2];
	rot.W = oMat[3];
			
	tran.X = oMat[4];
	tran.Y = oMat[5];
	tran.Z = oMat[6];
			
	scale.X = oMat[8];
	scale.Y = oMat[9];
	scale.Z = oMat[10];
			
	return ToMatrixWithScale(tran, scale, rot);
}

FMatrix getHeadOMatrix(uintptr_t boneAddr) { 
	float oMat[11];
    pvm(boneAddr, &oMat, sizeof(oMat));
	
    rot.X = oMat[0];
	rot.Y = oMat[1];
	rot.Z = oMat[2];
	rot.W = oMat[3];
			
	tran.X = oMat[4];
	tran.Y = oMat[5];
	tran.Z = oMat[6];
			
	scale.X = oMat[8];
	scale.Y = oMat[9];
	scale.Z = oMat[10];
			
	return ToMatrixWithScale(tran, scale, rot);
}

FMatrix getOMatrix2(uintptr_t boneAddr, float factor) { 
	float oMat[11];
    pvm(boneAddr, &oMat, sizeof(oMat));
	
    rot.X = oMat[0];
	rot.Y = oMat[1];
	rot.Z = oMat[2];
	rot.W = oMat[3];
			
	tran.X = oMat[4];
	tran.Y = oMat[5];
	tran.Z = oMat[6];
			
	scale.X = oMat[8] * factor;
	scale.Y = oMat[9] * factor;
	scale.Z = oMat[10] * factor;
			
	return ToMatrixWithScale(tran, scale, rot);
}

Vec2 getPointingAngle(const Vec3 &camera, const Vec3 &xyz) {
	Vec2 PointingAngle;

	float zbcx = xyz.X - camera.X;
	float zbcy = xyz.Y - camera.Y;
	float zbcz = xyz.Z - camera.Z;
	PointingAngle.Y = atan2(zbcy, zbcx) * 180 / PI;
	PointingAngle.X = atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI;

	return PointingAngle;
}

char* getText(uintptr_t addr) {
	static char txt[42];
	memset(txt, 0, 42);
	char buff[41];
	pvm(addr + 4 + SIZE, &buff, 41);
	int i;
	for (i = 0; i < 41; i++) {
		if (buff[i] == 0) {
            break;
        }
		txt[i] = buff[i];
	}
	txt[i + 1] = '\0';
	return txt;
}

char *getNameByte(uintptr_t address) {
	char static lj[64];
	memset(lj, 0, 64);
	unsigned short int nameI[32];
	pvm(address, nameI, sizeof(nameI));
	char s[10];
	for (int i = 0; i < 32; i++) {
		if (nameI[i] == 0) {
			break;
        }
		sprintf(s, "%d:", nameI[i]);
		strcat(lj, s);
	}
	lj[63] = '\0';

	return lj;
}

template<typename T>
void Write(uintptr_t addr, T data) {
    iovec l_iov{&data, sizeof(T)};
    iovec r_iov{reinterpret_cast<void *>(addr), sizeof(T)};
    syscall(process_vm_writev_syscall, pid, &l_iov, 1, &r_iov, 1, 0);
}

#endif
