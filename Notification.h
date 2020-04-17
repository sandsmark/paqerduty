#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>

class QLabel;

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

private slots:
    void onDismiss();

private:
    QString m_id;
    QLabel *m_body;
    QLabel *m_title;
};
#endif // NOTIFICATION_H
