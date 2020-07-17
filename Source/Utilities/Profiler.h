#pragma once
class Profiler
{
public:
	Profiler();
	~Profiler();

	static bool showAdvancedDebugInfo;
	static bool showChunkMetrics;

	void init();

	double getCurrentProcessCPU();
};

