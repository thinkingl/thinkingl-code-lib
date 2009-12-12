#pragma once

class INoCopy
{
public:
	INoCopy()
	{

	}
private:
	INoCopy( const INoCopy& );
	INoCopy& operator=( const INoCopy& );
};
