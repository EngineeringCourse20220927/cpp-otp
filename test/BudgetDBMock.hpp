#pragma once

#include "../main/IBudgetDB.hpp"

class BudgetDBMock : public IBudgetDB {
public:
    BudgetDBMock();

    ~BudgetDBMock() noexcept = default;

    std::vector<IBudgetDB::Budget> findAll() override;

private:
    std::vector<IBudgetDB::Budget> budgetList_;
};