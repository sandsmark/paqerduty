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

protected:
    void showEvent(QShowEvent *showEvent) override;

private slots:
    void onDismiss();
    void onToggleAutostart();

private:
    void makeVisible(); // updates geometry etc

    void enableAutostart();
    void disableAutostart();
    QString autostartPath();

    QString m_id;
    QLabel *m_body;
    QLabel *m_title;
    QSystemTrayIcon *m_trayIcon;
    QAction *m_autostartAction;
};
#endif // NOTIFICATION_H
