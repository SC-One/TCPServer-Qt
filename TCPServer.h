#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <TCPManager.h>
class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

    bool start();
    void close();

private slots:
    void onNewConnection();

signals:
    void dataRecieved(TemporaryId ,const QByteArray&);
    void closed(TemporaryId);
    void connecting(TemporaryId);
    void connected(TemporaryId);
private:
    QScopedPointer<QTcpServer> _tcpServer;
    TCPManager _tcpManager;
};

#endif // TCPSERVER_H
