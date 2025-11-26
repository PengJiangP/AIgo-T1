std_msgs/Header header

# 状态信息
uint8 state
uint8 ins_state
uint8 pos_state
uint8 rtk_state

# 地理坐标系 (WGS84)
float64 latitude   # 经度 [deg]
float64 longitude  # 纬度 [deg]
float64 altitude   # 海拔高度 [m]

# 局部坐标系 (例如：ENU)
float64 x          # X轴坐标 [m]
float64 y          # Y轴坐标 [m]
float64 z          # Z轴坐标 [m]

# 姿态欧拉角 (弧度单位)
float64 roll       # 横滚角 [rad]
float64 pitch      # 俯仰角 [rad]
float64 yaw        # 偏航角 [rad]

# 方差配置 (9个独立方差)
float64[9] covariance  # 协方差矩阵，行优先排列
                        # 索引对应关系：
                        # [0]  latitude 方差
                        # [1]  longitude 方差
                        # [2]  altitude 方差
                        # [3]  x 方差
                        # [4]  y 方差
                        # [5]  z 方差
                        # [6]  roll 方差
                        # [7]  pitch 方差
                        # [8]  yaw 方差
