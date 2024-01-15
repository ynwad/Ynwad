#include "utils.h"
#include <string.h>
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <cstringt.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <execinfo.h>
#include <sys/syscall.h>
#include <dirent.h>

#endif

#include "log.h"

namespace sylar {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

pid_t GetThreadId() {
    return syscall(SYS_gettid);
}

uint32_t GetFiberId() {
    // return sylar::Fiber::GetFiberId();
    // 暂未实现协程。
    return 0;
}

#ifdef _WIN32

LPCWSTR stdStrToWideChar(std::string str) {
	// 将 std::string 转换为 LPCWSTR
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* wideStr = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideStr, size);
	return wideStr;
}

static bool __mkdir(LPCTSTR wStrPath) {
	if ((NULL == wStrPath) || (0 == _tcslen(wStrPath)))
	{
		return FALSE;
	}

	if ((TRUE == PathFileExists(wStrPath)) || (TRUE == PathIsRoot(wStrPath)))
	{
		return TRUE;
	}
	TCHAR szParentpath[MAX_PATH] = _T("");
	_tcscpy_s(szParentpath, _countof(szParentpath), wStrPath);

	TCHAR* pTmp = PathRemoveBackslash(szParentpath);//去除路径最后的反斜杠
	if (NULL == pTmp)
	{
		return FALSE;
	}

	BOOL bRet = PathRemoveFileSpec(szParentpath);//将路径末尾的文件名或文件夹和反斜杠去掉

	if (0 == _tcscmp(wStrPath, szParentpath))
	{
		return FALSE;
	}

	if (__mkdir(szParentpath))//递归创建直到上一层存在或是根目录
	{
		return CreateDirectory(wStrPath, NULL);
	}
	else
	{
		return FALSE;
	}
}

bool Mkdir(LPCTSTR dirname) {
	DWORD attributes = GetFileAttributesW(dirname);
	if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
		return true;
	}
	//搜索‘/’在字符串中第一次出现的位置
	return __mkdir(dirname);
}

#else
static int __lstat(const char* file, struct stat* st = nullptr) {
    struct stat lst;
    int ret = lstat(file, &lst);
    if(st) {
        *st = lst;
    }
    return ret;
}

static int __mkdir(const char* dirname) {
    if(access(dirname, F_OK) == 0) {
        return 0;
    }
    return mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void FSUtil::ListAllFile(std::vector<std::string>& files
                            ,const std::string& path
                            ,const std::string& subfix){
    if(access(path.c_str(), 0) != 0){
        return;
    }
    DIR* dir = opendir(path.c_str());
    if(dir == nullptr){
        return;
    }
    struct dirent* dp = nullptr;
    while((dp = readdir(dir)) != nullptr){
        if(dp->d_type == DT_DIR){
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
                continue;
            }
            ListAllFile(files, path + "/" + dp->d_name, subfix);
        }
        else if(dp->d_type == DT_REG){
            std::string filename(dp->d_name);
            if(subfix.empty()) {
                files.push_back(path + "/" + filename);
            } else {
                if(filename.size() < subfix.size()) {
                    continue;
                }
                if(filename.substr(filename.length() - subfix.size()) == subfix) {
                    files.push_back(path + "/" + filename);
                }
            }
        }
    }
    closedir(dir);
}

bool FSUtil::Mkdir(const std::string& dirname) {
    if(std::filesystem::is_directory(dirname.c_str())) {
        return true;
    } 
    //strdup()，复制string，返回char*
    char* path = strdup(dirname.c_str());

    //搜索‘/’在字符串中第一次出现的位置
    char* ptr = strchr(path + 1, '/');
    do {
        for(; ptr; *ptr = '/', ptr = strchr(ptr + 1, '/')) {
            *ptr = '\0';
            if(__mkdir(path) != 0) {
                break;
            }
        }
        if(ptr != nullptr) {
            break;
        } else if(__mkdir(path) != 0) {
            break;
        }
        free(path);
        return true;
    } while(0);
    free(path);
    return false;
}

#endif

bool FSUtil::OpenForWrite(std::ofstream& ofs, const std::string& filename
                        ,std::ios_base::openmode mode) {
    ofs.open(filename.c_str(), mode);   
    if(!ofs.is_open()) {
        std::string dir = Dirname(filename);
        #ifdef _WIN32
            Mkdir(stdStrToWideChar(folderPath))
        #else
            Mkdir(dir);
        #endif
        ofs.open(filename.c_str(), mode);
    }
    return ofs.is_open();
}

std::string FSUtil::Dirname(const std::string& filename){
    if(filename.empty()) {
        return ".";
    }
    auto pos = filename.rfind('/');
    if(pos == 0) {
        return "/";
    } else if(pos == std::string::npos) {
        return ".";
    } else {
        return filename.substr(0, pos);
    }
}
}