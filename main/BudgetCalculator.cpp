#include "BudgetCalculator.hpp"
#include <stdexcept>
#include <vector>
#include "IBudgetDB.hpp"
#include "date.h"

BudgetCalculator::BudgetCalculator(IBudgetDB &budgetDB) : budgetDB_(budgetDB) {
}

uint32_t BudgetCalculator::query(const date::year_month_day &start, const date::year_month_day &end) {
    Period period{start, end};
    uint32_t budgetBetween = 0;
    for (const auto &budget: budgetDB_.findAll()) {
        budgetBetween += budget.getOverlappingAmount(period);
    }
    return budgetBetween;
}

