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


#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <sstream>
#include <vector>

// Forward-Declarations
class Candidate;

class Block
{
public:
	explicit Block( const size_t rowIndex, const size_t columnIndex,
					const std::string &rowName, const std::string &columnName );

	void addCandidate( const Candidate *candidate );

	std::string toString() const;

	bool isFree() const
	{
		return ( this->assignedCandidate == nullptr );
	}

	size_t getCandidateCnt() const
	{
		return this->candidates.size();
	}

	const std::vector<Candidate*>& getCandidates() const
	{
		return this->candidates;
	}

	void setAssignedCandidate( const Candidate *candidate );

	const Candidate* getAssignedCandidate() const
	{
		return this->assignedCandidate;
	}

	std::pair<const size_t, const size_t> getIndex() const
	{
		return this->index;
	}

private:
	std::pair<const size_t, const size_t> index;

	std::vector<Candidate*> candidates;
	std::pair<std::string, std::string> blockName;
	Candidate *assignedCandidate;
};

#endif // BLOCK_H