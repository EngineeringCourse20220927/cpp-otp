#include "Birthday.h"

bool Birthday::IsBirthday() {
    return date::year_month_day().month() == date::month{4} && date::year_month_day().day() == date::day{9};
}
