#include "Notification.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QSettings>
#include <QInputDialog>

Notification::Notification(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::X11BypassWindowManagerHint); // Shows on all desktops, always on top

    setAttribute(Qt::WA_X11NetWmWindowTypeNotification); // For compton or whatever you call it, avoids active check e. g. for darkening

    setLayout(new QVBoxLayout);

    m_title = new QLabel("Title");
    m_body = new QLabel("Body");
    m_body->setTextFormat(Qt::RichText);

    QPushButton *dismiss = new QPushButton("Dismiss");

    layout()->addWidget(m_title);
    layout()->addWidget(m_body);
    layout()->addWidget(dismiss);

    connect(dismiss, &QPushButton::clicked, this, &Notification::onDismiss);
    QTimer::singleShot(100, this, [this]() {
        onNewEvent("Test", "adsfasdfasd\nasdfasdfasdf<a href=google.com>test</a>", "1234");
    });

    setMinimumSize(100, 100);

    QSettings settings;
    if (!settings.contains("token")) {
        settings.setValue("token", QInputDialog::getText(nullptr, "Need token", "Please enter an API token"));
    }
}

Notification::~Notification()
{
}

void Notification::onNewEvent(const QString &title, const QString &body, const QString &id)
{
    if (id == m_id && isVisible()) {
        return;
    }

    m_id = id;
    m_title->setText(title);
    m_body->setText(body);
    adjustSize();

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

