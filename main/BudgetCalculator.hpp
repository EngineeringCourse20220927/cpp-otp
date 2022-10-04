#pragma once

#include <cstdint>
#include <stdexcept>
#include "IBudgetDB.hpp"
#include "date.h"

class BudgetCalculator {

public:
    explicit BudgetCalculator(IBudgetDB &budgetDB);

    uint32_t query(const date::year_month_day &start, const date::year_month_day &end);

private:
    IBudgetDB &budgetDB_;
};