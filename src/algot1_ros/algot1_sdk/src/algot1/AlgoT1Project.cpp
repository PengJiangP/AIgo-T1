#include "AlgoProject.h"

#include "uliti/AlgoBaseUliti.h"
#include "uliti/AlgoSystemTime.h"
#include "uliti/AlgoDirectory.h"

namespace Algo1010
{

    bool AlgoT1Project::create(std::string rootPath)
    {
        if (!rootPath.empty())
            m_rootPath = rootPath;

        double currentTime = AlgoSystemTime::getCurrentTime();
        std::string ymd = AlgoSystemTime::getFormattedDate(currentTime);
        std::string hms = AlgoSystemTime::getFormattedTime(currentTime);

        // project dir and name
        m_curYmdDir = m_rootPath + "/" + ymd + "/";
        m_curYmdHmsDir = m_rootPath + "/" + ymd + "/" + hms + "/";
        m_projectPath = m_curYmdHmsDir;
        m_projectName = ymd + "_" + hms;

        // image dir and filename
        m_imageRootName = m_projectName + "image";
        m_imageRootDir = m_projectPath + "/" + m_imageRootName;
        m_imageDataDir0 = m_imageRootDir + "/" + m_imageDataDirName0;     // path/cam0
        m_cam0DescPathName = m_imageRootDir + "/" + m_imageDataDescName0; // path/description0.txt
        m_imageDataDir1 = m_imageRootDir + "/" + m_imageDataDirName1;     // path/cam1
        m_cam1DescPathName = m_imageRootDir + "/" + m_imageDataDescName1; // path/description1.txt

        // gnss+imu+fusion filename
        m_gnss1PathName = m_projectPath + m_projectName + ".algo";
        m_gnss2PathName = m_projectPath + m_projectName + "_aux.algo";
        m_memsPathName = m_projectPath + m_projectName + ".algoimua";
        m_rtcmPathName = m_projectPath + m_projectName + "_rtcm.algo";
        m_ggaPathName = m_projectPath + m_projectName + "_gga.txt";
        m_rmcPathName = m_projectPath + m_projectName + "_rmc.txt";
        m_inspvaxaPathName = m_projectPath + m_projectName + "_inspvaxa.txt";
        m_pppb2bPathName = m_projectPath + m_projectName + "_pppb2b.b2b";

        m_posPathName = m_projectPath + m_projectName + "_gga.pos";
        m_vinsPathName = m_projectPath + m_projectName + "_vins.txt";

        // create new folder
        if (!AlgoDirectory::directoryExists(m_rootPath))
        {
            AlgoDirectory::createDirectory(m_rootPath);
        }

        if (!AlgoDirectory::directoryExists(m_curYmdDir))
        {
            AlgoDirectory::createDirectory(m_curYmdDir);
        }

        if (!AlgoDirectory::directoryExists(m_curYmdHmsDir))
        {
            AlgoDirectory::createDirectory(m_curYmdHmsDir);
        }

        if (!AlgoDirectory::directoryExists(m_imageRootDir))
        {
            AlgoDirectory::createDirectory(m_imageRootDir);
        }

        if (!AlgoDirectory::directoryExists(m_imageDataDir0))
        {
            AlgoDirectory::createDirectory(m_imageDataDir0);
        }

        if (!AlgoDirectory::directoryExists(m_imageDataDir1))
        {
            AlgoDirectory::createDirectory(m_imageDataDir1);
        }

        std::string imageDescHeader = "#filename,week,sec,#datetime validMark dpps_sec pps_sec ppstickms curtickms#\n";
        if (!AlgoDirectory::fileExists(m_cam0DescPathName))
        {
            AlgoDirectory::createFile(m_cam0DescPathName, imageDescHeader);
        }
        if (!AlgoDirectory::fileExists(m_cam1DescPathName))
        {
            AlgoDirectory::createFile(m_cam1DescPathName, imageDescHeader);
        }

        // create new file
        if (!AlgoDirectory::fileExists(m_gnss1PathName))
        {
            AlgoDirectory::createFile(m_gnss1PathName);
        }

        if (!AlgoDirectory::fileExists(m_gnss2PathName))
        {
            AlgoDirectory::createFile(m_gnss2PathName);
        }

        if (!AlgoDirectory::fileExists(m_memsPathName))
        {
            AlgoDirectory::createFile(m_memsPathName);
        }
        if (!AlgoDirectory::fileExists(m_rtcmPathName))
        {
            AlgoDirectory::createFile(m_rtcmPathName);
        }

        if (!AlgoDirectory::fileExists(m_ggaPathName))
        {
            AlgoDirectory::createFile(m_ggaPathName);
        }

        if (!AlgoDirectory::fileExists(m_rmcPathName))
        {
            AlgoDirectory::createFile(m_rmcPathName);
        }

        if (!AlgoDirectory::fileExists(m_inspvaxaPathName))
        {
            AlgoDirectory::createFile(m_inspvaxaPathName);
        }

        if (!AlgoDirectory::fileExists(m_pppb2bPathName))
        {
            AlgoDirectory::createFile(m_pppb2bPathName);
        }

        return true;
    }

