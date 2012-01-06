#pragma once

enum EDIPFunction
{
	DIP_Invalid = 0,			// ��Ч����.
	DIP_RGB2Gray_HSI,			// RGBת�Ҷ�ͼ, HSI��ʽ.
	DIP_RGB2Gray_YUV,			// RGBת�Ҷ�ͼ, YUV��ʽ	

	DIP_Spatial_Rotation,		// ��ת.
	DIP_Spatial_Translation,	// ����ϵת��.
	DIP_Spatial_Scaling,			// ����.

	DIP_IntensityLevels,		// �޸����ȵȼ�.
	DIP_IntensityNegitive,		// ��ת. s = L-1-r
	DIP_IntensityLog,			// ȡָ. s = c * Log( 1+r )
	DIP_IntensityPower,			// ��ֵ, Power-law (Gamma) Transformation. s = c * r^v 
	DIP_IntensityBitsPlaneReconstruct,	// λͼ�ĸ�λ�ع�.
	DIP_IntensityBitsPlaneShow,	// λͼ�ĸ�λ�ֲ���ʾ.
	DIP_IntensityHistogramEqualization,	// ֱ��ͼƽ��.
};

// end of the file

