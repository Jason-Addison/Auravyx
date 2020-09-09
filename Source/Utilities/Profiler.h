#pragma once
class Profiler
{
public:
	Profiler();
	~Profiler();

	static bool showAdvancedDebugInfo;
	static bool showChunkMetrics;

	static void init();

	static double getCurrentProcessCPU();

	static int getCPUCoreCount();
};

