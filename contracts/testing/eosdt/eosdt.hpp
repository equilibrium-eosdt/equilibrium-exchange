#pragma once

#include "tables.hpp"

namespace eosdt {

    class contract : public eosio::contract {

    public:
        using eosio::contract::contract;

    protected:
        auto orasetting_get() {
            auto ora = (EOSDTCURRENT == EOSDTORCLIZE) ? _self : ctrsetting_get().oraclize_account;
            orasettings_table orasettings(ora, ora.value);
            auto itr = orasettings.find(0);
            ds_assert(itr != orasettings.end(), "% %.", NEED_TO_SET_UP, "orasettings"_n);
            return *itr;
        }

        ctrsetting ctrsetting_get() {
            auto ctr = (EOSDTCURRENT == EOSDTCNTRACT) ? _self : EOSDTCNTRACT;
            if (EOSDTCURRENT == EOSDTLIQDATR) {
                ctr = liqsetting_get().eosdtcntract_account;
            }
            else if (EOSDTCURRENT == EOSDTLOCKUPP) {
                ctr = locsetting_get().eosdtcntract_account;
            }
            ctrsettings_table ctrsettings(ctr, ctr.value);
            auto itr = ctrsettings.find(0);
            ds_assert(itr != ctrsettings.end(), "% %.", NEED_TO_SET_UP, "ctrsettings"_n);
            return *itr;
        }

        liqsetting liqsetting_get() {
            auto liq = (EOSDTCURRENT == EOSDTLIQDATR) ? _self : ctrsetting_get().liquidator_account;
            liqsettings_table liqsettings(liq, liq.value);
            auto itr = liqsettings.find(0);
            ds_assert(itr != liqsettings.end(), "% %.", NEED_TO_SET_UP, "liqsettings"_n);
            return *itr;
        }


        auto govsetting_get() {
            auto gov = (EOSDTCURRENT == EOSDTGOVERNC) ? _self : EOSDTGOVERNC;
            govsettings_table govsettings(gov, gov.value);
            auto itr = govsettings.find(0);
            ds_assert(itr != govsettings.end(), "% %.", NEED_TO_SET_UP, "govsettings"_n);
            return *itr;
        }

        auto ressetting_get() {
            auto res = (EOSDTCURRENT == EOSDTRESERVE) ? _self : EOSDTRESERVE;
            ressettings_table ressettings(res, res.value);
            auto itr = ressettings.find(0);
            ds_assert(itr != ressettings.end(), "% %.", NEED_TO_SET_UP, "ressettings"_n);
            return *itr;
        }

        locsetting locsetting_get() {
            auto res = (EOSDTCURRENT == EOSDTLOCKUPP) ? _self : EOSDTLOCKUPP;
            locsettings_table locsettings(res, res.value);
            auto itr = locsettings.find(0);
            ds_assert(itr != locsettings.end(), "% %.", NEED_TO_SET_UP, "locsettings"_n);
            return *itr;
        }

        auto ctr_by_symbol(const ds_symbol &symbol) {
            if (symbol == STABLE_SYMBOL) {
                return ctrsetting_get().sttoken_account;
            }
            if (symbol == UTILITY_SYMBOL) {
                return ctrsetting_get().nutoken_account;
            }
            return EOSCTRACT;
        }

        auto time_get() {
            auto time = ds_time(eosio::current_time_point().sec_since_epoch());
#ifdef TESTNET
            auto ctr = (EOSDTCURRENT == EOSDTCNTRACT) ? _self : EOSDTCNTRACT;
            if (EOSDTCURRENT == EOSDTLIQDATR) {
                ctr = liqsetting_get().eosdtcntract_account;
            }
            ctrsettings_table ctrsettings(ctr, ctr.value);
            auto itr = ctrsettings.find(0);

            //ds_print("\r\ntime_get: real node time: %", time);
            if (itr == ctrsettings.end()) {
                ds_print("\r\ntime_get: no settings found");
            } else {
                //ds_print("\r\ntime_get: time_shift: %", itr->time_shift);
            }

            return time + (itr == ctrsettings.end() ? 0 : itr->time_shift);
#else
            return time;
#endif
        }

