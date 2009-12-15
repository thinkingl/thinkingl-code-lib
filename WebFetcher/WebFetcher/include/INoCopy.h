#pragma once

class INoCopy
{
public:
	INoCopy()
	{

	}
    virtual ~INoCopy()
    {

    }
private:
	INoCopy( const INoCopy& );
	INoCopy& operator=( const INoCopy& );
};
