/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		13.10.2018
Last update:	13.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#include "csvparser.h"

#include "file_not_exist_exception.h"

#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

// static ---
std::string CSVParser::extractFileName( const std::string &fullFileName )
{
	std::string tmpFileName = CSVParser::replaceAll( fullFileName, "/", "\\" );

	const size_t pos = tmpFileName.find_last_of( '\\' );

	if( pos != std::string::npos && pos + 1 < tmpFileName.size() )
	{
		return std::move( tmpFileName.substr( pos + 1 ) );
	}
	else if( tmpFileName.size() > 0 )
	{
		return tmpFileName;
	}
	else
	{
		return "";
	}
}

std::string CSVParser::extractFilePath( const std::string &fullFileName )
{
	std::string tmpFileName = CSVParser::replaceAll( fullFileName, "/", "\\" );

	const size_t pos = tmpFileName.find_last_of( '\\' );

	if( pos != std::string::npos )
	{
		return std::move( tmpFileName.substr( 0, pos + 1 ) );
	}
	else
	{
		return "";
	}
}

std::string CSVParser::replaceAll( std::string str, const std::string &from, const std::string &to )
{
	size_t start_pos = 0;

	while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
	{
		str.replace( start_pos, from.size(), to );
		start_pos += to.size();
	}

	return std::move( str );
}

std::vector<std::string> CSVParser::combineMissplittedColumns( const std::vector<std::string> &seperatedColumns )
{
	std::vector<std::string> recombined;
	std::string data;

	bool combineMode = false;

	for( const auto column : seperatedColumns )
	{
		bool isValid = CSVParser::isValidQuoted( column );

		if( isValid && !combineMode )
		{
			if( data.size() > 0 )
			{
				data.pop_back();
				recombined.push_back( data );
				data.clear();
			}

			recombined.push_back( column );
		}
		else
		{
			bool finishCombination = false;

			if( combineMode && !isValid )
			{
				combineMode = false;
				finishCombination = true;
			}
			else if( !combineMode )
			{
				combineMode = true;
			}

			data += column + ';';

			if( finishCombination )
			{
				if( data.size() > 0 )
				{
					data.pop_back();
					recombined.push_back( data );
					data.clear();
				}
			}
		}
	}

	if( data.size() > 0 )
	{
		recombined.push_back( data );
	}

	return std::move( recombined );
}

void CSVParser::maskColumnNewlines( std::vector<std::string> &seperatedColumns )
{
	for( auto &column : seperatedColumns )
	{
		if( column.size() >= 2 )
		{
			auto startDoubleQuote = column.find_first_of( '\"' );
			auto endDoubleQuote = column.find_last_of( '\"' );

			if( startDoubleQuote != std::string::npos && endDoubleQuote != std::string::npos &&
				startDoubleQuote < endDoubleQuote )
			{
				if( CSVParser::count( column, '\"' ) > 2 )
				{
					endDoubleQuote = column.substr( 0, endDoubleQuote - 1 ).find_last_of( '\"' );
				}

				std::string tmp = column.substr( startDoubleQuote, endDoubleQuote - startDoubleQuote );
				tmp = CSVParser::replaceAll( tmp, "\n", "\\n" );

				column = column.replace( startDoubleQuote, endDoubleQuote - startDoubleQuote, tmp );
			}
		}
	}
}

void CSVParser::unMaskColumnNewlines( std::vector<std::string> &rows )
{
	for( auto &row : rows )
	{
		row = CSVParser::replaceAll( row, "\\n", "\n" );
	}
}