        auto balance_get(const ds_account &account, const ds_symbol &symbol) {
            sysaccounts_table sysaccounts(ctr_by_symbol(symbol), account.value);
            auto itr = sysaccounts.find(symbol.code().raw());
            if (itr == sysaccounts.end()) {
                return ds_asset(0ll, symbol);
            }
            ds_print("\r\n% balance: %", account, itr->balance);
            return itr->balance;
        }

        auto supply_get(const ds_symbol &symbol) {
            auto id = symbol.code().raw();
            sysstat_table sysstats(ctr_by_symbol(symbol), id);
            auto itr = sysstats.find(id);
            if (itr == sysstats.end()) {
                return ds_asset(0ll, symbol);
            }
            return itr->supply;
        }

        auto unissued_supply_get(const ds_symbol &symbol) {
            auto id = symbol.code().raw();
            sysstat_table sysstats(ctr_by_symbol(symbol), id);
            auto itr = sysstats.find(id);
            if (itr == sysstats.end()) {
                return ds_asset(0ll, symbol);
            }
            return itr->max_supply - itr->supply;
        }

#ifdef DEBUG

        void debug_print_balance() {
            auto eos_balance = balance_get(_self, EOS_SYMBOL);
            auto stable_balance = balance_get(_self, STABLE_SYMBOL);
            ds_print("\r\n%(%) % %", EOSDTCURRENT, _self, eos_balance, stable_balance);
        }

#endif

        auto rexpool_available() {
            rexpool_table rexpool(EOSSYSTEM, EOSSYSTEM.value);
            auto itr = rexpool.begin();
            return itr != rexpool.end() && itr->total_rex.amount > 0;
        }

        auto rexpool_get() {
            rexpool_table rexpool(EOSSYSTEM, EOSSYSTEM.value);
            return *rexpool.begin();
        }


        auto rex_to_eos(const ds_asset &from_rex) {
            auto rexpool = rexpool_get();
            const int64_t S0 = rexpool.total_lendable.amount;
            const int64_t R0 = rexpool.total_rex.amount;
            const int64_t p = (uint128_t(from_rex.amount) * S0) / R0;
            auto res = ds_asset(p, EOS_SYMBOL);
            ds_print("\r\nrex_to_eos(%) = from_rex(%) * S0(%) / R0(%)",
                     res, from_rex, rexpool.total_lendable, rexpool.total_rex);
            return res;
        }

        auto eos_to_rex(const ds_asset &from_eos) {
            auto rexpool = rexpool_get();
            const int64_t S0 = rexpool.total_lendable.amount;
            const int64_t R0 = rexpool.total_rex.amount;
            uint128_t fr = uint128_t(from_eos.amount) * R0;
            int64_t p = fr / S0;
            if ((uint128_t(p) * S0) < fr) {
                p++;
            }
            ds_print("\r\neos_to_rex(%) = from_eos(%) * R0(%) / S0(%)",
                     p, from_eos, rexpool.total_rex, rexpool.total_lendable);
            return ds_asset(p, REX_SYMBOL);
        }


        auto rexbalance_get(const ds_account &account) {
            rexbalance_table rexbalances(EOSSYSTEM, EOSSYSTEM.value);
            auto itr = rexbalances.find(account.value);
            if (itr == rexbalances.end()) {
                return ds_asset(0ll, EOS_SYMBOL);
            }
            return rex_to_eos(itr->rex_balance);
        }

        auto rexbalance_pending_get(const ds_account &account) {
            rexqueue_table rexqueue(EOSSYSTEM, EOSSYSTEM.value);
            auto itr = rexqueue.find(account.value);
            if (itr == rexqueue.end()) {
                return ds_asset(0ll, EOS_SYMBOL);
            }
            return rex_to_eos(itr->rex_requested);
        }

