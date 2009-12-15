#pragma once

class IApplication
{
public:
    virtual ~IApplication(){};

	virtual int RunWebFetch() = 0;
};
