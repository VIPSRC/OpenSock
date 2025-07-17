#ifndef INCLUDES_H
#define INCLUDES_H

#include <sys/uio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <sys/un.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
#include <thread>
#include <cmath>
#include "Offsets.h"


#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

struct FMatrix {
    float M[4][4];
};

struct Vec4 {
    float X, Y, Z, W;
};

struct Vec3 {
    float X, Y, Z;
    
    Vec3() : X(0), Y(0), Z(0) {}
    Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    
    static inline float Dot(Vec3 lhs, Vec3 rhs) {
        return (lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z);
    }
    
    inline Vec3 operator-(const Vec3 &rhs) const {
        return Vec3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
	}
};

struct Vec2 {
    float X, Y;
    
    Vec2() : X(0), Y(0) {}
    Vec2(float x, float y) : X(x), Y(y) {}
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

struct ItemDefineID {
    int Type;
    int TypeSpecificID;
    bool bValidItem;
    bool bValidInstance;
	uint64_t InstanceID;
};

struct PickUpItemData {
    ItemDefineID ID;
    int Count;
    char useless[0x10];
	int InstanceID;
};

struct TArray {
    uintptr_t data;
    int count;
    int max;
};

Vec4 rot;
Vec3 scale, tran;

int height = 1080, width = 2340, pid = 0;
float mx = 0, my = 0, mz = 0;

struct Ulevel {
    uintptr_t addr;
    int size;
};

#define SIZE sizeof(uintptr_t)

#endif
