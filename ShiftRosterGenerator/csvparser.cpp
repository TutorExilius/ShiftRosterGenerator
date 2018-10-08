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
#include <sstream>

#include "generator.h"

CSVParser::CSVParser( const std::string &fileName, const char SEPERATOR )
: currentLineReadState{ CSVParser::LINE_READ_STATE::SEARCHING_HEADER_ROW }
, csvFile{ fileName }
, seperator{ SEPERATOR }
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
	std::vector<std::string> allColumnData;

	// Read all Data and mask all newlines in each single Data-Columns-Field value
	while (getline(inFile, line, this->seperator))
	{
		if (line.size() > 0)
		{
			// CSV masks value of column field in quotes (") if field value has more than 1 line in text
			if( line[0] == '\"' && line[line.size() - 1] == '\"' )
			{
				// mask newlines
				line = Generator::replaceAll(line, "\"", "");
				line = Generator::replaceAll(line, "\n", "\\n");
				line = Generator::trim(line);
			}
			else if (line[line.size() - 1] == '\"' && line.find("\n") != std::string::npos )
			{
				allColumnData.push_back( line.substr(0, line.find("\n")+1 ) );
				
				if( line.size() >= line.find("\n") + 1 )
				{
					line = line.substr(line.find("\n")+1);
				}
				else
				{
					line = line.substr(line.find("\n"));
				}

				line = Generator::replaceAll(line, "\"", "");
				line = Generator::replaceAll(line, "\n", "\\n");
				line = Generator::trim(line);
			}
		}
		allColumnData.push_back( line );
	}

	std::stringstream ss;

	// write all Column-Data back to a stream after masking newlines in each Data-Columns-Field value 
	for (const auto &data : allColumnData)
	{
		ss << data;

		if( (data.size() == 0) || (data[data.size() - 1] != '\n') )
		{
			ss << this->seperator;
		}
	}

	while( getline( ss, line ) )
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
		auto result = std::find_if( seperatedValues.begin(),
									seperatedValues.end(),
									[]( const auto &val ){
										return val.size() > 0;
									}
		);

		if( result != seperatedValues.end() )
		{
			std::for_each( seperatedValues.begin(), seperatedValues.end(),
				[](std::string &value) {
					// unmask newlines in row-titles
					value = Generator::replaceAll(value, "\\n", "\n");
				}
			);

			this->spanColumnVector( seperatedValues );
		}
	}
	else
	{
		// unmask column-title. First value in vactor represents the column title!
		if( seperatedValues.size() > 0 )
		{
			// first value = index 0!
			seperatedValues[0] = Generator::replaceAll( seperatedValues[0], "\\n", "\n" );
		}

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