bool CSVParser::isValidQuoted( const std::string &str )
{
	size_t cnt = CSVParser::count( str, '\"' );

	if( CSVParser::isEven( static_cast<int>( cnt ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t CSVParser::count( const std::string &str, const char ch )
{
	size_t cnt = 0;

	for( const auto &c : str )
	{
		if( ch == c )
		{
			++cnt;
		}
	}

	return cnt;
}

bool CSVParser::isEven( const int &num )
{
	return ( num % 2 == 0 );
}

std::string CSVParser::generateRandomString( const size_t stringLength )
{
	std::string maskingChars{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890" };

	std::string str( stringLength, '\0' );

	for( size_t i = 0; i < stringLength; ++i )
	{
		const size_t randIndex = std::rand() % maskingChars.size();
		str[i] = maskingChars[randIndex];
	}

	return str;
}
// ---

CSVParser::CSVParser( const char seperator )
	: seperator{ seperator }
{
}

CSVParser::~CSVParser() = default;

void CSVParser::parse( const std::string &fullFileName )
{
	this->setFullFileName( fullFileName );

	std::ifstream inFile{ this->getFullFileName() };

	if( !inFile )
	{
		throw FileNotExistException{ "Not found: '" + this->getFullFileName() + "'" };
	}

	// read CSV File ---
	std::vector<std::string> withSeperatorSeperatedData;
	std::string seperatedColum;

	while( std::getline( inFile, seperatedColum, this->seperator ) )
	{
		withSeperatorSeperatedData.push_back( seperatedColum );
	}

	inFile.close();
	// ---

	withSeperatorSeperatedData = CSVParser::combineMissplittedColumns( withSeperatorSeperatedData );

	CSVParser::maskColumnNewlines( withSeperatorSeperatedData );
	CSVParser::maskColumnSeperators( withSeperatorSeperatedData );

	std::vector<std::string> rows = CSVParser::createRows( withSeperatorSeperatedData );

	CSVParser::unMaskColumnNewlines( rows );

	this->mapCSVData( rows );
}

void CSVParser::parse( const std::string &fullFileName, const char seperator )
{
	this->seperator = seperator;
	this->parse( fullFileName );
}

std::string CSVParser::getFileName() const
{
	return this->fileName;
}

std::string CSVParser::getFilePath() const
{
	return this->filePath;
}

std::string CSVParser::getFullFileName() const
{
	return this->filePath + this->fileName;
}

const CSVParser::Matrix& CSVParser::getCSVMatrix() const
{
	return this->csvDataMatrix;
}

std::string CSVParser::getCSVOutput() const
{
	std::stringstream ss;

	for( const auto &xVec : this->csvDataMatrix )
	{
		size_t cntColumn = 0;
		for( const auto yVec : xVec )
		{
			ss << yVec;

			if( ++cntColumn < xVec.size() )
			{
				ss << this->seperator;
			}
		}
		ss << std::endl;
	}

	return ss.str();
}

char CSVParser::getSeperator() const
{
	return this->seperator;
}

void CSVParser::setFullFileName( const std::string &fullFileName )
{
	this->setFileName( CSVParser::extractFileName( fullFileName ) );
	this->setFilePath( CSVParser::extractFilePath( fullFileName ) );
}

void CSVParser::setFileName( const std::string &fileName )
{
	this->fileName = fileName;
}

void CSVParser::setFilePath( const std::string &filePath )
{
	this->filePath = filePath;
}

std::vector<std::string> CSVParser::createRows( const std::vector<std::string> &seperatedColumns )
{
	std::vector<std::string> rows;
	std::stringstream ss;

	size_t cnt = 0;
	for( const auto &column : seperatedColumns )
	{
		ss << column;

		if( ++cnt < seperatedColumns.size() )
		{
			ss << this->seperator;
		}
	}

	std::string row;
	while( std::getline( ss, row ) )
	{
		rows.push_back( row );
	}

	return std::move( rows );
}

void CSVParser::maskColumnSeperators( std::vector<std::string> &rows )
{
	// find masking string, that can be used as seperator-masker ---
	bool foundUniqueString = false;

	size_t generateStrLenght = 3;
	do
	{
		const std::string randomString = CSVParser::generateRandomString( generateStrLenght );

		// 
		for( const auto &row : rows )
		{
			if( row.find( randomString ) != std::string::npos )
			{
				// found unique string in row, so break loop and generate new string
				break;
			}

			// randomString not found, so randomString is unique!
			this->seperatorMaskingStr = randomString;
			foundUniqueString = true;
		}

	} while( !foundUniqueString );
	// ---

	for( auto &row : rows )
	{
		row = CSVParser::replaceAll( row, std::string{ this->seperator }, this->seperatorMaskingStr );
	}
}

void CSVParser::mapCSVData( const std::vector<std::string> &rows )
{
	size_t rowsCnt = 0;
	size_t cntColumnTitles = 0;

	for( const auto &row : rows )
	{
		++rowsCnt;

		std::stringstream ss{ row };
		std::vector<std::string> columns;

		std::string column;
		while( std::getline( ss, column, this->seperator ) )
		{
			column = CSVParser::replaceAll( column, this->seperatorMaskingStr, std::string{ this->seperator } );
			columns.push_back( column );
		}

		if( rowsCnt == 1 )
		{
			cntColumnTitles = columns.size();
		}

		csvDataMatrix.emplace_back( std::move( columns ) );
	}

	// resize vectors to column-title length
	for( auto &xVec : this->csvDataMatrix )
	{
		xVec.resize( this->csvDataMatrix[0].size() ); // first index contains the vector of column-titles
	}
}