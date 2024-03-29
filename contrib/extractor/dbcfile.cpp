/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2012 MaNGOSZero <https://github.com/mangos-zero>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define _CRT_SECURE_NO_DEPRECATE

#include <cstdio>

#include "dbcfile.h"
#include "mpq_libmpq.h"

DBCFile::DBCFile(const std::string &filename) : filename(filename)
{
    data = NULL;
}

bool DBCFile::open()
{
    MPQFile f(filename.c_str());
    char header[4];
    unsigned int na,nb,es,ss;

    // Need some error checking, otherwise an unhandled exception error occurs
    // if people screw with the data path.
    if (f.isEof() == true)
        return false;

    if(f.read(header,4)!=4)                                 // Number of records
        return false;

    if (header[0]!='W' || header[1]!='D' || header[2]!='B' || header[3] != 'C')
    {
        f.close();
        data = NULL;
        printf("Critical Error: An error occured while trying to read the DBCFile %s.", filename.c_str());
        return false;
    }

    if(f.read(&na,4)!=4)                                    // Number of records
        return false;
    if(f.read(&nb,4)!=4)                                    // Number of fields
        return false;
    if(f.read(&es,4)!=4)                                    // Size of a record
        return false;
    if(f.read(&ss,4)!=4)                                    // String size
        return false;

    recordSize = es;
    recordCount = na;
    fieldCount = nb;
    stringSize = ss;
    if(fieldCount*4 != recordSize)
        return false;

    data = new unsigned char[recordSize*recordCount+stringSize];
    stringTable = data + recordSize*recordCount;

    size_t data_size = recordSize*recordCount+stringSize;
    if(f.read(data,data_size)!=data_size)
        return false;
    f.close();
    return true;
}

DBCFile::~DBCFile()
{
    delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(data);
    return Record(*this, data + id*recordSize);
}

size_t DBCFile::getMaxId()
{
    assert(data);

    size_t maxId = 0;
    for(size_t i = 0; i < getRecordCount(); ++i)
    {
        if(maxId < getRecord(i).getUInt(0))
            maxId = getRecord(i).getUInt(0);
    }
    return maxId;
}

DBCFile::Iterator DBCFile::begin()
{
    assert(data);
    return Iterator(*this, data);
}

DBCFile::Iterator DBCFile::end()
{
    assert(data);
    return Iterator(*this, stringTable);
}
