#include "AlgoDirectory.h"

#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

namespace Algo1010
{
    // 判断目录是否存在
    bool AlgoDirectory::directoryExists(const std::string &path)
    {
        if (::access(path.c_str(), F_OK) != 0)
        {
            return false;
        }
        return true;
    }

    // 创建目录
    bool AlgoDirectory::createDirectory(const std::string &path)
    {
        if (::mkdir(path.c_str(), 0777) == 0)
        {
            printf("create dir successed: %s\n", path.c_str());
            return true;
        }
        printf("create dir failed: %s\n", path.c_str());
        return false;
    }

    // 判断文件是否存在
    bool AlgoDirectory::fileExists(const std::string &path)
    {
        std::ifstream file(path);
        if (file)
        {
            file.close();
            return true;
        }
        return false;
    }

    // 创建文件
    bool AlgoDirectory::createFile(const std::string &path, const std::string header)
    {
        std::ofstream file(path);
        if (file)
        {
            if (!header.empty())
            {
                file.write(header.c_str(), header.length());
            }

            file.close();
            printf("create file successed: %s\n", path.c_str());
            return true;
        }
        printf("create file failed: %s\n", path.c_str());
        return false;
    }

    // 删除目录
    bool AlgoDirectory::deleteDirectory(const std::string &path)
    {
        if (::rmdir(path.c_str()) == 0)
        {
            printf("delete dir successed: %s\n", path.c_str());
            return true;
        }
        printf("delete dir successed: %s\n", path.c_str());
        return false;
    }

    // 删除文件
    bool AlgoDirectory::deleteFile(const std::string &path)
    {
        if (::remove(path.c_str()) == 0)
        {
            printf("delete file successed: %s\n", path.c_str());
            return true;
        }
        printf("delete file successed: %s\n", path.c_str());
        return false;
    }

    // 通过全路径获取文件名
    std::string AlgoDirectory::getFileName(const std::string &path)
    {
        char *fileName = basename(const_cast<char *>(path.c_str()));
        return std::string(fileName);
    }

    // 通过全路径获取路径
    std::string getDirectory(const std::string &path)
    {
        char *dirName = ::dirname(const_cast<char *>(path.c_str()));
        return std::string(dirName);
    }

    // 通过全路径获取文件扩展名
    std::string AlgoDirectory::getFileExtension(const std::string &path)
    {
        size_t pos = path.find_last_of('.');
        if (pos != std::string::npos)
        {
            return path.substr(pos + 1);
        }
        return "";
    }

}