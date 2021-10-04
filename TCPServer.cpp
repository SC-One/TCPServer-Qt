#include "TCPServer.h"

#include<QAbstractSocket>

TCPServer::TCPServer(QObject *parent) : QObject(parent),
    _tcpServer(new QTcpServer)
{
    connect(_tcpServer.get(),&QTcpServer::newConnection ,this,&TCPServer::onNewConnection);
    connect( _tcpManager.get(),&TCPManager::newSocketAdded,this,&TCPServer::newClient);
    connect( _tcpManager.get(),&TCPManager::newData,this,&TCPServer::dataRecieved);
    connect( _tcpManager.get(),&TCPManager::socketDeleted,this,&TCPServer::closed);
    connect( _tcpManager.get(),&TCPManager::stateChanged,
            this, [this](TemporaryId id, const QAbstractSocket::SocketState socketState){
        switch (socketState) {
        case QAbstractSocket::SocketState::ConnectedState:
            emit connected(id);
            break;
        case QAbstractSocket::SocketState::ConnectingState:
            emit connecting(id);
            break;
        default:
            // just eliminate warning!
            break;
        }
    });
}

bool TCPServer::start()
{
    return _tcpServer->listen(QHostAddress(_ipPort.ip) , _ipPort.port);
}

void TCPServer::stop()
{
    _tcpServer->pauseAccepting();
    _tcpManager.reset(nullptr);
    _tcpServer.reset(nullptr);

}
void TCPServer::onNewConnection() {
    auto const& newClient = _tcpServer->nextPendingConnection();
    _tcpManager->addNewTCPSocket(newClient);
}

TCPServer::IpPort TCPServer::ipPort() const
{
    return _ipPort;
}

void TCPServer::setIpPort(const IpPort &ipPort)
{
    _ipPort = ipPort;
    stop();
    start();
}
