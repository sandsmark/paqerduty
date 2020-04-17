#include "Notification.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

Notification::Notification(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::X11BypassWindowManagerHint); // Shows on all desktops, always on top

    setAttribute(Qt::WA_X11NetWmWindowTypeNotification); // For compton or whatever you call it, avoids active check e. g. for darkening

    setLayout(new QVBoxLayout);

    m_title = new QLabel("Title");
    m_body = new QLabel("Body");

    QPushButton *dismiss = new QPushButton("Dismiss");

    layout()->addWidget(m_title);
    layout()->addWidget(m_body);
    layout()->addWidget(dismiss);

    connect(dismiss, &QPushButton::clicked, this, &Notification::onDismiss);
    QTimer::singleShot(100, this, [this]() {
        onShowEvent("Test", "adsfasdfasd\nasdfasdfasdf<a href=google.com>test</a>", "1234");
    });
}

Notification::~Notification()
{
}

void Notification::onShowEvent(const QString &title, const QString &body, const QString &id)
{
    m_id = id;
    m_title->setText(title);
    m_body->setText(body);

    const QScreen *screen = QGuiApplication::primaryScreen();
    QRect geometry = rect();
    geometry.moveTop(0);

    geometry.moveLeft(screen->size().width() - geometry.width());

    setGeometry(geometry);
    show();
}

void Notification::onDismiss()
{
    emit dismissed(m_id);
    m_id.clear();
    hide();
}

