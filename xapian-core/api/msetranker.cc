/** @file  msetranker.cc
 *  @brief Implementation of MSetRanker class
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

#include <algorithm>
#include <iterator>
#include <vector>

#include "msetranker.h"
#include "omenquireinternal.h"

using namespace Xapian;
using namespace std;

bool sortbyweight(const Xapian::Internal::MSetItem &a, const Xapian::Internal::MSetItem &b)
{
    return a.wt > b.wt;
}

Xapian::MSet
MSetRanker::rerank(const std::vector<double> & weights_) const
{
    vector<double> weights = weights_;

    vector<double>::iterator weights_it;
    vector<Xapian::Internal::MSetItem>::iterator items_it;

    for(items_it = matches.internal->items.begin(), weights_it = weights.begin();
         (items_it != matches.internal->items.end()) && weights_it != weights.end(); ++items_it, ++weights_it){

        items_it->wt = *weights_it;
    }

    sort(matches.internal->items.begin(), matches.internal->items.end(), sortbyweight);

    return matches;
}
