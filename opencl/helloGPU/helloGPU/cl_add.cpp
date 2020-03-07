// TestOpenCL.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
/*	OpenCL_01.cpp
*	(c) by keyring <keyrings@163.com>
*	2013.10.26
*/

#include <iostream>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
#endif


#define KERNEL(...)#__VA_ARGS__

#pragma warning( disable : 4996 )

const char *kernelSourceCode = KERNEL(
	__kernel void hellocl(__global uint *buffer)
{
	size_t gidx = get_global_id(0);
	size_t gidy = get_global_id(1);
	size_t lidx = get_local_id(0);
	buffer[gidx + 4 * gidy] = (1 << gidx) | (0x10 << gidy);

}
);

int main(int argc, char const *argv[])
{
	//printf("hello OpenCL\n");
	cl_int status = 0;
	size_t deviceListSize;

	// 得到并选择可用平台
	cl_uint numPlatforms;
	cl_platform_id platform = NULL;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);

	if (status != CL_SUCCESS) {
		printf("ERROR: Getting Platforms.(clGetPlatformIDs)\n");
		return EXIT_FAILURE;
	}

	if (numPlatforms > 0) {
		cl_platform_id *platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		if (status != CL_SUCCESS) {
			printf("Error: Getting Platform Ids.(clGetPlatformIDs)\n");
			return -1;
		}

		for (unsigned int i = 0; i < numPlatforms; ++i) {
			char pbuff[100];
			status = clGetPlatformInfo(
				platforms[i],
				CL_PLATFORM_VENDOR,
				sizeof(pbuff),
				pbuff,
				NULL);
			platform = platforms[i];
			if (!strcmp(pbuff, "Advanced Micro Devices, Inc.")) {
				break;
			}
		}

		delete platforms;
	}

	// 如果我们能找到相应平台，就使用它，否则返回NULL
	cl_context_properties cps[3] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform,
		0
	};

	cl_context_properties *cprops = (NULL == platform) ? NULL : cps;


	// 生成 context
	cl_context context = clCreateContextFromType(
		cprops,
		CL_DEVICE_TYPE_GPU,
		NULL,
		NULL,
		&status);
	if (status != CL_SUCCESS) {
		printf("Error: Creating Context.(clCreateContexFromType)\n");
		return EXIT_FAILURE;
	}

	// 寻找OpenCL设备

	// 首先得到设备列表的长度
	status = clGetContextInfo(context,
		CL_CONTEXT_DEVICES,
		0,
		NULL,
		&deviceListSize);
	if (status != CL_SUCCESS) {
		printf("Error: Getting Context Info device list size, clGetContextInfo)\n");
		return EXIT_FAILURE;
	}
	cl_device_id *devices = (cl_device_id *)malloc(deviceListSize);
	if (devices == 0) {
		printf("Error: No devices found.\n");
		return EXIT_FAILURE;
	}

	// 现在得到设备列表
	status = clGetContextInfo(context,
		CL_CONTEXT_DEVICES,
		deviceListSize,
		devices,
		NULL);
	if (status != CL_SUCCESS) {
		printf("Error: Getting Context Info (device list, clGetContextInfo)\n");
		return EXIT_FAILURE;
	}


	// 装载内核程序，编译CL program ,生成CL内核实例

	size_t sourceSize[] = { strlen(kernelSourceCode) };
	cl_program program = clCreateProgramWithSource(context,
		1,
		&kernelSourceCode,
		sourceSize,
		&status);
	if (status != CL_SUCCESS) {
		printf("Error: Loading Binary into cl_program (clCreateProgramWithBinary)\n");
		return EXIT_FAILURE;
	}

	// 为指定的设备编译CL program.
	status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		printf("Error: Building Program (clBuildingProgram)\n");
		return EXIT_FAILURE;
	}

	// 得到指定名字的内核实例的句柄
	cl_kernel kernel = clCreateKernel(program, "hellocl", &status);
	if (status != CL_SUCCESS) {
		printf("Error: Creating Kernel from program.(clCreateKernel)\n");
		return EXIT_FAILURE;
	}

	// 创建 OpenCL buffer 对象
	unsigned int *outbuffer = new unsigned int[4 * 4];
	memset(outbuffer, 0, 4 * 4 * 4);
	cl_mem outputBuffer = clCreateBuffer(
		context,
		CL_MEM_ALLOC_HOST_PTR,
		4 * 4 * 4,
		NULL,
		&status);

	if (status != CL_SUCCESS) {
		printf("Error: Create Buffer, outputBuffer. (clCreateBuffer)\n");
		return EXIT_FAILURE;
	}


	//  为内核程序设置参数
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&outputBuffer);
	if (status != CL_SUCCESS) {
		printf("Error: Setting kernel argument. (clSetKernelArg)\n");
		return EXIT_FAILURE;
	}

	// 创建一个OpenCL command queue
	cl_command_queue commandQueue = clCreateCommandQueue(context,
		devices[0],
		0,
		&status);
	if (status != CL_SUCCESS) {
		printf("Error: Create Command Queue. (clCreateCommandQueue)\n");
		return EXIT_FAILURE;
	}


	// 将一个kernel 放入 command queue
	size_t globalThreads[] = { 4, 4 };
	size_t localThreads[] = { 2, 2 };
	status = clEnqueueNDRangeKernel(commandQueue, kernel,
		2, NULL, globalThreads,
		localThreads, 0,
		NULL, NULL);
	if (status != CL_SUCCESS) {
		printf("Error: Enqueueing kernel\n");
		return EXIT_FAILURE;
	}

	// 确认 command queue 中所有命令都执行完毕
	status = clFinish(commandQueue);
	if (status != CL_SUCCESS) {
		printf("Error: Finish command queue\n");
		return EXIT_FAILURE;
	}

	// 将内存对象中的结果读回Host
	status = clEnqueueReadBuffer(commandQueue,
		outputBuffer, CL_TRUE, 0,
		4 * 4 * 4, outbuffer, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
		printf("Error: Read buffer queue\n");
		return EXIT_FAILURE;
	}

	// Host端打印结果
	printf("out:\n");
	for (int i = 0; i < 16; ++i) {
		printf("%x ", outbuffer[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}

	// 资源回收
	status = clReleaseKernel(kernel);
	status = clReleaseProgram(program);
	status = clReleaseMemObject(outputBuffer);
	status = clReleaseCommandQueue(commandQueue);
	status = clReleaseContext(context);

	free(devices);
	delete outbuffer;

	system("pause");

	return 0;
}
