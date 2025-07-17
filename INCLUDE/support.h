#include "socket.h"

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

bool VMRead(void* address, void* buffer, size_t size) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    ssize_t bytes = syscall(process_vm_readv_syscall, pid, local, 1, remote, 1, 0);
    return bytes == size;
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

void WriteDword(uintptr_t address, int value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void WriteFloat(uintptr_t address, float value){
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
    }
}

uintptr_t getUE4() {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "libUE4.so")) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

pid_t getPid(char * name)
{
	char text[69];
	pid_t pid = 0;
	sprintf(text,"pidof %s",name);
	FILE *chkRun = popen(text, "r");
	if (chkRun)
	{
		char output[10];
		fgets(output ,10,chkRun);
		pclose(chkRun);
		pid= atoi(output);
	}
	if (pid < 10)
	{
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

int isValid64(uintptr_t addr) {
    if (addr < 0x1000000000 || addr>0xefffffffff || addr % SIZE != 0)
        return 0;
    return 1;
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
