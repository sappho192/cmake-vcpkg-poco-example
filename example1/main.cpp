#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/ParallelSocketAcceptor.h>

const Poco::UInt16 PORT = 32452;

class Session
{
private:
    Poco::Net::StreamSocket _socket;
    string _peerAddress;
    Poco::Net::SocketReactor &_reactor;

public:
    Session(Poco::Net::StreamSocket &socket, Poco::Net::SocketReactor &reactor) : _socket(socket), _reactor(reactor)
    {
        _peerAddress = socket.peerAddress().toString();
        cout << Poco::format("Session: peerAddress=%s\n", _peerAddress) << endl;

        _reactor.addEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

        // Poco::Thread::sleep(10000);
    }

    ~Session()
    {
        cout << Poco::format("~Session: peerAddress=%s\n", _peerAddress) << endl;

        _reactor.removeEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

        // Poco::Thread::sleep(10000);
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
            cout << Poco::format("Session: peerAddress=%s, %s\n", _peerAddress, buffer) << endl;

            char szSendMessage[256] = {
                0,
            };

            sprintf(szSendMessage, "RE:%s", buffer);
            int nMsgLen = (int)strlen(szSendMessage);

            _socket.sendBytes(szSendMessage, nMsgLen);

            Poco::Thread::sleep(10000);

            cout << "Finished wating onReadable " << buffer << endl;
        }
        else
        {
            _socket.shutdownSend();
            delete this;
        }
    }
};

int main()
{
    Poco::Net::ServerSocket serverSocket(PORT);
    Poco::Net::SocketReactor reactor;
    Poco::Net::ParallelSocketAcceptor<Session, Poco::Net::SocketReactor> acceptor(serverSocket, reactor);

    cout << "Listening on port " << PORT << endl;
    reactor.run();

    return 0;
}
