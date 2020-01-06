#pragma once
class IExplode
{	
public:
	IExplode() = default;
	virtual ~IExplode();
	virtual void explode() = 0;
};

