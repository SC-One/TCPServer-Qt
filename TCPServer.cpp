#include "TCPServer.h"

#include<QAbstractSocket>

TCPServer::TCPServer(QObject *parent) : QObject(parent),
    _tcpServer(new QTcpServer)
{
    connect(_tcpServer.get(),&QTcpServer::newConnection ,this,&TCPServer::onNewConnection);
    connect(&_tcpManager,&TCPManager::newData,this,&TCPServer::dataRecieved);
    connect(&_tcpManager,&TCPManager::socketDeleted,this,&TCPServer::closed);
    connect(&_tcpManager,&TCPManager::stateChanged,
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
void TCPServer::onNewConnection(){
    auto const& newClient = _tcpServer->nextPendingConnection();
    _tcpManager.addNewTCPSocket(newClient);
}
