#pragma once
class IExplode
{
protected:
	IExplode() = default;
	virtual ~IExplode() = default;
	
public:
	IExplode(const IExplode &) = delete;
	IExplode(IExplode &&) = delete;
	IExplode & operator= (const IExplode &) = delete;
	IExplode & operator= (IExplode &&) = delete;

	virtual void explode() = 0;
};

