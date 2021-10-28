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
/** @file HelloPrecompiled.cpp
*  @author fengqiao
*  @date 20211028
*/

#include "HelloPrecompiled.h"

using namespace dev::precompiled;


HelloPrecompiled::HelloPrecompiled()
{
	/*RegisterFunc("get(string)", &HelloPrecompiled::get);
	RegisterFunc("set(string,string)", &HelloPrecompiled::set);
	RegisterFunc("mget(string)", &HelloPrecompiled::mget);
	RegisterFunc("sget(string,string)", &HelloPrecompiled::sget);
	RegisterFunc("add(string)", &HelloPrecompiled::add);*/

	BCOS_CONTRAT_REG(HelloPrecompiled,
		((get)(string))
		((set)(string, string))
		((mget)(string))
		((sget)(string,string))
		((add)(string))
	)
}

std::string HelloPrecompiled::get(const std::string& key)
{
	if (key == "hello")
	{
		return "world";
	}
	return std::string();
}

std::tuple<std::string, int> HelloPrecompiled::mget(const std::string & key)
{
	if (key == "hello")
	{
		return{ "world",1 };
	}
	return {key, 0};
}

void HelloPrecompiled::set(const std::string& key, const std::string& val)
{
	auto k = key;
	auto v = val;
}


std::string HelloPrecompiled::sget(const std::string& key1, const std::string& key2)
{
	if (key1 == "hello" && key2 == "world")
	{
		return "Welcome To HelloPrecompiled!";
	}
	return key1 + "," + key2 + " Mismatch";
}

std::string HelloPrecompiled::add(const std::string& key)
{
	return key + " add success:";
}