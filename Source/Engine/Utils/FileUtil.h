//
//  FileUtil.h
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__FileUtil__
#define __DungeonSmith__FileUtil__

#include <string>
#include <vector>

class FileUtil {
public:
    // Utility functions for file system
    static bool GetAllFiles(const std::string dir, std::vector<std::string> &fileNames);
    static bool GetFilesOfType(const std::string dir, const std::string type, std::vector<std::string> &fileNames);
    static bool DoesFolderExist( const std::string dir );
    static bool DoesFileExist( const std::string dir, const std::string fileName );
    static bool CreateFolder( const std::string dir );
    // Application file system path
	static std::string GetPath();
    static void UpdateRelativePath();
private:
    static std::string relativePath;
};

#endif /* defined(__DungeonSmith__FileUtil__) */
