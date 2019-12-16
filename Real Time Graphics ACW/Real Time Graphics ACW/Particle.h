#pragma once
class Particle
{
public:
	Particle() = default;
	~Particle() = default;

	Particle(const Particle &) = delete;
	Particle(Particle &&) = delete;
	Particle & operator= (const Particle &) = delete;
	Particle & operator= (Particle &&) = delete;
};

