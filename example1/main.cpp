/* Echo server using SocketReactor, multithreading */

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
        cout << "Session: peerAddress=" << _peerAddress << endl;

        _reactor.addEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

        // Poco::Thread::sleep(3000);
    }

    ~Session()
    {
        cout << "~Session: peerAddress=" << _peerAddress << endl;

        _reactor.removeEventHandler(_socket, Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

        // Poco::Thread::sleep(3000);
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
            cout << "Message from the session: " << _peerAddress << endl;

            char szSendMessage[256] = {
                0,
            };

            // sprintf(szSendMessage, "Reply: %s", buffer);
            sprintf(szSendMessage, "Hello client!");
            int nMsgLen = (int)strlen(szSendMessage);

            _socket.sendBytes(szSendMessage, nMsgLen);

            // The client will be allocated per thread.
            // If some clients are connected to the different threads,
            // they don't interfere with each other.
            Poco::Thread::sleep(1000);

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
