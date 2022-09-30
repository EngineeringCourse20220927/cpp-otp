#include "gtest/gtest.h"
#include "../main/AuthenticationService.h"

class StubRsaTokenDao : public RsaTokenDao {
public:
    std::string getRandom(std::string userName) override {
        return "000000";
    };
};

class StubProfileDao : public ProfileDao {
public:
    std::string getPassword(std::string userName) override {
        return "91";
    };
};

namespace {

    TEST(AuthenticationService, IsValid) {
        StubProfileDao profileDao;
        StubRsaTokenDao rsaTokenDao;
        AuthenticationService target = AuthenticationService(profileDao, rsaTokenDao);

        bool actual = target.isValid("joey", "91000000");

        ASSERT_TRUE(actual);
    }

}