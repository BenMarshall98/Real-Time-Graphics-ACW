#pragma once
class IExplode
{	
public:
	IExplode() {};
	virtual ~IExplode();
	virtual void explode() = 0;
};

