/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		04.10.2018
Last update:	04.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#include "csvparser.h"

#include "file_not_exist_exception.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "generator.h"

CSVParser::CSVParser( const std::string &fileName )
: currentLineReadState{ CSVParser::LINE_READ_STATE::SEARCHING_HEADER_ROW }
, csvFile{ fileName }
{
}

void CSVParser::parse()
{	
	this->resetParser();

	std::ifstream inFile{ this->csvFile };

	if( !inFile )
	{
		const std::string errorMessage = "file couldn't be opened: " + this->csvFile;
		throw FileNotExistException( errorMessage );
	}

	std::string line;

	while( getline( inFile, line ) )
	{
		this->readLine( line );
	}

	this->clearLastEmptyRows(); 
	this->currentLineReadState = LINE_READ_STATE::FINISHED;
}

void CSVParser::resetParser()
{
	this->currentLineReadState = LINE_READ_STATE::SEARCHING_HEADER_ROW;
	this->data_matrix.clear();
}

void CSVParser::readLine( const std::string &line, const char SEPERATOR )
{
	std::vector<std::string> seperatedValues = Generator::split( line, SEPERATOR );

	if( this->currentLineReadState == LINE_READ_STATE::SEARCHING_HEADER_ROW )
	{
		bool canSpanColumnVector = false;

		auto result = std::find_if( seperatedValues.begin(),
									seperatedValues.end(),
									[]( const auto &val ){
										return val.size() > 0;
									}
		);

		if( result != seperatedValues.end() )
		{
			this->spanColumnVector( seperatedValues );
		}
	}
	else
	{
		this->data_matrix.emplace_back( std::move( seperatedValues ) );
	}
}

void CSVParser::spanColumnVector( std::vector<std::string> &columns )
{
	this->data_matrix.emplace_back( std::move(columns) );
	this->currentLineReadState = LINE_READ_STATE::READING_DATA_ROW;
}

void CSVParser::clearLastEmptyRows()
{
	while( this->data_matrix.size() > 0  )
	{
		auto lastItem = this->data_matrix.back();

		auto result = std::find_if( lastItem.begin(),
									lastItem.end(),
									[]( const auto &val ){
										return val.size() > 0;
									}
		);

		if( result == lastItem.end() )
		{
			this->data_matrix.pop_back();
		}
		else
		{
			break;
		}
	}
}