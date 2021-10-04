#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <TCPManager.h>
class TCPServer : public QObject
{
    struct IpPort
    {
        QString ip;
        quint16 port;
    };

    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

    bool start();
    void stop();

    IpPort ipPort() const;
    void setIpPort(const IpPort &ipPort);

private slots:
    void onNewConnection();

signals:
    void dataRecieved(TemporaryId ,const QByteArray&);
    void newClient(TemporaryId);
    void closed(TemporaryId);
    void connecting(TemporaryId);
    void connected(TemporaryId);
private:
    IpPort _ipPort;
    QScopedPointer<QTcpServer> _tcpServer;
    QScopedPointer<TCPManager> _tcpManager;
};

#endif // TCPSERVER_H
