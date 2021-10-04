#include "TCPManager.h"
#include<QSharedPointer>

TCPManager::TCPManager(QObject *parent) : QObject(parent)
{

}

TemporaryId TCPManager::addNewTCPSocket(QTcpSocket *socket)
{
    if(nullptr!= socket){
        TemporaryId tempId = TemporaryId(socket);
        _sockets.insert(tempId,QSharedPointer<QTcpSocket>());
        _sockets[tempId].reset(socket);
        connect(socket,&QTcpSocket::aboutToClose,this ,[this, tempId](){
            emit socketDeleted(tempId);
            _sockets.remove(tempId);
        });
        connect(socket,&QTcpSocket::stateChanged,
                this , [this, tempId](QAbstractSocket::SocketState socketState){
            emit stateChanged(tempId, socketState);
        });
        connect(socket,&QTcpSocket::readyRead,this ,[this, tempId](){
            auto ba=_sockets[tempId]->readAll();
            emit newData(tempId , ba);
        });
        return tempId;
    }
    else {
        return std::string::npos;
    }
}

int TCPManager::socketSize() const
{
    return _sockets.keys().size();
}
