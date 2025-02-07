/*
* Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk>
* Advanced Processor Technologies Group, School of Computer Science
* University of Manchester, Manchester M13 9PL UK
*
* This source code is free software; you can redistribute it
* and/or modify it in source code form under the terms of the GNU
* General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

/*
* Saif parser wrapper class
* 07/02/2014 Wei Song
*
*
*/

#include "saif_parser.hpp"

#include <fstream>
#include "saif_util.hpp"
#include "saif.hh"
#include "saif_db.hpp"


saif::SaifParser::SaifParser(const std::string& fn)
  : saif_file(fn) {}


bool saif::SaifParser::parse(SaifDB * db)
{
  std::fstream saif_file_handler;
  saif_file_handler.open(saif_file);
  if (!saif_file_handler.is_open())
    { std::cerr << "Cannot open file: " << saif_file << "\n"; exit(-1); }

  SaifLexer * lexer = new SaifLexer(&saif_file_handler);
  saif_parser * parser = new saif_parser(lexer, db);
  return 0 == parser->parse();
}
