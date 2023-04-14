#pragma once
#include <semaphore>
#include <mutex>

class Semaphores
{
public:
	Semaphores(int available);
	~Semaphores();

	void acquire() const;
	void acquire(int permits) const;
	void release() const;
	void release(int permits) const;

private:
	const static int GLOBAL_MAX_PERMIT = 50;
	typedef std::counting_semaphore<GLOBAL_MAX_PERMIT> Semaphore;
	Semaphore* semaphore;
};