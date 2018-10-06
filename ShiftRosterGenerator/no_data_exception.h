/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		06.10.2018
Last update:	06.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#ifndef NODATAEXCEPTION_H
#define NODATAEXCEPTION_H

#include <exception>
#include <string>

class NoDataException : public std::exception
{
public:
	NoDataException( const std::string &errorMessage )
		: errorMessage{ errorMessage }
	{
	}

	virtual const char* what() const
	{
		return this->errorMessage.c_str();
	}

private:
	const std::string errorMessage;
};

#endif // NODATAEXCEPTION_H