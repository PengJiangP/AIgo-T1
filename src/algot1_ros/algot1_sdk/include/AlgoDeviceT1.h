/**
 * @file AlgoDeviceT1.h
 * @brief This header file contains the definition of AlgoDeviceT1.
 *        It is used to handle data from different sources in Algo1010 devices.
 * @version 1.0
 * @date Oct.20,2023
 * @author jack <283422622@qq.com>
 * @copyright Copyright (c) 2023
 * @namespace Algo1010
 */

#ifndef _ALGO_DEVICE_T1_H_
#define _ALGO_DEVICE_T1_H_

#include "AlgoData.h"

namespace Algo1010
{

    /**
     * @brief The AlgoImuDataPacket class represents a data packet containing IMU data.
     */
    class AlgoImuDataPacket : public AlgoDataPacket
    {
    public:
        AlgoImuDataPacket() {}
        ~AlgoImuDataPacket() {}

    public:
        int index; /**< Index of the IMU data packet. */

        // time
        bool hasPPS;      /**< Flag indicating whether the data packet has the Pulse Per Second (PPS) signal. */
        int cur_week;      /**< Current GPS week. */
        double cur_sec;    /**< Current GPS second. */
        double curtick_ms; /**< Current device clock time in milliseconds. */

        // imu data
        double accel_x; /**< Acceleration value along the X-axis. */
        double accel_y; /**< Acceleration value along the Y-axis. */
        double accel_z; /**< Acceleration value along the Z-axis. */
        double gyro_x;  /**< Gyroscope value along the X-axis. */
        double gyro_y;  /**< Gyroscope value along the Y-axis. */
        double gyro_z;  /**< Gyroscope value along the Z-axis. */
    };
    typedef std::shared_ptr<AlgoImuDataPacket> AlgoImuDataPacketPtr;

    /**
     * @brief The AlgoCameraDataPacket class represents a data packet containing camera data.
     */
    class AlgoCameraDataPacket : public AlgoDataPacket
    {
    public:
        AlgoCameraDataPacket() {}
        ~AlgoCameraDataPacket() {}

    public:
        // pos
        char posType;  /**< Position type. */
        double posLat; /**< Latitude position. */
        double posLon; /**< Longitude position. */
        double posHgt; /**< Height position. */
        double yaw;    /**< Yaw angle. */
        double roll;   /**< Roll angle. */
        double pitch;  /**< Pitch angle. */

        // time
        bool hasPPS;      /**< Flag indicating whether the data packet has the Pulse Per Second (PPS) signal. */
        int pps_week;      /**< GPS week with PPS sync. */
        double pps_sec;    /**< GPS second with PPS sync. */
        double ppstick_ms; /**< PPS tick in milliseconds. */
        int cur_week;      /**< Current GPS week. */
        double cur_sec;    /**< Current GPS second. */
        double curtick_ms; /**< Current device clock time in milliseconds. */
        double dpps_sec;   /**< Time difference in seconds between the PPS ticks. */
        double dlapse_ms;  /**< Time lapse in milliseconds from the last frame. */
        algo_gtime_t time; /**< GPS time. */

        // image
        int cameraId;            /**< Camera ID. */
        int cameraMode;          /**< Camera mode. */
        int imageIndex;          /**< Image index. */
        unsigned char imageType; /**< Image type. */
        int imageWidth;          /**< Image width. */
        int imageHeight;         /**< Image height. */
        int pbufSize;            /**< Size of the image buffer. */
        char *pbuf = nullptr;    /**< Pointer to the ImageBuffer. */
    };
    typedef std::shared_ptr<AlgoCameraDataPacket> AlgoCameraDataPacketPtr;

    /**
     * @brief The AlgoINSPVAXADataPacket class represents a data packet containing INSPVAXA data.
     */
    class AlgoINSPVAXADataPacket : public AlgoDataPacket
    {
    public:
        AlgoINSPVAXADataPacket() {}
        ~AlgoINSPVAXADataPacket() {}

    public:
        int index; /**< Index of the INSPVAXA data packet. */

