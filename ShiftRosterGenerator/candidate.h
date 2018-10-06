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


#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>

// Forward-Declarations
class Block;

class Candidate
{
public:
	Candidate( const std::string &name );

	void addWishBlock( const Block *block );
	void addAssignedBlock( const Block *block );

	const std::string& getName() const
	{
		return this->name;
	}

	void setName( const std::string &newName )
	{
		this->name = newName;
	}

	bool isFree() const
	{
		return ( wishedBlocks.size() > 0 );
	}

	size_t getWishedBlockCnt() const
	{
		return this->wishedBlocks.size();
	}

	size_t getAssignedBlockCnt() const
	{
		return this->assignedBlocks.size();
	}

	void removeBlockFromWishlist( const Block *const block );

private:
	std::string name;
	std::vector<Block*> wishedBlocks;
	std::vector<Block*> assignedBlocks;
};

#endif