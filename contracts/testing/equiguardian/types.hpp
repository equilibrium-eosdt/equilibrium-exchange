#ifndef TYPES_HPP
#define TYPES_HPP

#ifdef COMMON

#include <eosio/testing/tester.hpp>
#define ds_time eosio::chain::time_point_sec
#define ds_account eosio::chain::name
#define ds_asset eosio::chain::asset
#define ds_symbol eosio::chain::symbol
#define ds_checksum eosio::chain::checksum256_type

#else
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>

#define ds_time eosio::time_point_sec
#define ds_account eosio::name
#define ds_symbol eosio::symbol
#define ds_asset eosio::asset
#define ds_checksum capi_checksum256
#define N(X) name{#X}
#endif

#define DELETEDATA
#define TESTNET
#define DEBUG

#ifdef DEBUG
#define PRINT_STARTED(ACTION) ds_print("\r\n[%] % started.", __LINE__,ACTION);
#define PRINT_FINISHED(ACTION) ds_print("\r\n[%] % finished.", __LINE__,ACTION);
#else
#define PRINT_STARTED(ACTION)
#define PRINT_FINISHED(ACTION)
#endif

#define ds_int int
#define ds_uint unsigned int
#define ds_ulong unsigned long long
#define ds_long long long
#define ds_llong __int128_t
#define ds_string std::string
#define ds_nullptr std::nullptr_t

#define EOS_SYMBOL_STR "EOS"
#define EOS_SYMBOL_DECIMAL 4
#define USD_SYMBOL_STR "USD"
#define USD_SYMBOL_DECIMAL 4
#define STABLE_SYMBOL_STR "EOSDT"
#define STABLE_SYMBOL_PAD_STR " " STABLE_SYMBOL_STR
#define STABLE_SYMBOL_DECIMAL 9
#define UTILITY_SYMBOL_STR "NUT"
#define UTILITY_SYMBOL_PAD_STR " " UTILITY_SYMBOL_STR
#define UTILITY_SYMBOL_DECIMAL 9

#define EOSCTRACT_STR "eosio.token"
#define EOSDTCNTRACT_STR "eosdtcntract"
#define EOSDTORCLIZE_STR "eosdtorclize"
#define ORACLERATES_STR "orarates"

#ifdef COMMON
#define EOSCTRACT N(eosio.token)
#define EOSDTCNTRACT N(eosdtcntract)
#define EOSDTORCLIZE N(eosdtorclize)
#define ORACLERATES N(orarates)
#define EOS_SYMBOL ds_symbol(EOS_SYMBOL_DECIMAL,EOS_SYMBOL_STR)
#define EOS_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(EOS_SYMBOL_DECIMAL,EOS_SYMBOL_STR)
#define USD_SYMBOL ds_symbol(USD_SYMBOL_DECIMAL,USD_SYMBOL_STR)
#define USD_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(USD_SYMBOL_DECIMAL,USD_SYMBOL_STR)
#define STABLE_SYMBOL ds_symbol(STABLE_SYMBOL_DECIMAL,STABLE_SYMBOL_STR)
#define STABLE_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(STABLE_SYMBOL_DECIMAL,STABLE_SYMBOL_STR)
#define UTILITY_SYMBOL ds_symbol(UTILITY_SYMBOL_DECIMAL,UTILITY_SYMBOL_STR)
#define UTILITY_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(UTILITY_SYMBOL_DECIMAL,UTILITY_SYMBOL_STR)
#else
#define EOSCTRACT "eosio.token"_n
#define EOSDTCNTRACT "eosdtcntract"_n
#define EOSDTORCLIZE "eosdtorclize"_n
#define ORACLERATES "orarates"_n
#define EOSDTNUTOKEN "eosdtnutoken"_n
#define EOSDTSTTOKEN "eosdtsttoken"_n
#define EOSDTFORUM "eosdtgovernc"_n
#define EOS_SYMBOL ds_symbol(EOS_SYMBOL_STR,EOS_SYMBOL_DECIMAL)
#define EOS_SYMBOL_VALUE EOS_SYMBOL.value()
#define USD_SYMBOL ds_symbol(USD_SYMBOL_STR,USD_SYMBOL_DECIMAL)
#define USD_SYMBOL_VALUE USD_SYMBOL.value()
#define STABLE_SYMBOL ds_symbol(STABLE_SYMBOL_STR,STABLE_SYMBOL_DECIMAL)
#define STABLE_SYMBOL_VALUE STABLE_SYMBOL.value()
#define UTILITY_SYMBOL ds_symbol(UTILITY_SYMBOL_STR,UTILITY_SYMBOL_DECIMAL)
#define UTILITY_SYMBOL_VALUE UTILITY_SYMBOL.value()
#endif
#define EMPTY_SYMBOL (ds_symbol(0ull))


