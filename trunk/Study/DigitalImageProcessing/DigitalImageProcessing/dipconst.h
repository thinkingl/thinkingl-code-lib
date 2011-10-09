#pragma once

enum EDIPFunction
{
	DIP_Invalid = 0,			// 无效功能.
	DIP_RGB2Gray_HSI,			// RGB转灰度图, HSI方式.
	DIP_RGB2Gray_YUV,			// RGB转灰度图, YUV方式	

	DIP_IntensityLevels,		// 修改亮度等级.
	DIP_IntensityNegitive,		// 反转. s = L-1-r
	DIP_IntensityLog,			// 取指. s = c * Log( 1+r )
	DIP_IntensityPower,			// 幂值, Power-law (Gamma) Transformation. s = c * r^v 
	DIP_IntensityBitsPlaneReconstruct,	// 位图的各位重构.
};

// end of the file

