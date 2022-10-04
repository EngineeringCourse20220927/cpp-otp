#include "BudgetCalculator.hpp"
#include <stdexcept>
#include <vector>
#include "IBudgetDB.hpp"
#include "date.h"

BudgetCalculator::BudgetCalculator(IBudgetDB &budgetDB) : budgetDB_(budgetDB) {
}

uint32_t BudgetCalculator::query(date::year_month_day const start, date::year_month_day const end) {
    if (start > end) {
        return 0;
    }

    std::vector<IBudgetDB::Budget> allBudget = budgetDB_.findAll();

    Period period{start, end};
    auto budget = allBudget.begin();
    uint32_t budgetBetween = 0;
    while (budget != allBudget.end()) {
        budgetBetween += budget->getDailyAmount() * period.getOverlappingDayCount(budget->getPeriod());
        budget++;
    }

    return budgetBetween;
}

