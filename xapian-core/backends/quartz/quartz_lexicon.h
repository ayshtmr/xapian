/* quartz_lexicon.h: Lexicon in a quartz database
 *
 * ----START-LICENCE----
 * Copyright 1999,2000 BrightStation PLC
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 * -----END-LICENCE-----
 */

#ifndef OM_HGUARD_QUARTZ_LEXICON_H
#define OM_HGUARD_QUARTZ_LEXICON_H

#include "config.h"
#include "quartz_table.h"
#include "om/omtypes.h"

/** The lexicon in a quartz database.
 *  The lexicon stores an entry for each term in the database.  This entry
 *  contains:
 *   - a termid, for use in accessing a posting list or a position
 *     list for that term.
 *   - the term frequency of the term (ie, how many documents contain
 *     the term.)
 */
class QuartzLexicon {
    private:
	/** Make a key for accessing a given entry in the lexicon.
	 */
	static void make_key(QuartzDbKey & key,
			     const om_termname & tname);

	/** Parse an entry from the lexicon.
	 */
	static void parse_entry(const std::string & data,
				om_termid * tid,
				om_doccount * termfreq);

	/** Make an entry to go into the lexicon.
	 */
	static void make_entry(std::string & data,
			       om_termid tid,
			       om_doccount termfreq);

    public:
	/** Add an entry to the lexicon.
	 */
	static void add_entry(QuartzBufferedTable * table,
			      const om_termname & tname,
			      om_termid tid,
			      om_doccount termfreq);

	/** Remove an entry from the lexicon.  If the entry
	 *  doesn't already exist, no action is taken.
	 */
	static void delete_entry(QuartzBufferedTable * table,
				 const om_termname & tname);


	/** Get an entry from the lexicon.
	 *
	 *  @param table    The table which the lexicon is stored in.
	 *  @param tname    The term being looked up in the lexicon.
	 *  @param tid      A pointer to a value which is filled with the
	 *                  term ID of the term looked up, if found.
	 *  @param termfreq A pointer to a value which is filled with the
	 *                  term frequency.
	 *
	 *  @return       true if term was found in lexicon, false
	 *                otherwise.
	 */
	static bool get_entry(QuartzTable * table,
			      const om_termname & tname,
			      om_termid * tid,
			      om_doccount * termfreq);
};

#endif /* OM_HGUARD_QUARTZ_LEXICON_H */
