/**
 * @file mega/proxy.h
 * @brief Class for manipulating proxy data
 *
 * (c) 2013-2014 by Mega Limited, Auckland, New Zealand
 *
 * This file is part of the MEGA SDK - Client Access Engine.
 *
 * Applications using the MEGA API must present a valid application key
 * and comply with the the rules set forth in the Terms of Service.
 *
 * The MEGA SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @copyright Simplified (2-clause) BSD License.
 *
 * You should have received a copy of the license along with this
 * program.
 */

#ifndef PROXY_H
#define PROXY_H

#include "mega/types.h"
#include <string>

namespace mega {

#define DEFINE_PROXY_TYPES(expander) expander(NONE) expander(AUTO) expander(CUSTOM)

struct MEGA_API Proxy
{
public:
#define IDENTITY(name) name,

    enum ProxyType
    {
        DEFINE_PROXY_TYPES(IDENTITY)
    };

#undef IDENTITY

    Proxy();
    void setProxyType(int newType);
    void setProxyURL(const std::string& newURL);
    void setCredentials(const std::string& newUsername, const std::string& newPassword);
    int getProxyType() const;
    std::string getProxyURL() const;
    bool credentialsNeeded() const;
    std::string getUsername() const;
    std::string getPassword() const;
    static Proxy parseFromURL(const std::string& url);
    bool operator==(const Proxy& other) const;

protected:
    int proxyType;
    std::string proxyURL;
    std::string username;
    std::string password;
};

int proxyTypeFromString(const std::string& type);

const std::string* proxyTypeToString(int type);

// This function retrieves the proxy settings from environment variables
// (http_proxy, HTTP_PROXY, https_proxy or HTTPS_PROXY) in order.
// If none is set, it returns a default proxy.
void getEnvProxy(Proxy* proxy);

} // namespace

#endif // PROXY_H
