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


#include "block.h"

#include "candidate.h"

#include <algorithm>

Block::Block( const size_t rowIndex, const size_t columnIndex,
			  const std::string &rowName, const std::string &columnName )
	: index{ std::make_pair( rowIndex,columnIndex ) }
	, blockName{ std::make_pair( rowName, columnName ) }
	, assignedCandidate{ nullptr }
{
}

void Block::addCandidate( const Candidate *const candidate )
{
	auto existingCandidate = std::find_if( this->candidates.begin(),
										   this->candidates.end(),
										   [&]( const auto &candidatePtr ){
		return candidatePtr == candidate;
	}
	);

	if( existingCandidate == this->candidates.end() )
	{
		this->candidates.push_back( const_cast<Candidate*>( candidate ) );
	}
}

std::string Block::toString() const
{
	std::stringstream ss;
	ss << "Index: " << this->index.first << "," << this->index.second << std::endl;
	ss << "Block-Name: " << this->blockName.second << " " << this->blockName.first << std::endl;
	ss << "Candidates: ";

	for( const auto &candidate : this->candidates )
	{
		ss << candidate->getName() << ", ";
	}

	return ss.str();
}

void Block::setAssignedCandidate( const Candidate *candidate )
{
	this->assignedCandidate = const_cast<Candidate*>( candidate );

	for( auto &otherCandidate : this->candidates )
	{
		if( this->assignedCandidate->getName() == otherCandidate->getName() )
		{
			continue;
		}

		otherCandidate->removeBlockFromWishlist( this );
	}
}