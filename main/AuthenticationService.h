#ifndef CPP_OTP_AUTHENTICATIONSERVICE_H
#define CPP_OTP_AUTHENTICATIONSERVICE_H

#include <string>
#include "ProfileDao.h"
#include "RsaTokenDao.h"

class AuthenticationService {

public:
    AuthenticationService(ProfileDao &profileDao, RsaTokenDao &rsaTokenDao);

    bool isValid(const std::string userName, const std::string password);

private:
    ProfileDao &profileDao;
    RsaTokenDao &rsaTokenDao;
};


#endif //CPP_OTP_AUTHENTICATIONSERVICE_H
