#pragma once

#include <cstdint>
#include <stdexcept>
#include "IBudgetDB.hpp"
#include "date.h"

class BudgetCalculator {

public:
    BudgetCalculator(IBudgetDB &budgetDB);

    uint32_t query(date::year_month_day const start, date::year_month_day const end);

private:
    struct BudgetRange {
        std::vector<IBudgetDB::Budget>::const_iterator start;
        std::vector<IBudgetDB::Budget>::const_iterator end;
    };

    static uint32_t getDailyAmount(IBudgetDB::Budget const &budget) noexcept;

    static int
    calculateDaysBetween(date::year_month_day const &start, date::year_month_day const &end) noexcept;

    static BudgetRange
    filterBudgetList(std::vector<IBudgetDB::Budget> const &allBudget, date::year_month_day const &start,
                     date::year_month_day const &end) noexcept;

    static date::year_month_day getLastDayOfMonth(date::year_month const &yearMonth) noexcept;

    static date::year_month_day getFirstDayOfMonth(date::year_month const &yearMonth) noexcept;

    IBudgetDB &budgetDB_;
};

class Period {
public:
    Period(date::year_month_day const &start, date::year_month_day const &end) : start_(start), end_(end) {}

    date::year_month_day const &getStart() const noexcept {
        return start_;
    }

    date::year_month_day const &getEnd() const noexcept {
        return end_;
    }

private:
    date::year_month_day const start_;
    date::year_month_day const end_;
};