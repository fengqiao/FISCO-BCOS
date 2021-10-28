#include "PrecompiledEx.h"

using namespace dev;
using namespace dev::precompiled;

PrecompiledExecResult::Ptr PrecompiledEx::call(bv::ExecutiveContext::Ptr _context,
	bytesConstRef _param, Address const& _origin ,
	Address const& _sender )
{
	
	PRECOMPILED_LOG(TRACE) << LOG_BADGE(toString()) << LOG_DESC("call")
		<< LOG_KV("param", toHex(_param));
	// parse function name
	uint32_t func = getParamFunc(_param);
	bytesConstRef data = getParamData(_param);
	auto callResult = m_precompiledExecResultFactory->createPrecompiledResult();
	m_callParam = CallParameter{ _context , _origin, _sender, callResult };
	callResult->gasPricer()->setMemUsed(_param.size());
	dev::eth::ContractABI abi;

	auto itFind = m_handlers.find(func);
	if (itFind == m_handlers.end())
	{
		// unknown function call
		PRECOMPILED_LOG(ERROR) << LOG_BADGE(toString()) << LOG_DESC(" unknown func ")
			<< LOG_KV("func", func);
		callResult->setExecResult(abi.abiIn("", u256(CODE_UNKNOW_FUNCTION_CALL)));
		return callResult;
	}
	itFind->second(callResult, data);
	return callResult;
}

Address dev::precompiled::PrecompiledEx::GetOriginAddr()
{
	return m_callParam._origin;
}

Address dev::precompiled::PrecompiledEx::GetSenderAddr()
{
	return m_callParam._sender;
}

bv::ExecutiveContext::Ptr dev::precompiled::PrecompiledEx::GetContext()
{
	return m_callParam._context;
}

PrecompiledExecResult::Ptr dev::precompiled::PrecompiledEx::GetCallResult()
{
	return m_callParam._callcallResult;
}
