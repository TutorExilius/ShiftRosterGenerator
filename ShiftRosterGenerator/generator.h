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


#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <list>

#include "block.h"
#include "candidate.h"
#include "csvparser.h"

class Generator
{
public:
	static std::vector<std::string> split( const std::string &line, const char SEPERATOR );

	explicit Generator( const std::string &fileName );

	void printBlocks() const;
	std::string getResultAsCSV( const char SEPERATOR = ';' );

	Candidate* getCandidate( const std::string &name )
	{
		for( auto &candidate : this->candidates )
		{
			if( candidate.getName() == name )
			{
				return &candidate;
			}
		}

		return nullptr;
	}

	Block* getMostCriticalBlock( const std::vector<Block*> &blocks ) const;
	Candidate* getMostCriticalCandidate( const std::vector<Candidate*> &candidates ) const;

	size_t countWishWeighting( const std::vector<Candidate*> candidates ) const
	{
		size_t wishWeigth = 0;

		for( const auto &candidate : candidates )
		{
			wishWeigth += candidate->getWishedBlockCnt();
		}

		return wishWeigth;
	}

private:
	// deletes ---
	Generator( const Generator& ) = delete;
	Generator( Generator&& ) = delete;
	Generator& operator=( const Generator& ) = delete;
	Generator&& operator=( Generator&& ) = delete;
	// ---

	// Helper-Methods ---
	Candidate* addCandidate( const Candidate &candidate );
	void generateBlocks();
	std::vector<Candidate> splitToCandidates( const std::string &data ) const;
	void distributeBlocks();
	size_t countFreeDistributableBlocks() const;
	size_t countFreeDistributableCandidates() const;
	std::vector<Block*> getFreeBlocksByCandidatesCnt( const size_t candidatesCnt ) const;
	std::string trim( std::string str ) const;
	// ---

	CSVParser csvParser;

	// do not use the std::vector container for 'blocks' and candidates'! 
	// std::vector re-allocate memory for the entire vectot after
	// continues push_backs.
	// The classes "Candidate" and "Block" uses references on those!
	// References will be invalid.
	std::list<Block> blocks;
	std::list<Candidate> candidates;

	size_t rowCnt;
	size_t columnCnt;
};

#endif // GENERATOR_H