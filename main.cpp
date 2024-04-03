#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <fmt/core.h>
#include <Poco/Net/DNS.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/NetException.h>

#include <Poco/Logger.h>

void ShowHostEntryItems(const Poco::Net::HostEntry &hostEntry)
{
    cout << Poco::format("HostEntry: %s\n", hostEntry.name()) << endl;

    for (const auto &address : hostEntry.addresses())
    {
        cout << Poco::format("  %s\n", address.toString()) << endl;
    }
}

int main()
{
    fmt::print("Test 1\n");
    cout << "Poco::Net::DNS::hostName()" << endl;
    cout << Poco::format("  %s", Poco::Net::DNS::hostName()) << endl;
    cout << endl;

    const string hostname("www.google.com");
    cout << Poco::format("Poco::Net::DNS::hostByName(%s)", hostname) << endl;
    ShowHostEntryItems(Poco::Net::DNS::hostByName(hostname));
    cout << endl;

    fmt::print("Test 2\n");
    const string hostAddress(Poco::Net::DNS::hostByName(hostname).addresses().begin()->toString());
    const Poco::Net::IPAddress ipAddress(hostAddress);

    cout << Poco::format("Poco::Net::IPAddress(%s)", hostAddress) << endl;
    ShowHostEntryItems(Poco::Net::DNS::hostByAddress(ipAddress));
    cout << endl;

    fmt::print("Test 3\n");
    cout << Poco::format("Poco::Net::DNS::resolve(%s)", hostname) << endl;
    ShowHostEntryItems(Poco::Net::DNS::resolve(hostname));
    cout << endl;

    fmt::print("Test 4\n");
    cout << Poco::format("Poco::Net::DNS::resolve(%s)", hostAddress) << endl;
    ShowHostEntryItems(Poco::Net::DNS::resolve(hostAddress));

    return 0;
}