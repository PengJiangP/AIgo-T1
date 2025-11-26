/**
 * @file AlgoData.h
 * @author jack
 * @date October 20, 2023
 * @version 1.0
 * @brief This header file contains the definition of callback Data.
 *
 * This file defines the structures and classes related to data handling and device management.
 */

#ifndef _ALGO_DATA_H_
#define _ALGO_DATA_H_

#include <memory>
#include <string>

namespace Algo1010
{

  /**
   * @struct algo_gtime_t
   * @brief Structure for representing gps time.
   *
   * This structure represents gps time, which consists of standard time expressed by time_t
   * and a fraction of a second.
   */
  typedef struct
  {
    time_t time; /**< time (s) expressed by standard time_t */
    double sec;  /**< fraction of second under 1 s */
  } algo_gtime_t;

  /**
   * @class AlgoDataPacket
   * @brief Base class for data packets.
   *
   * This class serves as the base class for all data packets. It provides a shared pointer type for convenience.
   */
  class AlgoDataPacket
  {
  public:
    AlgoDataPacket() {}
    ~AlgoDataPacket() {}

  public:
    // Note: Must be nothing because the decode stream data uses memory!!!
  };
  typedef std::shared_ptr<AlgoDataPacket> AlgoDataPacketPtr;

  /**
   * @class AlgoDevice
   * @brief Base class for devices.
   *
   * This class provides an interface for device operations such as loading, starting, and stopping.
   */
  class AlgoDevice
  {
  public:
    AlgoDevice() {}
    ~AlgoDevice() {}

    /**
     * @brief Load data from a specified path.
     * @param path The path to load data from.
     * @return True if the loading is successful, false otherwise.
     */
    virtual bool load(std::string path) = 0;

    /**
     * @brief Start the device.
     * @return True if the device starts successfully, false otherwise.
     */
    virtual bool start() = 0;

    /**
     * @brief Stop the device.
     * @return True if the device stops successfully, false otherwise.
     */
    virtual bool stop() = 0;

  protected:
    void *m_private; /**< Private data for device-specific implementation. */
  };

  /**
   * @class AlgoProject
   * @brief Base class for projects.
   *
   * This class provides functionalities for managing projects, including creating, opening, and closing projects.
   */
  class AlgoProject
  {
  public:
    std::string m_rootPath = "";    /**< Root path of the project. */
    std::string m_yyyymmdd = "";    /**< Date of the project in YYYYMMDD format. */
    std::string m_hhmmss = "";      /**< Time of the project in HHMMSS format. */
    std::string m_projectPath = ""; /**< Path of the project. */
    std::string m_projectName = ""; /**< Name of the project. */

  public:
    AlgoProject()
    {
#ifdef _OS_RV1126_
      m_rootPath = "/sdcard/AlgoUserData"; // config key = "rootdir"
#elif _OS_UBUNTU_
      m_rootPath = "/home/AlgoUserData"; // config key = "rootdir"
#else //_OS_WINDOWS_
      m_rootPath = "C:/AlgoUserData"; // config key = "rootdir"
#endif
    }
    ~AlgoProject() {}

    /**
     * @brief Create directories and files for saving device's data.
     * @param rootPath The root path of the project.
     * @return True if the creation is successful, false otherwise.
     */
    virtual bool create(std::string rootPath = "") = 0;

    /**
     * @brief Open an existing project.
     * @param projectPath The path of the project.
     * @param projectName The name of the project.
     * @return True if the opening is successful, false otherwise.
     */
    virtual bool open(std::string projectPath, std::string projectName) = 0;

    /**
     * @brief Close the open project.
     * @return True if the closing is successful, false otherwise.
     */
    virtual bool close() = 0;
  };

}

#endif // !_ALGO_DATA_H_