#pragma once
class EnvironmentMapping
{
public:
	EnvironmentMapping() = default;
	~EnvironmentMapping() = default;

	EnvironmentMapping(const EnvironmentMapping &) = delete;
	EnvironmentMapping(EnvironmentMapping &&) = delete;
	EnvironmentMapping & operator= (const EnvironmentMapping &) = delete;
	EnvironmentMapping & operator= (EnvironmentMapping &&) = delete;
};

