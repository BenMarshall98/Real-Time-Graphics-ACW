#pragma once
class IExplode
{
public:
	IExplode() = default;
	virtual ~IExplode() = default;

	virtual void explode() = 0;
};

