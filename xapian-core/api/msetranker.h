/** @file  msetranker.h
 *  @brief Class to re-rank list of search results with updated weights
 */
/* Copyright (C) 2016 Ayush Tomar
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef XAPIAN_INCLUDED_MSETRANKER_H
#define XAPIAN_INCLUDED_MSETRANKER_H

#include <xapian.h>

#include <vector>

namespace Xapian {

/// Class to re-rank list of search results with updated weights
class MSetRanker {

  public:

    /// MSet to be re-ranked
    Xapian::MSet matches;

    /// Constructor
    MSetRanker(const Xapian::MSet mset):matches(mset) { }

    /// Destructor.
    ~MSetRanker();

    /// Performs re-ranking of MSet with updated weights.
    Xapian::MSet rerank(const std::vector<double> & weights) const;

  private:

    /// Copying not allowed.
    MSetRanker(const MSetRanker & o);

    /// Copying not allowed.
    MSetRanker & operator=(const MSetRanker & o);

};

}

#endif // XAPIAN_INCLUDED_MSETRANKER_H
