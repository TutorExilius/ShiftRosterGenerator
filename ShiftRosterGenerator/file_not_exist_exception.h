/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		04.10.2018
Last update:	13.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#ifndef FILE_NOT_EXIST_EXCEPTION_H
#define FILE_NOT_EXIST_EXCEPTION_H

#include <exception>
#include <string>

class FileNotExistException : public std::exception
{
public:
	FileNotExistException( const std::string &errorMessage )
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

#endif // FILE_NOT_EXIST_EXCEPTION_H