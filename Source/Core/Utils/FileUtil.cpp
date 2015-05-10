//
//  FileUtil.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "FileUtil.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>    // Needed for application path
#include <dirent.h>         // Needed for directory structures
#endif

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <xstring>
#include "StringUtil.h"     // Needed for Windows paths
#endif

#include <sys/stat.h>       // Creating folders
#include <sys/types.h>      // Creating folders
#include <sstream>          // string stream

std::string FileUtil::relativePath = "PATH_NOT_SET";
std::string FileUtil::GetPath() { return relativePath; };

#if defined(_WIN32)
// ----> TODO: Use console instead of printf
// ----> TODO: Fix for windows - functions moved to StringUtils class

//========================================================================
// SetRelativePath()
// Puts the path for the .app (OSX) or .exe (Windows) into a string
//========================================================================
void FileUtil::SetRelativePath()
{
    TCHAR path[MAX_PATH];
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int pathLength = GetModuleFileName(hInstance, path, MAX_PATH);
    if (pathLength != 0) {
        std::wstring widePath( path, pathLength );
        std::string utf8Path = utf8_encode( widePath );
        // Cull executable name from path
        int found=utf8Path.find_last_of("/\\") + 1; //leave last forwardslash
        relativePath = utf8Path.substr(0,found);
        printf( "runtime path: %s \n", relativePath.c_str() );
    } else {
        relativePath = "FAILTRAIN, ALL ABOARD!";
        printf("SysCore couldn't get path, length returned 0");
    }
}
//========================================================================
// GetFiles()
// retrieves list of files in dir
//========================================================================
bool FileUtil::GetFiles(const std::string dir, std::vector<std::string> &fileNames)
{
    WIN32_FIND_DATA FindFileData;
    wchar_t FileName[260];
    string2wchar_t(dir, FileName);
    HANDLE hFind = FindFirstFile(FileName, &FindFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    fileNames.push_back(wchar_t2string(FindFileData.cFileName));
    
    while (FindNextFile(hFind, &FindFileData))
        fileNames.push_back(wchar_t2string(FindFileData.cFileName));
    
    return true;
}
//========================================================================
// GetAllFiles()
// retrieves list of all files in dir
//========================================================================
bool FileUtil::GetAllFiles(const std::string dir, std::vector<std::string> &fileNames)
{
    std::string directory = dir;
    directory.append("*");
    return GetFiles(directory, fileNames);
}
//========================================================================
// GetFilesOfType()
// retrieves list of files with name ending in type in dir
//========================================================================
bool FileUtil::GetFilesOfType(const std::string dir, const std::string type, std::vector<std::string> &fileNames)
{
    std::string directory = dir;
    directory.append("*");
    directory.append(type);
    return GetFiles(directory, fileNames);;
}
//========================================================================
// DoesFolderExist()
// checks if folder at path exists
//========================================================================
bool FileUtil::DoesFolderExist(const std::string dir) {
    DWORD ftyp = GetFileAttributesA(dir.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES &&
            (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}
//========================================================================
// CreateFolder()
// Creates folder at path unless exists
//========================================================================
bool FileUtil::CreateFolder(const std::string dir) {
    if (DoesFolderExist(dir)) return false;
    
    wchar_t FileName[260];
    string2wchar_t(dir, FileName);
    
    CreateDirectory(FileName, NULL);
    return true;
}
//========================================================================
// DoesFileExist()
// checks if file at path exists
//========================================================================
bool FileUtil::DoesFileExist( const std::string dir, const std::string fileName )
{
    std::string dirAndName = dir;
    dirAndName.append( fileName );
    
    WIN32_FIND_DATA FindFileData;
    //wchar_t * FilePath = string2wchar_t(dirAndName);
    wchar_t FilePath[260];
    string2wchar_t(dirAndName, FilePath);
    HANDLE hFind = FindFirstFile(FilePath, &FindFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    return true;
}
#elif defined __APPLE__
//========================================================================
// SetRelativePath()
// Puts the path for the .app (OSX) or .exe (Windows) into a string
//========================================================================
void FileUtil::UpdateRelativePath()
{
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        std::string str(path);
        // Cull last few folders
        size_t found=str.find_last_of("/\\");
        found = str.substr(0,found).find_last_of("/\\");
        found = str.substr(0,found).find_last_of("/\\");
        found = str.substr(0,found).find_last_of("/\\") + 1; //leave last forwardslash
        //printf("relative path is %s, %i\n", str.substr(0,found).c_str() , found);
        relativePath = str.substr(0,found);
    } else {
        relativePath = "FAILTRAIN, ALL ABOARD!";
        printf("SysCore couldn't get path, buffer too small; need size %u\n", size);
    }
}
//========================================================================
// GetAllFiles()
// retrieves list of all files in dir
//========================================================================
bool FileUtil::GetAllFiles(const std::string dir, std::vector<std::string> &fileNames)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        printf("Error opening directory: %s\n", dir.c_str() );
        return false;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        fileNames.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return true;
}
//========================================================================
// GetFilesOfType()
// retrieves list of files with name ending in type in dir
//========================================================================
bool FileUtil::GetFilesOfType(const std::string dir, const std::string type, std::vector<std::string> &fileNames)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        printf("Error opening directory: %s\n", dir.c_str() );
        return false;
    }
    size_t typeLen = type.length();
    if ( typeLen > 0 ) {
        while ((dirp = readdir(dp)) != NULL) {
            size_t fileNameLen = strlen(dirp->d_name);
            if ( fileNameLen > typeLen ) {
                const char *endChars = &dirp->d_name[fileNameLen-typeLen];
                if ( strcmp( type.c_str(), endChars ) == 0 ) {
                    fileNames.push_back(std::string(dirp->d_name));
                }
            }
        }
    }
    closedir(dp);
    return true;
}
//========================================================================
// DoesFolderExist()
// checks if folder at path exists
//========================================================================
bool FileUtil::DoesFolderExist( const std::string dir ) {
    DIR *dp;
    if((dp  = opendir( dir.c_str())) == NULL) {
        printf("Can't open directory: %s\n", dir.c_str() );
        return false;
    } else {
        closedir(dp);
    }
    return true;
}
//========================================================================
// DoesFileExist()
// checks if file at path exists
//========================================================================
bool FileUtil::DoesFileExist( const std::string dir, const std::string fileName ) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir( dir.c_str())) == NULL) {
        printf("Can't open directory: %s\n", dir.c_str() );
        return false;
    } else {
        size_t nameLen = strlen( fileName.c_str() );
        if ( nameLen > 0 ) {
            while ((dirp = readdir(dp)) != NULL) {
                size_t fileNameLen = strlen(dirp->d_name);
                if ( fileNameLen == nameLen ) {
                    if ( strcmp( fileName.c_str(), dirp->d_name ) == 0 ) {
                        closedir(dp);
                        return true;
                    }
                }
            }
        }
        closedir(dp);
    }
    return false;
}
//========================================================================
// CreateFolder()
// Creates folder at path unless exists
//========================================================================
bool FileUtil::CreateFolder( const std::string dir ) {
    DIR *dp;
    if( (dp = opendir( dir.c_str())) == NULL ) {
        mode_t process_mask = umask(0);
        int result_code = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
        umask(process_mask);
        if ( result_code != 0 ) printf("[SysCore] Can't create directory: %s\n", dir.c_str() );
        else return true;
    } else {
        closedir(dp);
        printf("[SysCore] Directory already exists: %s\n", dir.c_str() );
        return true;
    }
    return false;
}
#endif