#define NEED_TO_SET_UP "need to set up(liqdatr)"
#define ALREADY_STORED "already stored"
#define DOES_NOT_EXIST "does not exist"
#define AS_AN_ACCOUNT "as an account"
#define DELETE "delete"
#define STARTED "started"
#define FINISHED "finished"


#define PARAMETERS "parameters"
struct liqparameter {
    ds_ulong parameter_id;
    ds_asset surplus_debt;
    ds_asset bad_debt;
    ds_asset eos_balance;
    ds_asset nut_collat_balance;

    ds_ulong primary_key() const { return parameter_id; }
};

#ifdef COMMON
FC_REFLECT(liqparameter, (parameter_id)
(surplus_debt)(bad_debt)(eos_balance)(nut_collat_balance));
#endif

#define SETTINGS "liqsettings"
struct liqsetting {
    ds_ulong setting_id;
    ds_account eosdtcntract_account;
    uint8_t global_unlock;
    ds_asset auction_price;

    ds_ulong primary_key() const { return setting_id; }
};

#ifdef COMMON
FC_REFLECT(liqsetting, (setting_id)(eosdtcntract_account)(global_unlock)(auction_price))
#endif

struct ctrsetting {
    ds_ulong setting_id;
    uint8_t global_lock;
    ds_long time_shift;
    ds_account liquidator_account;
    ds_account oraclize_account;
    ds_account sttoken_account;
    ds_account nutoken_account;
    double governance_fee;
    double stability_fee;
    double critical_ltv;
    double liquidation_penalty;
    double liquidator_discount;
    ds_asset liquidation_price;
    double nut_auct_ratio;
    double nut_discount;
    double profit_factor;
    ds_uint vote_period;
    ds_uint stake_period;
    double reserve_ratio;
    double staking_weight;
    ds_account bpproxy_account;
    ds_account governc_account;

    ds_ulong primary_key() const { return setting_id; }
};

struct oracle_rate
{
    ds_asset rate;
    ds_time update;
    ds_asset provablecb1a_price;
    ds_time provablecb1a_update;
    ds_asset delphioracle_price;
    ds_time delphioracle_update;
    ds_asset equilibriumdsp_price;
    ds_time equilibriumdsp_update;

    uint64_t primary_key() const {

#ifdef COMMON
        return rate.get_symbol().value();
#else
        return rate.symbol.raw();
#endif
    }
};

#ifdef COMMON
FC_REFLECT(oracle_rate, (rate)(update)(provablecb1a_price)(provablecb1a_update)(delphioracle_price)(delphioracle_update)(equilibriumdsp_price)(equilibriumdsp_update));
#endif

struct position {
    ds_ulong position_id;
    ds_account maker;
    ds_asset outstanding;
    ds_asset governance;
    double collateral;

    uint64_t primary_key() const { return position_id; }
    uint64_t get_maker() const { return maker.value; }
};

#ifdef COMMON
FC_REFLECT(position, (position_id) (maker)(outstanding)(governance)(collateral));
#endif

#endif
