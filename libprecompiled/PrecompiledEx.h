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
/** @file PrecompiledEx.h
*  @author fengqiao
*  @date 20211028
*/
#pragma once
#include <map>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include "Common.h"
#include "PrecompiledResult.h"
#include <libdevcore/Address.h>
#include <libblockverifier/ExecutiveContext.h>
#include <libethcore/ABI.h>
#include <boost/preprocessor.hpp>

namespace bv = dev::blockverifier;

#define BCOS_REG_INTERNAL_PARAM(elem) BOOST_PP_STRINGIZE(BOOST_PP_REMOVE_PARENS(elem))
#define BCOS_REG_INTERNAL_BASE(OP, NAME, FUNC) RegisterFunc(NAME, &OP::FUNC);
#define BCOS_REG_INTERNAL( r, OP, elem )   BCOS_REG_INTERNAL_BASE(OP, BCOS_REG_INTERNAL_PARAM(elem), BOOST_PP_SEQ_ELEM(0, elem))


#define BCOS_CONTRAT_REG(TYPE, MEMBERS) BOOST_PP_SEQ_FOR_EACH(BCOS_REG_INTERNAL, TYPE, MEMBERS)


namespace dev
{
namespace precompiled
{

struct CallParameter {
	bv::ExecutiveContext::Ptr _context;
	Address _origin;
	Address _sender;
	PrecompiledExecResult::Ptr _callcallResult;
};

class PrecompiledEx : public dev::precompiled::Precompiled
{
public:	
	PrecompiledExecResult::Ptr call(bv::ExecutiveContext::Ptr _context,
		bytesConstRef _param, Address const& _origin = Address(),
		Address const& _sender = Address()) override;

	Address GetOriginAddr();
	Address GetSenderAddr();
	bv::ExecutiveContext::Ptr GetContext();
	PrecompiledExecResult::Ptr GetCallResult();

	template<typename F>
	void RegisterFunc(const char* pszFuncName, F func)
	{
		auto selector = getFuncSelector(pszFuncName);
		if (m_handlers.find(selector) != m_handlers.end())
		{
			throw std::invalid_argument(std::string(pszFuncName) + " : this function name  has already exist!");
		}
		m_handlers[selector] = std::bind(&PrecompiledEx::Proxy<F>, this, func, std::placeholders::_1, std::placeholders::_2);
	}

private:

	template<typename F>
	void Proxy(F func,  PrecompiledExecResult::Ptr res, bytesConstRef dataRef)
	{
		_Proxy(func,res, dataRef);
	}

	template<typename R, typename T,  typename... Args>
	void _Proxy(R(T::* func)(Args...), PrecompiledExecResult::Ptr res, 
		bytesConstRef dataRef)
	{
		using ArgsType = std::tuple<typename std::decay<Args>::type...>;
		ArgsType tuple;
		Deserialize(dataRef, tuple);
		CallFunc<R>(func, (T*)this, res, tuple, std::make_index_sequence<sizeof...(Args)>{});
	}

	template<typename R, typename T, typename F, typename Tuple, std::size_t... I>
	typename std::enable_if<std::is_same<R, void>::value, void >::type
		CallFunc(F func, T* pObj, PrecompiledExecResult::Ptr res, Tuple& tup, std::index_sequence<I...>)
	{
		try
		{
			(pObj->*func)(std::get<I>(tup)...);
		}
		catch (const std::exception& e)
		{
			dev::eth::ContractABI abi;
			res->setExecResult(abi.abiIn("", std::string(e.what())));
		}
	}

	template<typename R, typename T,  typename F, typename Tuple, std::size_t... I>
	typename std::enable_if<!std::is_same<R, void>::value, void >::type
		CallFunc(F func, T* pObj, PrecompiledExecResult::Ptr res, Tuple& tup, std::index_sequence<I...>)
	{
		try
		{
			R retValue = std::move((pObj->*func)(std::get<I>(tup)...));
			SetCallResult(retValue, res);
		}
		catch (const std::exception& e)
		{
			dev::eth::ContractABI abi;
			res->setExecResult(abi.abiIn("", std::string(e.what())));
		}		
	}

	template<typename P>
	void SetCallResult(P& val, PrecompiledExecResult::Ptr res)
	{
		dev::eth::ContractABI abi;
		res->setExecResult(abi.abiIn("", val));
	}

	template<typename Tuple, std::size_t... I>
	void _SetCallResult(PrecompiledExecResult::Ptr res, Tuple& tup, std::index_sequence<I...>)
	{
		dev::eth::ContractABI abi;
		res->setExecResult(abi.abiIn("", std::get<I>(tup)...));
	}

	template<typename...Args>
	void SetCallResult(std::tuple<Args...>& val, PrecompiledExecResult::Ptr res)
	{
		_SetCallResult(res, val, std::make_index_sequence<sizeof...(Args)>{ });
	}

	template<typename Tuple, std::size_t... I>
	void _Deserialize(bytesConstRef dataRef, Tuple& tup, std::index_sequence<I...>)
	{
		dev::eth::ContractABI abi;
		abi.abiOut(dataRef, std::get<I>(tup)...);
	}

	template <typename...Args>
	void Deserialize(bytesConstRef dataRef, std::tuple<Args...>& val)
	{
		_Deserialize(dataRef, val, std::make_index_sequence<sizeof...(Args)>{ });
	}
protected:
	CallParameter m_callParam;
private:
	std::map<uint32_t, std::function<void(PrecompiledExecResult::Ptr, bytesConstRef)>> m_handlers;
	
};

}  // namespace precompiled

}  // namespace dev