        // time
        bool hasPPS;      /**< Flag indicating whether the data packet has the Pulse Per Second (PPS) signal. */
        int cur_week;      /**< Current GPS week with PPS sync. */
        double cur_sec;    /**< Current GPS second with PPS sync. */
        double curtick_ms; /**< Current device clock time in milliseconds since system start. */
        
        // pos
        char insStatus = 0; /**< INS status as defined in ByNav documentation. */
        char posType = 0;   /**< Position type as defined in ByNav documentation. */
        double posLat;      /**< Latitude position. */
        double posLon;      /**< Longitude position. */
        double posHgt;      /**< Height position. */
        double velE;        /**< East velocity component. */
        double velN;        /**< North velocity component. */
        double velU;        /**< Upward velocity component. */
        double yaw;         /**< Yaw angle. */
        double roll;        /**< Roll angle. */
        double pitch;       /**< Pitch angle. */
        double sigLat;      /**< Standard deviation of the latitude position. */
        double sigLon;      /**< Standard deviation of the longitude position. */
        double sigHgt;      /**< Standard deviation of the height position. */
        double sigVelE;     /**< Standard deviation of the East velocity component. */
        double sigVelN;     /**< Standard deviation of the North velocity component. */
        double sigVelU;     /**< Standard deviation of the upward velocity component. */
        double sigYaw;      /**< Standard deviation of the yaw angle. */
        double sigRoll;     /**< Standard deviation of the roll angle. */
        double sigPitch;    /**< Standard deviation of the pitch angle. */
    };
    typedef std::shared_ptr<AlgoINSPVAXADataPacket> AlgoINSPVAXADataPacketPtr;

    /**
     * @brief The AlgoGPGGADataPacket class represents a data packet containing GPGGA data.
     */
    class AlgoGPGGADataPacket : public AlgoDataPacket
    {
    public:
        AlgoGPGGADataPacket() {}
        ~AlgoGPGGADataPacket() {}

    public:
        int index; /**< Index of the GPGGA data packet. */

        // time
        bool hasPPS;       /**< Flag indicating whether the data packet has the Pulse Per Second (PPS) signal. */
        int cur_week;       /**< Current GPS week. */
        double cur_sec;     /**< Current GPS second. */
        double curtick_ms;  /**< Current device clock time in milliseconds. */
        std::string ddmmyy; /**< Date in the format DDMMYY. */
        std::string hhmmss; /**< Time in the format HHMMSS. */

        // pos
        char posType;  /**< Position type. */
        double posLat; /**< Latitude position. */
        double posLon; /**< Longitude position. */
        double posHgt; /**< Height position. */
        double diffAge;
        int satNum;
        // --- 新增下面这一行 ---
        double hdop;   /**< Horizontal Dilution of Precision */
        // --------------------
    };
    typedef std::shared_ptr<AlgoGPGGADataPacket> AlgoGPGGADataPacketPtr;

    // ALGO_NMEA_GPRMC
    /**
     * @brief Class for storing GPRMC data packet.
     */
    class AlgoGPRMCDataPacket : public AlgoDataPacket
    {
    public:
        /**
         * @brief Default constructor
         */
        AlgoGPRMCDataPacket() {}

        /**
         * @brief Default destructor
         */
        ~AlgoGPRMCDataPacket() {}

    public:
        int index; /**< Index of the data packet */

        // time
        bool hasPPS;       /**< Flag indicating whether the data contains pps */
        int cur_week;       /**< GPS week (invalid for GPGGA) */
        double cur_sec;     /**< GPS second */
        double curtick_ms;  /**< Clocktime of device, possibly from system start */
        std::string ddmmyy; /**< Date in DDMMYY format */
        std::string hhmmss; /**< Time in HHMMSS.SSSS format */

        // pos
        char posType;  /**< Position type */
        double posLat; /**< Latitude of position */
        double posLon; /**< Longitude of position */
        double posHgt; /**< Height of position */
    };
    typedef std::shared_ptr<AlgoGPRMCDataPacket> AlgoGPRMCDataPacketPtr;

