#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QMap>
#include <QTcpSocket>
using TemporaryId = size_t;
/// \brief The TCPManager class will manage sockets and delete them automatically when closed
class TCPManager : public QObject
{
    Q_OBJECT
public:
    explicit TCPManager(QObject *parent = nullptr);

    /// \brief addNewTCPSocket to tcpmanager
    /// \param socket targetSocket(Accepted) , note: tcpmanager will be ownership of that
    /// \return return size_t that represent Temporary ID
    /// why temporary? because when it is destroying another sockets fill there with same Id
    /// But it guaranteed that TemporaryID is unique at same time.
    ///
    TemporaryId addNewTCPSocket(QTcpSocket* socket);
    int socketSize() const;
signals:
    void newData(TemporaryId, const QByteArray &);
    void socketDeleted(TemporaryId);
    void stateChanged(TemporaryId , QAbstractSocket::SocketState);
private:
    QMap<size_t,QSharedPointer<QTcpSocket>> _sockets;
};

#endif // TCPMANAGER_H
