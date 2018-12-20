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

/**
 * @brief : external interface of blockchain
 * @author: mingzhenliu
 * @date: 2018-09-21
 */
#pragma once

#include <libdevcore/FixedHash.h>
#include <libethcore/Block.h>
#include <libethcore/Common.h>
#include <libethcore/Transaction.h>
#include <libethcore/TransactionReceipt.h>
namespace dev
{
namespace blockverifier
{
class ExecutiveContext;
}  // namespace blockverifier
namespace blockchain
{
enum class CommitResult
{
    OK = 0,             // 0
    ERROR_NUMBER = -1,  // 1
    ERROR_PARENT_HASH = -2,
    ERROR_COMMITTING = -3
};
// Configuration item written to the table of genesis block,
// groupMark/consensusType/storageType/stateType excluded.
struct GenesisBlockParam
{
    std::string groupMark;  // Data written to extra data of genesis block.
    dev::h512s minerList;
    dev::h512s observerList;
    std::string consensusType;
    std::string storageType;
    std::string stateType;
    uint64_t txCountLimit;
    uint64_t txGasLimit;
};
class BlockChainInterface
{
public:
    BlockChainInterface() = default;
    virtual ~BlockChainInterface(){};
    virtual int64_t number() = 0;
    virtual dev::h256 numberHash(int64_t _i) = 0;
    virtual dev::eth::Transaction getTxByHash(dev::h256 const& _txHash) = 0;
    virtual dev::eth::LocalisedTransaction getLocalisedTxByHash(dev::h256 const& _txHash) = 0;
    virtual dev::eth::TransactionReceipt getTransactionReceiptByHash(dev::h256 const& _txHash) = 0;
    virtual dev::eth::LocalisedTransactionReceipt getLocalisedTxReceiptByHash(
        dev::h256 const& _txHash) = 0;
    virtual std::shared_ptr<dev::eth::Block> getBlockByHash(dev::h256 const& _blockHash) = 0;
    virtual std::shared_ptr<dev::eth::Block> getBlockByNumber(int64_t _i) = 0;
    virtual CommitResult commitBlock(
        dev::eth::Block& block, std::shared_ptr<dev::blockverifier::ExecutiveContext>) = 0;
    virtual std::pair<int64_t, int64_t> totalTransactionCount() = 0;
    virtual dev::bytes getCode(dev::Address _address) = 0;


    /// If it is a genesis block, function returns true.
    /// If it is a subsequent block with same extra data, function returns true.
    /// Returns an error in the rest of the cases.
    virtual bool checkAndBuildGenesisBlock(GenesisBlockParam& initParam) = 0;
    /// get miner or observer nodes
    virtual dev::h512s minerList() = 0;
    virtual dev::h512s observerList() = 0;
    /// get system config
    virtual std::string getSystemConfigByKey(std::string const& key) = 0;

    /// Register a handler that will be called once there is a new transaction imported
    template <class T>
    dev::eth::Handler<> onReady(T const& _t)
    {
        return m_onReady.add(_t);
    }

protected:
    ///< Called when a subsequent call to import transactions will return a non-empty container. Be
    ///< nice and exit fast.
    dev::eth::Signal<> m_onReady;
};
}  // namespace blockchain
}  // namespace dev
