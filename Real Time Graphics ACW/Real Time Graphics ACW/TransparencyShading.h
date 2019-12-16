#pragma once
class TransparencyShading
{
public:
	TransparencyShading() = default;
	~TransparencyShading() = default;

	TransparencyShading(const TransparencyShading &) = delete;
	TransparencyShading(TransparencyShading &&) = delete;
	TransparencyShading & operator= (const TransparencyShading &) = delete;
	TransparencyShading & operator= (TransparencyShading &&) = delete;
};

