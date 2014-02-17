#pragma once

class IRenderThread
{
public:
	IRenderThread() {};
	virtual ~IRenderThread() {};

	virtual void start();
	virtual void stop();
private:
};