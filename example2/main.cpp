/* Echo client using SocketReactor */

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/SocketConnector.h>

const Poco::UInt16 PORT = 32452;

class Session
{
private:
    Poco::Net::StreamSocket _socket;
    string _peerAddress;
    Poco::Net::SocketReactor &_reactor;

    int _sendCount = 1;

public:
    Session(Poco::Net::StreamSocket &socket, Poco::Net::SocketReactor &reactor) : _socket(socket), _reactor(reactor)
    {
        cout << "Connection success" << endl;

        _reactor.addEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

        SendMessage();
    }

    ~Session()
    {
        try
        {
            cout << "~Session" << endl;

            _reactor.removeEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

            _reactor.stop();
        }
        catch (Poco::Exception &ex)
        {
            std::cerr << ex.displayText() << endl;
        }
    }

    void onReadable(Poco::Net::ReadableNotification *pNf)
    {
        pNf->release();

        char buffer[256] = {
            0,
        };
        int n = _socket.receiveBytes(buffer, sizeof(buffer));

        if (n > 0)
        {
            cout << "Message from the server: \"" << buffer << '\"' << endl;

            if (_sendCount < 7)
            {
                ++_sendCount;
                SendMessage();
            }
            else
            {
                cout << "Disconnecting with the server" << endl;

                _socket.close();

                // By calling close(), onReadable() will not be called anymore.
                // So this object can be deleted here.
                delete this;
            }
        }
        else
        {
            cout << "OnReadable disconnected..." << endl;
            delete this;
        }
    }

    void SendMessage()
    {
        char szMessage[256] = {
            0,
        };
        sprintf(szMessage, "Hello server! %d", _sendCount);
        int nMsgLen = (int)strlen(szMessage);
        _socket.sendBytes(szMessage, nMsgLen);

        cout << "Sent \"" << szMessage << '\"' << endl;
    }
};

int main()
{
    Poco::Net::SocketReactor reactor;
    Poco::Net::SocketAddress socketAddress("127.0.0.1", PORT);
    Poco::Net::SocketConnector<Session> connector(socketAddress, reactor);

    cout << "Connecting to " << socketAddress.toString() << endl;
    reactor.run();

    cout << "Done" << endl;
    // getchar();

    return 0;
}
