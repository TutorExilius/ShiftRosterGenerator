/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		04.10.2018
Last update:	09.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>

class CSVParser
{
public:
	enum class LINE_READ_STATE
	{
		SEARCHING_HEADER_ROW,
		READING_DATA_ROW,
		FINISHED
	};

	explicit CSVParser( const std::string &fileName, const char SEPERATOR = ';' );

	void parse();

	const std::vector<std::vector<std::string>>& getDataMatrix() const
	{
		return this->data_matrix;
	}

private:
	// deletes ---
	CSVParser( const CSVParser& ) = delete;
	CSVParser( CSVParser&& ) = delete;
	CSVParser& operator=( const CSVParser& ) = delete;
	CSVParser&& operator=( CSVParser&& ) = delete;
	// ---

	// Helper-Methods ---
	void resetParser();
	void readLine( const std::string &line, const char SEPERATOR = ';' );
	void spanColumnVector( std::vector<std::string> &columns );
	void clearLastEmptyRows();
	// ---

	LINE_READ_STATE currentLineReadState;
	std::string csvFile;
	std::vector<std::vector<std::string>> data_matrix;
	char seperator;
};

#endif // CSVPARSER_H