    // ALGO_VINSPOS
    /**
     * @brief Class for storing VINSPOS data packet.
     */
    class AlgoVINSPOSDataPacket : public AlgoDataPacket
    {
    public:
        /**
         * @brief Default constructor
         */
        AlgoVINSPOSDataPacket() {}

        /**
         * @brief Default destructor
         */
        ~AlgoVINSPOSDataPacket() {}

    public:
        int index; /**< Index of the data packet */

        // time
        bool hasPPS;       /**< Flag indicating whether the data contains pps */
        int cur_week;       /**< GPS week (invalid for GPGGA) */
        double cur_sec;     /**< GPS second */
        double curtick_ms;  /**< Clocktime of device, possibly from system start */
        std::string ddmmyy; /**< Date in DDMMYY format */
        std::string hhmmss; /**< Time in HHMMSS.SSSS format */

        // pos
        char posType;   /**< Position type */
        double Q[4];    /**< Quaternion representing orientation */
        double P[3];    /**< Position vector */
        double V[3];    /**< Velocity vector */
        double stdQ[4]; /**< Standard deviation of quaternion */
        double stdP[3]; /**< Standard deviation of position vector */
        double stdV[3]; /**< Standard deviation of velocity vector */
        double td;      /**< Time difference */
    };
    typedef std::shared_ptr<AlgoVINSPOSDataPacket> AlgoVINSPOSDataPacketPtr;

    /**
     * @brief Data type enumeration for AlgoDeviceT1.
     */
    enum AlgoT1DataType
    {
        ALGO_DAT_NONE,     ///< No data type
        ALGO_DAT_CAM0,     ///< Camera 0 data
        ALGO_DAT_CAM1,     ///< Camera 1 data
        ALGO_DAT_GNSS1,    ///< GNSS 1 data
        ALGO_DAT_GNSS2,    ///< GNSS 2 data
        ALGO_DAT_MEMS,     ///< MEMS data
        ALGO_DAT_RTCM,     ///< RTCM data
        ALGO_DAT_INSPVAXA, ///< INSPVAXA data
        ALGO_DAT_GPGGA,    ///< GPGGA data
        ALGO_DAT_PPPB2B,   ///< PPPB2B data
        ALGO_DAT_GPRMC,    ///< GPRMC data
        ALGO_DAT_WHEEL,    ///< Wheel data
        ALGO_DAT_VINS      ///< VINS data
    };

    /**
     * @class AlgoDeviceAlgoT1
     * @brief An abstract base class for handling data from different sources in Algo1010 devices.
     */
    class AlgoDeviceAlgoT1 : public AlgoDevice
    {
    public:
        /**
         * @brief Default constructor of AlgoDeviceAlgoT1.
         */
        AlgoDeviceAlgoT1();

        /**
         * @brief Destructor of AlgoDeviceAlgoT1.
         */
        ~AlgoDeviceAlgoT1();

        /**
         * @brief Callback function for receiving data from the device.
         * @param handle The handle of the device.
         * @param dataType The type of data received.
         * @param packet The data packet.
         * @param data The data buffer.
         * @param len The length of the data buffer.
         */
        void dataCallback(int handle, AlgoT1DataType dataType, AlgoDataPacket *packet, char *data, int len);

        // Define callback functions for specific types of data
        virtual void Cam0DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len) = 0;
        virtual void Cam1DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len) = 0;
        virtual void Gnss1DataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void Gnss2DataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void MemsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void RtcmDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void GpggaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void Pppb2bDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void GprmcDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void WheelDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;
        virtual void VinsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len) = 0;

    public:
        /**
         * @brief Load the configuration file of the device.
         * @param path The path to the configuration file.
         * @return True if successful, false otherwise.
         */
        bool load(std::string path);

        /**
         * @brief Start the device.
         * @return True if successful, false otherwise.
         */
        bool start();

        /**
         * @brief Stop the device.
         * @return True if successful, false otherwise.
         */
        bool stop();
    };

}

#endif //!_ALGO_DEVICE_T1_H_
