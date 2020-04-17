#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>

class QLabel;
class QSystemTrayIcon;

class Notification : public QWidget
{
    Q_OBJECT

public:
    Notification(QWidget *parent = nullptr);
    ~Notification();

signals:
    void dismissed(const QString &id);

public slots:
    void onNewEvent(const QString &title, const QString &body, const QString &id);
    void onNoEvents();

private slots:
    void onDismiss();

private:
    QString m_id;
    QLabel *m_body;
    QLabel *m_title;
    QSystemTrayIcon *m_trayIcon;
};
#endif // NOTIFICATION_H
