#pragma once

class iTimer
{
public:
	iTimer() = default;
	virtual ~iTimer() = default;

	virtual void Setup() = 0;
	virtual void Update(float FPS) = 0;
	virtual float GetEllapsedTime() = 0;
	virtual float GetWorldTime() = 0;
	virtual float GetTickFPSEllapsedTime() = 0;
	virtual unsigned long GetFrameRate() = 0;
	virtual bool GetTickFPS() = 0;
};

