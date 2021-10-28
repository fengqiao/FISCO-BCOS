/*
* @CopyRight:
* FISCO-BCOS is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* FISCO-BCOS is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
* (c) 2016-2018 fisco-dev contributors.
*/
/** @file HelloPrecompiled.h
*  @author fengqiao
*  @date 20211028
*/
#pragma once
#include "libprecompiled/PrecompiledEx.h"
#include <string>

namespace dev
{
namespace precompiled
{

class HelloPrecompiled : public PrecompiledEx
{
public:
	HelloPrecompiled();

	void set(const std::string& key, const std::string& val);
	std::string get(const std::string& key);
	std::tuple<std::string, int> mget(const std::string& key);
	std::string sget(const std::string& key1, const std::string& key2);

	std::string add(const std::string& key);
};


}  // namespace precompiled

}  // namespace dev