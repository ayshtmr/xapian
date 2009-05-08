/** @file decvalwtsource.cc
 * @brief A posting source which returns decreasing weights from a value.
 */
/* Copyright (C) 2009 Lemur Consulting Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <config.h>

#include "xapian/decvalwtsource.h"
#include "xapian/error.h"
#include "serialise.h"
#include "serialise-double.h"
#include <cmath>

using namespace Xapian;

DecreasingValueWeightPostingSource::DecreasingValueWeightPostingSource(
    Xapian::valueno slot_,
    Xapian::docid range_start_,
    Xapian::docid range_end_)
	: Xapian::ValueWeightPostingSource(slot_),
	  range_start(range_start_),
	  range_end(range_end_)
{
}

Xapian::weight
DecreasingValueWeightPostingSource::get_weight() const {
    return curr_weight;
}

Xapian::DecreasingValueWeightPostingSource *
DecreasingValueWeightPostingSource::clone() const {
    return new DecreasingValueWeightPostingSource(slot, range_start,
						  range_end);
}

std::string
DecreasingValueWeightPostingSource::name() const {
    return "Xapian::DecreasingValueWeightPostingSource";
}

std::string
DecreasingValueWeightPostingSource::serialise() const {
    std::string result;
    result += encode_length(slot);
    result += encode_length(range_start);
    result += encode_length(range_end);
    return result;
}

Xapian::DecreasingValueWeightPostingSource *
DecreasingValueWeightPostingSource::unserialise(const std::string &s) const {
    const char * pos = s.data();
    const char * end = pos + s.size();
    Xapian::valueno new_slot = decode_length(&pos, end, false);
    Xapian::docid new_range_start = decode_length(&pos, end, false);
    Xapian::docid new_range_end = decode_length(&pos, end, false);
    if (pos != end)
	throw Xapian::NetworkError("Junk at end of serialised "
				   "DecreasingValueWeightPostingSource");
    return new DecreasingValueWeightPostingSource(new_slot, new_range_start,
						  new_range_end);
}

void
DecreasingValueWeightPostingSource::init(const Xapian::Database & db_) {
    Xapian::ValueWeightPostingSource::init(db_);
    seen_weight_in_range = false;
    if (range_end == 0 || db.get_doccount() <= range_end)
	items_at_end = false;
    else
	items_at_end = true;
}

void
DecreasingValueWeightPostingSource::skip_if_in_range(Xapian::weight min_wt)
{
    if (value_it == db.valuestream_end(slot)) return;
    curr_weight = Xapian::ValueWeightPostingSource::get_weight();
    Xapian::docid docid = Xapian::ValueWeightPostingSource::get_docid();
    if (docid >= range_start && (range_end == 0 || docid <= range_end)) {
	if (items_at_end) {
	    if (curr_weight < min_wt) {
		// skip to end of range.
		value_it.skip_to(range_end + 1);
		if (value_it != db.valuestream_end(slot))
		    curr_weight = Xapian::ValueWeightPostingSource::get_weight();
	    }
	} else {
	    if (curr_weight < min_wt) {
		// terminate early.
		value_it = db.valuestream_end(slot);
	    } else {
		// Update max_weight.
		max_weight = curr_weight;
		if (!seen_weight_in_range) {
		    first_weight_in_range = curr_weight;
		    notify_new_maxweight();
		    last_weight_band = 100;
		} else {
		    if (first_weight_in_range != 0) {
			int weight_band = floor(100.0 * curr_weight / first_weight_in_range);
			if (weight_band != last_weight_band) {
			    notify_new_maxweight();
			    last_weight_band = weight_band;
			}
		    }
		}
	    }
	}
    }
}

void
DecreasingValueWeightPostingSource::next(Xapian::weight min_wt) {
    if (max_weight < min_wt) {
	value_it = db.valuestream_end(slot);
	return;
    }
    Xapian::ValueWeightPostingSource::next(min_wt);
    skip_if_in_range(min_wt);
}

void
DecreasingValueWeightPostingSource::skip_to(Xapian::docid min_docid,
					    Xapian::weight min_wt) {
    if (max_weight < min_wt) {
	value_it = db.valuestream_end(slot);
	return;
    }
    Xapian::ValueWeightPostingSource::skip_to(min_docid, min_wt);
    skip_if_in_range(min_wt);
}

bool
DecreasingValueWeightPostingSource::check(Xapian::docid min_docid,
					  Xapian::weight min_wt) {
    if (max_weight < min_wt) {
	value_it = db.valuestream_end(slot);
	return true;
    }
    bool valid = Xapian::ValueWeightPostingSource::check(min_docid, min_wt);
    if (valid) {
	skip_if_in_range(min_wt);
    }
    return valid;
}

std::string
DecreasingValueWeightPostingSource::get_description() const {
    return "DecreasingValueWeightPostingSource()";
}