        auto orarate_get(const ds_symbol &token_symbol) {
            auto ora = (EOSDTCURRENT == EOSDTORCLIZE) ? _self : ctrsetting_get().oraclize_account;
            orarates_table orarates(ora, ora.value);
            auto itr = orarates.find(token_symbol.raw());
            ds_assert(itr != orarates.end(), "did not find token(%) for orarate.", token_symbol);
            ds_print("\r\nrate:%", itr->rate);
            return itr->rate;
        }

        auto auction_price_get(const ds_symbol &token_symbol) {
            auto liqsettings = liqsetting_get();
            if (liqsettings.global_unlock && token_symbol == USD_SYMBOL) {
                return liqsettings.auction_price;
            }
            return orarate_get(token_symbol);
        }

        auto liquidation_price_get(const ds_symbol &token_symbol) {
            auto ctrsettings = ctrsetting_get();
            if (ctrsettings.global_lock && token_symbol == USD_SYMBOL) {
                return ctrsettings.liquidation_price;
            }
            return orarate_get(token_symbol);
        }

        void issue(const ds_account &to, const ds_asset &quantity, const ds_string &memo) {
            auto ctract = ctr_by_symbol(quantity.symbol);
            auto balance = balance_get(to, quantity.symbol);
            ds_print("\r\nissue: {to: %, quantity: % ,by: %, memo: '%', before: %, after: %}",
                     to, quantity, ctract, memo, balance, balance + quantity);
            if (quantity.amount <= 0) {
                return;
            }
            eosio::action(
                    eosio::permission_level{_self, "active"_n},
                    ctract,
                    "issue"_n,
                    std::make_tuple(_self, quantity, memo)
            ).send();
            if (to != _self) {
                trans(to, quantity, memo);
            }
        }

        void trans(const ds_account &to, const ds_asset &quantity, const ds_string &memo) {
            PRINT_STARTED("trans")
            auto ctract = ctr_by_symbol(quantity.symbol);
            auto balance = balance_get(_self, quantity.symbol);
            ds_print("\r\ntrans: {from: %, to: %, quantity: % ,by: %, memo: '%', before: %, after: %}",
                     _self, to, quantity, ctract, memo, balance, balance - quantity);
            if (quantity.amount <= 0) {
                return;
            }
            eosio::action(
                    eosio::permission_level{_self, "active"_n},
                    ctract,
                    "transfer"_n,
                    std::make_tuple(_self, to, quantity, memo)
            ).send();
            PRINT_FINISHED("trans")
        }

        void receive(const ds_account &from, const ds_asset &quantity, const ds_string &memo) {
            auto ctract = ctr_by_symbol(quantity.symbol);
            auto balance = balance_get(_self, quantity.symbol);
            ds_print("\r\nreceive: {from: %, to: %, quantity: % ,by: %, memo: '%', before: %, after: %}",
                     from, _self, quantity, ctract, memo, balance, balance + quantity);
            if (quantity.amount <= 0) {
                return;
            }
            eosio::action(
                    eosio::permission_level{_self, "active"_n},
                    ctract,
                    "receive"_n,
                    std::make_tuple(from, _self, quantity, memo)
            ).send();
        }

        auto act_by_symbol(const ds_symbol &symbol) {
            if (symbol == STABLE_SYMBOL) {
                return "retire"_n;
            }

            return "burn"_n;
        }

        void burn(const ds_account &from, const ds_asset &quantity, const ds_string &memo) {
            auto ctract = ctr_by_symbol(quantity.symbol);
            auto action = act_by_symbol(quantity.symbol);
            auto balance = balance_get(from, quantity.symbol);
            ds_print("\r\nburn: {from: %, quantity: % ,by: %, memo: '%', before: %,  after: %}",
                     from, quantity, ctract, memo, balance, balance - quantity);
            if (quantity.amount <= 0) {
                return;
            }
            eosio::action(
                    eosio::permission_level{from, "active"_n},
                    ctract,
                    action,
                    std::make_tuple(from, quantity, memo)
            ).send();
        }
    };

}