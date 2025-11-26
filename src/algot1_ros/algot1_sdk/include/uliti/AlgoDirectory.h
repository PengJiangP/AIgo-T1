/**
 * @file AlgoDirectory.h
 * @brief This header file contains the definition of AlgoDirectory class, which provides functions for file and directory operations.
 * @author jack <283422622@qq.com>
 * @date Oct.20, 2023
 * @version 1.0
 *
 * @details The AlgoDirectory class provides a set of static functions for file and directory operations on Linux systems.
 * The functions include checking if a directory exists, creating a directory, checking if a file exists, creating a file,
 * deleting a directory, deleting a file, retrieving the file name from a full path, retrieving the directory path from a full path,
 * and retrieving the file extension from a full path.
 *
 * @note This class is developed by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _ALGO_DIRECTORY_H_
#define _ALGO_DIRECTORY_H_

#include <iostream>
#include <fstream>

namespace Algo1010
{
    /**
     * @class AlgoDirectory
     * @brief The AlgoDirectory class encapsulates file and directory operations on Linux systems.
     */
    class AlgoDirectory
    {
    public:
        /**
         * @brief Check if a directory exists at the specified path.
         *
         * @param path The path to the directory.
         * @return true if the directory exists, false otherwise.
         */
        static bool directoryExists(const std::string &path);

        /**
         * @brief Create a directory at the specified path.
         *
         * @param path The path to the directory.
         * @return true if the directory is created successfully, false otherwise.
         */
        static bool createDirectory(const std::string &path);

        /**
         * @brief Check if a file exists at the specified path.
         *
         * @param path The path to the file.
         * @return true if the file exists, false otherwise.
         */
        static bool fileExists(const std::string &path);

        /**
         * @brief Create a file at the specified path with an optional header.
         *
         * @param path The path to the file.
         * @param header The optional header content for the file.
         * @return true if the file is created successfully, false otherwise.
         */
        static bool createFile(const std::string &path, const std::string header = "");

        /**
         * @brief Delete a directory at the specified path.
         *
         * @param path The path to the directory.
         * @return true if the directory is deleted successfully, false otherwise.
         */
        static bool deleteDirectory(const std::string &path);

        /**
         * @brief Delete a file at the specified path.
         *
         * @param path The path to the file.
         * @return true if the file is deleted successfully, false otherwise.
         */
        static bool deleteFile(const std::string &path);

        /**
         * @brief Retrieve the file name from a full path.
         *
         * @param path The full path to the file, including the file name.
         * @return The file name extracted from the path.
         */
        static std::string getFileName(const std::string &path);

        /**
         * @brief Retrieve the directory path from a full path.
         *
         * @param path The full path to the file or directory.
         * @return The directory path extracted from the path.
         */
        static std::string getDirectory(const std::string &path);

        /**
         * @brief Retrieve the file extension from a full path.
         *
         * @param path The full path to the file, including the file name.
         * @return The file extension extracted from the path.
         */
        static std::string getFileExtension(const std::string &path);
    };

}

#endif //!_ALGO_DIRECTORY_H_