# Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

cmake_minimum_required (VERSION 2.8)
project (MANGOS_IOMAP_EXTRACTOR)

LINK_DIRECTORIES( ${LINK_DIRECTORIES} ${CMAKE_SOURCE_DIR}/../../dep/libmpq/libmpq/.libs/ )

add_executable(vmapextractor adtfile.cpp  dbcfile.cpp  model.cpp  mpq_libmpq.cpp  vmapexport.cpp  wdtfile.cpp  wmo.cpp)

target_link_libraries(vmapextractor mpq bz2 z)
