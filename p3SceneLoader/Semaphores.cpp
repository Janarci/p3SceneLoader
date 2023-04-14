#include "Semaphores.h"

Semaphores::Semaphores(int available)
{
	this->semaphore = new Semaphore(available);
}

Semaphores::~Semaphores()
{
	delete this->semaphore;
}

void Semaphores::acquire() const
{
	this->semaphore->acquire();
}

void Semaphores::acquire(int permits) const
{
	for (int i = 0; i < permits; i++)
	{
		this->semaphore->acquire();
	}
}

void Semaphores::release(int permits) const
{
	this->semaphore->release(permits);
}

void Semaphores::release() const
{
	this->semaphore->release();
}