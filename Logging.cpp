/*
Copyright 2016 Laurent Claessens
contact : laurent@claessens-donadello.eu

This is part of 'lora': you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*/

#include <iostream>
#include <string>
#include "Logging.h"

void Logging::setFile(path l_path) 
{
    log_stream.open(l_path.string().c_str(), std::ofstream::trunc);
}
void Logging::write(std::string message) { log_stream<<message<<std::endl; }
