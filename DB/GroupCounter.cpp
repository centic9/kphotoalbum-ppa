/* Copyright (C) 2003-2010 Jesper K. Pedersen <blackie@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "GroupCounter.h"
#include "DB/MemberMap.h"
#include "DB/ImageDB.h"
#include "Utilities/Set.h"
using namespace DB;


/**
 * \class DB::GroupCounter
 * \brief Utility class to help counting matches for member groups.
 *
 * This class is used to count the member group matches when
 * categorizing. The class is instantiating with the category we currently
 * are counting items for.
 *
 * The class builds the inverse member map, that is a map pointing from items
 * to parent.
 *
 * As an example, imagine we have the following member map (stored in the
 * variable groupToMemberMap in  the code):
 * \code
 *    { USA |-> [Chicago, Santa Clara],
 *      California |-> [Santa Clara, Los Angeles] }
 * \endcode
 *
 * The inverse map (stored in _memberToGroup in the code ) will then look
 * like this:
 * \code
 *  { Chicago |-> [USA],
 *    Sanata Clara |-> [ USA, California ],
 *    Los Angeless |-> [ California ] }
 * \endcode
 */
GroupCounter::GroupCounter( const QString& category )
{
    const MemberMap map = DB::ImageDB::instance()->memberMap();
    QMap<QString,StringSet> groupToMemberMap = map.groupMap(category);

    _memberToGroup.reserve( 2729 /* A large prime */ );
    _groupCount.reserve( 2729 /* A large prime */ );

    // Populate the _memberToGroup map
    for( QMap<QString,StringSet>::Iterator groupToMemberIt= groupToMemberMap.begin();
         groupToMemberIt != groupToMemberMap.end(); ++groupToMemberIt ) {

        StringSet members = groupToMemberIt.value();
        QString group = groupToMemberIt.key();

        for( StringSet::const_iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt ) {
            _memberToGroup[*memberIt].append( group );
        }
        _groupCount.insert( group, 0 );
    }
}

/**
 * categories is the selected categories for one image, members may be Las Vegas, Chicago, and Los Angeles if the
 * category in question is Places.
 * This function then increases _groupCount with 1 for each of the groups the relavant items belongs to
 * Las Vegas might increase the _groupCount[Nevada] by one.
 * The tricky part is to avoid increasing it by more than 1 per image, that is what the countedGroupDict is
 * used for.
 */
void GroupCounter::count( const StringSet& categories )
{
    static StringSet countedGroupDict;

    countedGroupDict.clear();
    for( StringSet::const_iterator categoryIt = categories.begin(); categoryIt != categories.end(); ++categoryIt ) {
        if ( _memberToGroup.contains(*categoryIt)) {
            const QStringList groups = _memberToGroup[*categoryIt];
            Q_FOREACH( const QString& group, groups ) {
                if ( !countedGroupDict.contains( group ) ) {
                    countedGroupDict.insert( group );
                    (_groupCount[group])++;
                }
            }
        }
        // The item Nevada should itself go into the group Nevada.
        if ( !countedGroupDict.contains( *categoryIt ) == 0 && _groupCount.contains( *categoryIt ) ) {
             countedGroupDict.insert( *categoryIt);
             (_groupCount[*categoryIt])++;
        }
    }
}

QMap<QString,uint> GroupCounter::result()
{
    QMap<QString,uint> res;

    Q_FOREACH( const QString& key, _groupCount.keys()) {
        if ( _groupCount[key] != 0 )
            res.insert( key, _groupCount[key] );
    }
    return res;
}
// vi:expandtab:tabstop=4 shiftwidth=4:
