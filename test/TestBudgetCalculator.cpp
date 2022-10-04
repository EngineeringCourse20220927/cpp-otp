
#include "gtest/gtest.h"
#include <stdexcept>
#include "gmock/gmock.h"

#include "BudgetDBMock.hpp"
#include "../main/BudgetCalculator.hpp"
#include "../main/date.h"

TEST(TestBudgetCalculator, testCalculate) {
    BudgetDBMock budgetDBMock;
    BudgetCalculator budgetCalculator(budgetDBMock);

    EXPECT_THROW({
                     budgetCalculator.query(date::year(2020) / date::month(1) / date::day(5),
                                            date::year(2019) / date::month(1) / date::day(5));
                 }, std::runtime_error);

    uint32_t const budgetInSameMonth = budgetCalculator.query(date::year(2019) / date::month(1) / date::day(1),
                                                              date::year(2019) / date::month(1) / date::day(5));

    ASSERT_EQ(budgetInSameMonth, 5);

    uint32_t const budgetInNeighborMonth = budgetCalculator.query(date::year(2019) / date::month(1) / date::day(30),
                                                                  date::year(2019) / date::month(2) / date::day(2));
    ASSERT_EQ(budgetInNeighborMonth, 4);

    uint32_t const budgetOverMonth = budgetCalculator.query(date::year(2019) / date::month(1) / date::day(30),
                                                            date::year(2019) / date::month(4) / date::day(2));
    ASSERT_EQ(budgetOverMonth, 32);

    uint32_t const budgetTooOld = budgetCalculator.query(date::year(2000) / date::month(1) / date::day(30),
                                                         date::year(2000) / date::month(4) / date::day(2));

    ASSERT_EQ(budgetTooOld, 0);

    uint32_t const budgetTooNew = budgetCalculator.query(date::year(3000) / date::month(1) / date::day(30),
                                                         date::year(3000) / date::month(4) / date::day(2));

    ASSERT_EQ(budgetTooNew, 0);

    uint32_t const budgetValidToTooNew = budgetCalculator.query(date::year(2019) / date::month(4) / date::day(29),
                                                                date::year(3000) / date::month(4) / date::day(2));

    ASSERT_EQ(budgetValidToTooNew, 2);

    uint32_t const budgetTooOldToValid = budgetCalculator.query(date::year(2000) / date::month(4) / date::day(29),
                                                                date::year(2019) / date::month(2) / date::day(2));

    ASSERT_EQ(budgetTooOldToValid, 33);
}

class StubBudgetDB : public IBudgetDB {
public:
    MOCK_METHOD0(findAll, std::vector<IBudgetDB::Budget>());
};

using ::testing::Return;

TEST(TestBudgetCalculator, DailyAmount) {
    StubBudgetDB stubBudgetDb;
    BudgetCalculator budgetCalculator(stubBudgetDb);
    ON_CALL(stubBudgetDb, findAll()).WillByDefault(
            Return(std::vector<IBudgetDB::Budget>{IBudgetDB::Budget{2019, 1, 62}}));

    auto amount = budgetCalculator.query(date::year(2019) / date::month(1) / date::day(5),
                                         date::year(2019) / date::month(1) / date::day(9));

    ASSERT_EQ(amount, 5 * 2);
}