#pragma once
class IExplode
{
public:
	IExplode() = default;
	virtual ~IExplode() = default;

	IExplode(const IExplode &) = delete;
	IExplode(IExplode &&) = delete;
	IExplode & operator= (const IExplode &) = delete;
	IExplode & operator= (IExplode &&) = delete;

	virtual void explode() = 0;
};

