/**
 * @file: AlgoCoordTrans.h
 * @brief: This header file contains the definition of AlgoCoordTrans, a class that provides coordinate transformation functions.
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @note This file is owned by Algo1010 Technology Co., Ltd. All rights reserved.
 */

#ifndef _ALGO_COORTRANS_H_
#define _ALGO_COORTRANS_H_

namespace Algo1010
{

#define M_PI 3.14159265358979323846
#define RE_WGS84 6378137.0             /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84 (1.0 / 298.257223563) /* earth flattening (WGS84) */
#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)

    // projection coordinate (utm etc.)
    typedef struct xyh
    {
        double x; /**< The x coordinate */
        double y; /**< The y coordinate */
        double h; /**< The height */
    } xyh, *pxyh;

    // earth coordinate (ecef)
    typedef struct XYZ
    {
        double X; /**< The X coordinate */
        double Y; /**< The Y coordinate */
        double Z; /**< The Z coordinate */
    } XYZ, *pXYZ;

    // geodesy coordinate (wgs84 unit:rad rad m)
    typedef struct BLH
    {
        double B; /**< Latitude in radians */
        double L; /**< Longitude in radians */
        double H; /**< Height in meters */
    } BLH, *pBLH;

    // station coordinate
    typedef struct ENU
    {
        double E; /**< Easting */
        double N; /**< Northing */
        double U; /**< Up */
    } ENU, *pENU;

    // station coordinate (polar for satellite skymap)
    typedef struct RAH
    {
        double R; /**< Radius */
        double A; /**< Azimuth */
        double H; /**< Elevation */
    } RAH, *pRHA;

    /**
     * @class AlgoCoordTrans
     * @brief A class that provides coordinate transformation functions.
     */
    class AlgoCoordTrans
    {
    private:
        BLH m_refBLH; /**< The reference geodesy coordinate (BLH) */
        XYZ m_refXYZ; /**< The reference earth coordinate (XYZ) */

    public:
        /**
         * @brief Set the reference geodesy coordinate (BLH).
         * @param B Latitude in radians.
         * @param L Longitude in radians.
         * @param H Height in meters.
         */
        void setRefBLH(double B, double L, double H);

        /**
         * @brief Set the reference earth coordinate (XYZ).
         * @param X The X coordinate.
         * @param Y The Y coordinate.
         * @param Z The Z coordinate.
         */
        void setRefXYZ(double X, double Y, double Z);

        /**
         * @brief Convert geodesy coordinate (BLH) to earth coordinate (XYZ).
         * @param B Latitude in radians.
         * @param L Longitude in radians.
         * @param H Height in meters.
         * @return The converted earth coordinate (XYZ).
         */
        XYZ BLH2XYZ(double B, double L, double H);

        /**
         * @brief Convert earth coordinate (XYZ) to geodesy coordinate (BLH).
         * @param X The X coordinate.
         * @param Y The Y coordinate.
         * @param Z The Z coordinate.
         * @return The converted geodesy coordinate (BLH).
         */
        BLH XYZ2BLH(double X, double Y, double Z);

        /**
         * @brief Convert earth coordinate (XYZ) to station coordinate (ENU).
         * @param Xs The X coordinate of the station.
         * @param Ys The Y coordinate of the station.
         * @param Zs The Z coordinate of the station.
         * @return The converted station coordinate (ENU).
         */
        ENU XYZ2ENU(double Xs, double Ys, double Zs);

        /**
         * @brief Convert geodesy coordinate (BLH) to station coordinate (ENU).
         * @param Bs Latitude in radians of the station.
         * @param Ls Longitude in radians of the station.
         * @param Hs Height in meters of the station.
         * @return The converted station coordinate (ENU).
         */
        ENU BLH2ENU(double Bs, double Ls, double Hs);

        /**
         * @brief Convert earth coordinate (XYZ) to station coordinate (RAH) for satellite skymap.
         * @param Xs The X coordinate of the station.
         * @param Ys The Y coordinate of the station.
         * @param Zs The Z coordinate of the station.
         * @return The converted station coordinate (RAH).
         */
        RAH XYZ2RAH(double Xs, double Ys, double Zs);

        /**
         * @brief Convert geodesy coordinate (BLH) to station coordinate (RAH) for satellite skymap.
         * @param Bs Latitude in radians of the station.
         * @param Ls Longitude in radians of the station.
         * @param Hs Height in meters of the station.
         * @return The converted station coordinate (RAH).
         */
        RAH BLH2RAH(double Bs, double Ls, double Hs);

        static void test_AlgoCoordTrans()
        {
            AlgoCoordTrans m_coordTrans;
            m_coordTrans.setRefBLH(31.1669249 * DEG2RAD, 121.2888731 * DEG2RAD, 0);
            ENU e0 = m_coordTrans.BLH2ENU(31.1669249* DEG2RAD, 121.2888731* DEG2RAD, 0);
            ENU e1 = m_coordTrans.BLH2ENU(31.166* DEG2RAD, 121.288* DEG2RAD, 0);
        }
    };

} //! Algo1010

#endif //!_ALGO_COORTRANS_H_