#include "AlgoCoordTrans.h"

#include <stdio.h>
#include <math.h>

namespace Algo1010
{

    void AlgoCoordTrans::setRefBLH(double B, double L, double H)
    {
        m_refBLH = {B, L, H};
        m_refXYZ = BLH2XYZ(B, L, H);
    }

    void AlgoCoordTrans::setRefXYZ(double X, double Y, double Z)
    {
        m_refXYZ = {X, Y, Z};
        m_refBLH = XYZ2BLH(X, Y, Z);
    }

    XYZ AlgoCoordTrans::BLH2XYZ(double B, double L, double H)
    {
        double sinp = sin(B), cosp = cos(B), sinl = sin(L), cosl = cos(L);
        double e2 = FE_WGS84 * (2.0 - FE_WGS84), v = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);

        XYZ res = {0};
        res.X = (v + H) * cosp * cosl;
        res.Y = (v + H) * cosp * sinl;
        res.Z = (v * (1.0 - e2) + H) * sinp;

        return res;
    }

    BLH AlgoCoordTrans::XYZ2BLH(double X, double Y, double Z)
    {
        double a, f, e2, B = 0.0, N = 0.0, H = 0.0, R0, R1, deltaH, deltaB;
        ;
        a = RE_WGS84, f = FE_WGS84, e2 = f * (2 - f);
        BLH res = {0};
        R0 = sqrt(pow(X, 2) + pow(Y, 2));
        R1 = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
        // 经度直接求解
        res.L = atan2(Y, X);
        // 迭代求大地维度和大地高
        N = a;
        H = R1 - a;
        B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
        do
        {
            deltaH = N; // 判断收敛所用
            deltaB = B;
            N = a / sqrt(1 - e2 * pow(sin(B), 2));
            H = R0 / cos(B) - N;
            B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
        } while (fabs(deltaH - H) > 1.0e-3 && fabs(deltaB - B) > 1.0e-9);
        res.B = B;
        res.H = H;
        return res;
    }

    ENU AlgoCoordTrans::XYZ2ENU(double Xs, double Ys, double Zs)
    {
        double Xr = m_refXYZ.X;
        double Yr = m_refXYZ.Y;
        double Zr = m_refXYZ.Z;
        ENU enu = {0};
        BLH ref = XYZ2BLH(Xr, Yr, Zr);
        double sinL = sin(ref.L);
        double cosL = cos(ref.L);
        double sinB = sin(ref.B);
        double cosB = cos(ref.B);
        double dx = Xs - Xr;
        double dy = Ys - Yr;
        double dz = Zs - Zr;
        enu.E = -sinL * dx + cosL * dy;
        enu.N = -sinB * cosL * dx - sinB * sinL * dy + cosB * dz;
        enu.U = cosB * cosL * dx + cosB * sinL * dy + sinB * dz;
        return enu;
    }

    ENU AlgoCoordTrans::BLH2ENU(double Bs, double Ls, double Hs)
    {
        XYZ sta = BLH2XYZ(Bs, Ls, Hs);
        ENU enu = XYZ2ENU(sta.X, sta.Y, sta.Z);
        return enu;
    }

    RAH AlgoCoordTrans::XYZ2RAH(double Xs, double Ys, double Zs)
    {
        // double Xr = m_refXYZ.X;
        // double Yr = m_refXYZ.Y;
        // double Zr = m_refXYZ.Z;
        RAH rah = {0};
        ENU enu = XYZ2ENU(Xs, Ys, Zs);
        rah.H = atan2(enu.U, sqrt(enu.E * enu.E + enu.N * enu.N));
        rah.A = atan2(enu.E, enu.N);
        if (rah.A < 0)
            rah.A += 2 * M_PI;
        if (rah.A > 2 * M_PI)
            rah.A -= 2 * M_PI;
        rah.R = sqrt(enu.E * enu.E + enu.N * enu.N + enu.U * enu.U);
        return rah;
    }

    RAH AlgoCoordTrans::BLH2RAH(double Bs, double Ls, double Hs)
    {
        XYZ sta = BLH2XYZ(Bs, Ls, Hs);
        RAH rah = XYZ2RAH(sta.X, sta.Y, sta.Z);
        return rah;
    }

} //! Algo1010