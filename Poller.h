#ifndef POLLER_H
#define POLLER_H

#include <QObject>
#include <QPointer>

class QNetworkAccessManager;
class QTimer;
class QNetworkReply;

class Poller : public QObject
{
    Q_OBJECT
public:
    explicit Poller(QObject *parent = nullptr);

signals:
    void newEvent(const QString &title, const QString &body, const QString &id);

private slots:
    void doPoll();
    void onPollReply();

private:
    QNetworkAccessManager *m_netAccess;
    QTimer *m_pollTimer;
    QPointer<QNetworkReply> m_currentRequest;
};

#endif // POLLER_H
