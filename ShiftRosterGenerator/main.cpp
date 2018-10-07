/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		04.10.2018
Last update:	06.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#include <algorithm>
#include <iostream>
#include <fstream>

#include "generator.h"
#include "file_not_exist_exception.h"
#include "nullptr_exception.h"
#include "no_data_exception.h"

#include "app_info.h"

int main( int argc, char *argv[] )
{
	std::cerr << APP_NAME << std::endl;
	std::cerr << "Version: " << STRING_VERSION << std::endl;
	std::cerr << AUTHOR << "(" AUTHOR_EMAIL ")\n"<< std::endl;

	try
	{
		if( argc >= 2 && argc <= 3 )
		{
			std::cin.get();
			Generator generator{ argv[1] };

			if( argc == 3 )
			{
				std::string outFileName = argv[2];

				std::ofstream outFile{ outFileName };

				if( !outFile )
				{
					throw FileNotExistException( "could not create file: '" + outFileName + "'" );
				}

				outFile << generator.getResultAsCSV();
				outFile.close();
			}
			else
			{
				std::cout << generator.getResultAsCSV();
			}
		}
		else if( argc < 2 )
		{
			std::cout << "Not enough arguments" << std::endl;
		}
		else
		{
			std::cout << "Too many arguments" << std::endl;
		}
	}
	catch( const FileNotExistException &e )
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch( const NullptrException &e )
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch( const NoDataException &e )
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}