    bool AlgoT1Project::open(std::string projectPath, std::string projectName)
    {
        // TODO
        if (!AlgoDirectory::directoryExists(projectPath))
        {
            printf("project not exist:%s", projectPath.c_str());
            return false;
        }

        // m_rootPath = "C:/AlgoUserData"; // config key = "rootdir"
        // m_yyyymmdd = "";                // yyyymmddd
        // m_hhmmss = "";                  // hhmmss

        m_projectPath = projectPath;
        m_projectName = projectName;

        // image dir and filename
        m_imageRootName = m_projectName + "image";
        m_imageRootDir = m_projectPath + "/" + m_imageRootName;
        m_imageDataDir0 = m_imageRootDir + "/" + m_imageDataDirName0;     // path/cam0
        m_cam0DescPathName = m_imageRootDir + "/" + m_imageDataDescName0; // path/description0.txt
        m_imageDataDir1 = m_imageRootDir + "/" + m_imageDataDirName1;     // path/cam1
        m_cam1DescPathName = m_imageRootDir + "/" + m_imageDataDescName1; // path/description1.txt

        // gnss+imu+fusion filename
        m_gnss1PathName = m_projectPath + m_projectName + ".algo";
        m_gnss2PathName = m_projectPath + m_projectName + "_aux.algo";
        m_memsPathName = m_projectPath + m_projectName + ".algoimua";
        m_rtcmPathName = m_projectPath + m_projectName + "_rtcm.algo";
        m_ggaPathName = m_projectPath + m_projectName + "_gga.txt";
        m_rmcPathName = m_projectPath + m_projectName + "_rmc.txt";
        m_inspvaxaPathName = m_projectPath + m_projectName + "_inspvaxa.txt";
        m_pppb2bPathName = m_projectPath + m_projectName + "_pppb2b.b2b";

        m_posPathName = m_projectPath + m_projectName + "_gga.pos";
        m_vinsPathName = m_projectPath + m_projectName + "_vins.txt";

        return true;
    }

    bool AlgoT1Project::close()
    {
        // TODO
        return false;
    }

    bool AlgoT1Project::saveCam0Image(AlgoCameraDataPacket *frame, char *pData, int len) // 9000
    {
        std::string imagePathName = genImagePathName(this, frame, false);
        std::string imageRelativPathName = genImagePathName(this, (AlgoCameraDataPacket *)frame, false);

        // save image
        std::ofstream file(imagePathName, std::ios::out);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        // save description
        std::string line = genImageDesc(this, frame);

        std::ofstream fileDesc(m_cam0DescPathName, std::ios::app);
        if (fileDesc.is_open())
        {
            fileDesc.write(line.c_str(), line.length());
        }
        fileDesc.close();

        sync();

        return true;
    }

    bool AlgoT1Project::saveCam1Image(AlgoCameraDataPacket *frame, char *pData, int len) // 9001
    {
        if (frame->imageType == 0)
        {
            std::string imagePathName = genImagePathName(this, (AlgoCameraDataPacket *)frame, false) + ".bmp";
            WriteBMP(pData, 640, 360, imagePathName.c_str());
        }
        else
        {
            // save image
            std::string imagePathName = genImagePathName(this, (AlgoCameraDataPacket *)frame, false);
            std::ofstream file(imagePathName, std::ios::out);
            if (file.is_open())
            {
                file.write(pData, len);
            }
            file.close();
        }

        // save description
        std::string imageRelativPathName = genImagePathName(this, (AlgoCameraDataPacket *)frame, false);
        std::string line = genImageDesc(this, frame);
        std::ofstream fileDesc(m_cam1DescPathName, std::ios::app);
        if (fileDesc.is_open())
        {
            fileDesc.write(line.c_str(), line.length());
        }
        fileDesc.close();

        sync();

        return true;
    }

    bool AlgoT1Project::saveGnss1Data(char *pData, int len) // 9002
    {
        std::ofstream file(m_gnss1PathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveGnss2Data(char *pData, int len) // 9003
    {
        std::ofstream file(m_gnss2PathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveMemsData(char *pData, int len) // 9004
    {
        std::ofstream file(m_memsPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveRTCMData(char *pData, int len) // 9005
    {
        std::ofstream file(m_rtcmPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveINSPVAXAData(char *pData, int len) // 9006
    {
        std::ofstream file(m_inspvaxaPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveGPGGAData(char *pData, int len) // 9007
    {
        std::ofstream file(m_ggaPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::savePPPB2BData(char *pData, int len) // 9008
    {
        std::ofstream file(m_pppb2bPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveGPRMCData(char *pData, int len) // 9009
    {
        std::ofstream file(m_rmcPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

    bool AlgoT1Project::saveVINSPOSData(char *pData, int len) // 9010
    {
        std::ofstream file(m_vinsPathName, std::ios::app);
        if (file.is_open())
        {
            file.write(pData, len);
        }
        file.close();

        return true;
    }

}