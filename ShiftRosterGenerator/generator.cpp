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

#include "generator.h"

#include "nullptr_exception.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>

#include "no_data_exception.h"

std::vector<std::string> Generator::split( const std::string &line, const char SEPERATOR )
{
	std::vector<std::string> splittedStrings;

	std::string splittedString;
	std::stringstream ss{ line };

	while( getline( ss, splittedString, SEPERATOR ) )
	{
		splittedStrings.push_back( splittedString );
	}

	return splittedStrings;
}

std::string Generator::replaceAll( std::string str, const std::string& from, const std::string& to )
{
	size_t start_pos = 0;

	while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
	{
		str.replace( start_pos, from.size(), to );
		start_pos += to.size();
	}

	return std::move( str );
}

std::string Generator::trim( std::string str )
{
	size_t first = str.find_first_not_of( ' ' );

	if( first == std::string::npos )
	{
		return "";
	}

	size_t last = str.find_last_not_of( ' ' );

	return std::move( str.substr( first, ( last - first + 1 ) ) );
}

Generator::Generator( const std::string &fileName )
	: csvParser{ ';' }
	, rowCnt{ 0 }
	, columnCnt{ 0 }
{
	this->csvParser.parse( fileName );
	this->distributeBlocks();
}

Candidate* Generator::addCandidate( const Candidate &candidate )
{
	auto existingCandidate = std::find_if( this->candidates.begin(),
										   this->candidates.end(),
										   [&]( const Candidate &value ){
		return candidate.getName() == value.getName();
	}
	);

	if( existingCandidate != this->candidates.end() )
	{
		return &( *existingCandidate );
	}
	else
	{
		this->candidates.push_back( candidate );
		return &( this->candidates.back() );
	}
}

void Generator::generateBlocks()
{
	this->blocks.clear();
	this->candidates.clear();

	auto matrix = this->csvParser.getCSVMatrix();

	this->rowCnt = matrix.size() - 1;

	if( matrix.size() == 0 )
		throw NoDataException( "No data extracted from csv file" );

	this->columnCnt = matrix.at( 0 ).size() - 1;

	// Collect Raw Data from Matrix
	// start i with index 1. Index 0 contains ColumnNames!
	for( size_t i = 1; i < matrix.size(); i++ )
	{
		const std::string currentRowName = matrix[i][0];

		// start j with index 1. Index 0 contains RowNames!
		for( size_t j = 1; j < matrix.at( i ).size(); j++ )

		{
			const std::string currentColumnName = matrix[0][j];

			Block block{ i - 1, j - 1, currentRowName, currentColumnName };

			std::string data = matrix[i][j];
			std::vector<Candidate> candidatesVec = this->splitToCandidates( std::move( data ) );

			// collect Candidates to list and assign them to the new block
			for( const auto &candidate : candidatesVec )
			{
				Candidate *addedCandidate = this->addCandidate( candidate );

				block.addCandidate( addedCandidate );
			}

			// add new Block to list
			this->blocks.push_back( block );
			Block *newBlockRealPtr = &( this->blocks.back() );

			// assign new added Block to candidates to all Candidates
			for( const auto &candidate : candidates )
			{
				Candidate *candidatePtr = this->getCandidate( candidate.getName() );

				if( candidatePtr == nullptr )
				{
					throw NullptrException( "Candidate-Instance is nullptr." );
				}

				candidatePtr->addWishBlock( newBlockRealPtr );
			}
		}
	}
}

std::vector<Candidate> Generator::splitToCandidates( std::string data ) const
{
	std::vector<Candidate> candidatesVec;

	// remove newlines
	data = Generator::replaceAll( data, "\n", "" );

	// remove seperators
	data = Generator::replaceAll( data, std::string{ this->csvParser.getSeperator() }, "" );

	// remove double qoutes
	data = Generator::replaceAll( data, "\"", "" );

	// remove single qoutes
	data = Generator::replaceAll( data, "'", "" );

	// remove tabulators
	data = Generator::replaceAll( data, "\t", "" );

	std::vector<std::string> splittedData = Generator::split( data, ',' );

	for( const auto &name : splittedData )
	{
		std::string trimmedName = this->trim( name );

		if( trimmedName.size() > 0 )
		{
			candidatesVec.push_back( Candidate{ trimmedName } );
		}
	}

	return std::move( candidatesVec );
}

