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

    BudgetRange budgetRange = filterBudgetList(allBudget, start, end);

    if (budgetRange.end == allBudget.end() || budgetRange.start == allBudget.end()) {
        return 0;
    }

    date::year_month_day const lastDayOfFoundMonth = getLastDayOfMonth(budgetRange.end->budgetMonth_);
    date::year_month_day const firstDayOfFoundMonth = getFirstDayOfMonth(budgetRange.start->budgetMonth_);
    date::year_month_day realEnd{};
    date::year_month_day realStart{};

    if (lastDayOfFoundMonth < end) {
        realEnd = lastDayOfFoundMonth;
    } else {
        realEnd = end;
    }

    if (firstDayOfFoundMonth > start) {
        realStart = firstDayOfFoundMonth;
    } else {
        realStart = start;
    }

    if (budgetRange.start == budgetRange.end) {
        uint32_t const average = (*budgetRange.start).getDailyAmount();

        uint32_t const days = calculateDaysBetweenDate(realStart, realEnd);

        return average * days;
    }

    uint32_t const startMonthAverage = (*budgetRange.start).getDailyAmount();
    date::year_month_day const monthEnd = getLastDayOfMonth(budgetRange.start->budgetMonth_);
    uint32_t const daysInFirstMonth = calculateDaysBetweenDate(realStart, monthEnd);

    uint32_t const budgetFirstMonth = daysInFirstMonth * startMonthAverage;

    std::vector<IBudgetDB::Budget>::const_iterator it = budgetRange.start + 1U;
    uint32_t budgetBetween = 0;
    while (it != budgetRange.end) {
        budgetBetween += it->money_;
        it++;
    }

    uint32_t const endMonthAverage = (*budgetRange.end).getDailyAmount();
    date::year_month_day const monthStart = getFirstDayOfMonth(budgetRange.end->budgetMonth_);
    uint32_t const daysInLastMonth = calculateDaysBetweenDate(monthStart, realEnd);

    uint32_t const budgetEndMonth = daysInLastMonth * endMonthAverage;

    uint32_t const total = budgetFirstMonth + budgetBetween + budgetEndMonth;

    return total;
}

int BudgetCalculator::calculateDaysBetweenDate(date::year_month_day const &start,
                                               date::year_month_day const &end) noexcept {
    auto const dayDiff = date::sys_days(end) - date::sys_days(start);
    return dayDiff.count() + 1;
}

BudgetCalculator::BudgetRange
BudgetCalculator::filterBudgetList(std::vector<IBudgetDB::Budget> const &allBudget, date::year_month_day const &start,
                                   date::year_month_day const &end) noexcept {
    BudgetRange budgetRange;
    date::year_month startMonth = start.year() / start.month();
    date::year_month endMonth = end.year() / end.month();
    budgetRange.start = allBudget.end();
    budgetRange.end = allBudget.end();
    bool foundStart = false;
    std::vector<IBudgetDB::Budget>::const_iterator it = allBudget.begin();
    while (it != allBudget.end()) {
        if ((!foundStart) && (it->budgetMonth_ >= startMonth)) {
            budgetRange.start = it;
            foundStart = true;
        }

        if (it->budgetMonth_ <= endMonth) {
            budgetRange.end = it;
        } else {
            break;
        }
        it++;
    }
    return budgetRange;
}

date::year_month_day BudgetCalculator::getLastDayOfMonth(date::year_month const &yearMonth) noexcept {
    return yearMonth.year() / yearMonth.month() / date::last;
}

date::year_month_day BudgetCalculator::getFirstDayOfMonth(date::year_month const &yearMonth) noexcept {
    return yearMonth.year() / yearMonth.month() / 1;
}