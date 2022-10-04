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
    IBudgetDB &budgetDB_;
};