std::string Generator::getResultAsCSV() const
{
	std::stringstream outStream;
	std::vector<std::vector<std::string>> resultMatrix = this->csvParser.getCSVMatrix();

	std::vector<std::vector<const Block*>> matrix{};
	matrix.assign( this->rowCnt, std::vector<const Block*>{ this->columnCnt } );

	for( const auto &block : this->blocks )
	{
		auto index = block.getIndex();

		matrix[index.first][index.second] = &block;
	}

	for( size_t i = 0; i < matrix.size(); ++i )
	{
		for( size_t j = 0; j < matrix[i].size(); ++j )
		{
			const Block *block = matrix[i][j];

			if( block == nullptr )
			{
				continue;
			}

			const Candidate *assignedCandidate = block->getAssignedCandidate();

			if( assignedCandidate != nullptr )
			{
				const std::string candidateName = matrix[i][j]->getAssignedCandidate()->getName();
				resultMatrix[i + 1][j + 1] = candidateName;
			}
			else
			{
				resultMatrix[i + 1][j + 1] = "";
			}
		}
	}

	// collect result in stringstream
	for( size_t i = 0; i < resultMatrix.size(); ++i )
	{
		for( size_t j = 0; j < resultMatrix[i].size(); ++j )
		{
			std::string data = resultMatrix[i][j];
			outStream << data << this->csvParser.getSeperator();
		}
		outStream << std::endl;
	}

	return outStream.str();
}

void Generator::distributeBlocks()
{
	this->generateBlocks();

	// TODO: implement algorithm here
	size_t candidateCount = 1;

	while( countFreeDistributableBlocks() && countFreeDistributableCandidates() )
	{
		while( this->getFreeBlocksByCandidatesCnt( candidateCount ).size() > 0 )
		{
			Block *mostCriticalBlock = getMostCriticalBlock( this->getFreeBlocksByCandidatesCnt( candidateCount ) );

			if( mostCriticalBlock != nullptr )
			{
				Candidate *mostCriticalCandidate = this->getMostCriticalCandidate( mostCriticalBlock->getCandidates() );

				if( mostCriticalCandidate != nullptr )
				{
					mostCriticalCandidate->addAssignedBlock( mostCriticalBlock );
					mostCriticalBlock->setAssignedCandidate( mostCriticalCandidate );
				}
			}
		}

		++candidateCount;
	}
}

size_t Generator::countFreeDistributableBlocks() const
{
	size_t cntFreeBlocks = 0;

	for( const auto &block : this->blocks )
	{
		if( block.isFree() && block.getCandidateCnt() > 0 )
		{
			++cntFreeBlocks;
		}
	}

	return cntFreeBlocks;
}

size_t Generator::countFreeDistributableCandidates() const
{
	size_t cntFreeCandidates = 0;

	for( const auto &candidate : this->candidates )
	{
		if( candidate.isFree() && candidate.getWishedBlockCnt() > 0 )
		{
			++cntFreeCandidates;
		}
	}

	return cntFreeCandidates;
}

std::vector<Block*> Generator::getFreeBlocksByCandidatesCnt( const size_t candidatesCnt ) const
{
	std::vector<Block*> candidateCountedBlocks;

	for( auto &block : this->blocks )
	{
		if( block.isFree() && block.getCandidateCnt() == candidatesCnt )
		{
			candidateCountedBlocks.push_back( const_cast<Block*>( &block ) );
		}
	}

	return std::move( candidateCountedBlocks );
}

Block* Generator::getMostCriticalBlock( const std::vector<Block*> &blocksVec ) const
{
	if( blocksVec.size() == 0 )
	{
		return nullptr;
	}
	else if( blocksVec.size() == 1 )
	{
		return const_cast<Block*>( blocksVec.at( 0 ) );
	}
	else
	{
		auto smallestWeigthingBlock = std::min_element( blocksVec.begin(), blocksVec.end(),
														[this]( const Block *a, const Block *b ) -> bool{
			auto candidatesA = a->getCandidates();
			auto candidatesB = b->getCandidates();

			return this->countWishWeighting( candidatesA ) < this->countWishWeighting( candidatesB );
		}
		);

		return const_cast<Block*>( *smallestWeigthingBlock );
	}
}

Candidate* Generator::getMostCriticalCandidate( const std::vector<Candidate*> &candidatesVec ) const
{
	if( candidatesVec.size() == 0 )
	{
		return nullptr;
	}
	else if( candidatesVec.size() == 1 )
	{
		return const_cast<Candidate*>( candidatesVec.at( 0 ) );
	}
	else
	{
		auto smallestWeigthingCandidate = std::min_element( candidatesVec.begin(), candidatesVec.end(),
															[this]( const Candidate *a, const Candidate *b ) -> bool{

			if( a->getAssignedBlockCnt() > b->getAssignedBlockCnt() )
			{
				return false;
			}
			else if( a->getAssignedBlockCnt() < b->getAssignedBlockCnt() )
			{
				return true;
			}
			else
			{
				if( a->getWishedBlockCnt() >= b->getWishedBlockCnt() )
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}
		);

		return const_cast<Candidate*>( *smallestWeigthingCandidate );
	}
}