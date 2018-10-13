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


#include "candidate.h"

#include "block.h"

#include <algorithm>

Candidate::Candidate( const std::string &name )
	: name{ name }
{
}

void Candidate::addWishBlock( const Block *const block )
{
	auto existingBlock = std::find_if( this->wishedBlocks.begin(),
									   this->wishedBlocks.end(),
									   [&]( const auto &blockPtr ){
		return blockPtr == block;
	}
	);

	if( existingBlock == this->wishedBlocks.end() )
	{
		this->wishedBlocks.push_back( const_cast<Block*>( block ) );
	}
}

void Candidate::addAssignedBlock( const Block *const block )
{
	auto existingBlock = std::find_if( this->assignedBlocks.begin(),
									   this->assignedBlocks.end(),
									   [&]( const auto &blockPtr ){
		return blockPtr == block;
	}
	);

	if( existingBlock == this->assignedBlocks.end() )
	{
		this->assignedBlocks.push_back( const_cast<Block*>( block ) );
	}
}

void Candidate::removeBlockFromWishlist( const Block *const block )
{
	auto existingBlock = std::find( this->wishedBlocks.begin(),
									this->wishedBlocks.end(),
									block );

	if( existingBlock != this->wishedBlocks.end() )
	{
		this->wishedBlocks.erase( existingBlock );
	}
}
