#include "Notification.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QSettings>
#include <QInputDialog>
#include <QSystemTrayIcon>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QOperatingSystemVersion>

Notification::Notification(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::X11BypassWindowManagerHint); // Shows on all desktops, always on top

    setAttribute(Qt::WA_X11NetWmWindowTypeNotification); // For compton or whatever you call it, avoids active check e. g. for darkening

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    m_title = new QLabel("Title");
    m_body = new QLabel("Body");
    m_body->setTextFormat(Qt::RichText);

    QPushButton *dismiss = new QPushButton("Dismiss");
    QPushButton *quit = new QPushButton("Quit");

    QHBoxLayout *quitLayout = new QHBoxLayout;
    quitLayout->addStretch();
    quitLayout->addWidget(quit);
    layout->addLayout(quitLayout);

    layout->addWidget(m_title);
    layout->addWidget(m_body);
    layout->addWidget(dismiss);


    connect(dismiss, &QPushButton::clicked, this, &Notification::onDismiss);
    connect(quit, &QPushButton::clicked, qGuiApp, &QGuiApplication::quit);

    setMinimumSize(100, 100);

    m_trayIcon = new QSystemTrayIcon(QIcon(":/icon.png"), this);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Notification::show);
    m_trayIcon->show();

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
    m_trayIcon->setIcon(QIcon(":/icon-active.png"));
    m_trayIcon->show();
}

void Notification::onNoEvents()
{
    m_trayIcon->setIcon(QIcon(":/icon.png"));
}

void Notification::onDismiss()
{
    emit dismissed(m_id);
    m_id.clear();
    hide();
}

void Notification::enableAutostart()
{
    QFileInfo fileInfo(QCoreApplication::applicationFilePath());

#if defined(Q_OS_WIN)
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + QDir::separator() + "Startup" + QDir::separator();
    QFile::link(fileInfo.canonicalFilePath(), autostartPath());
#elif defined(Q_OS_LINUX)
    QSettings desktopFile(autostartPath());
    desktopFile.beginGroup("Desktop Entry");
    desktopFile.setValue("Exec", fileInfo.canonicalFilePath());
#elif defined(Q_OS_MACOS)
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +"Library/LaunchAgents/";
    QSettings plist(autostartPath(), QSettings::NativeFormat); // TODO check how it encodes QStringList and bools
    plist.setValue("Label", QCoreApplication::applicationName());
    plist.setValue("ProgramArguments", QStringList({fileInfo.canonicalFilePath()}));
    plist.setValue("ProcessType", "Interactive");
    plist.setValue("RunAtLoad", true);
    plist.setValue("KeepAlive", false);
#else
    qWarning() << "Unsupported OS";
#endif
}

void Notification::disableAutostart()
{
    QFile::remove(autostartPath());
}

QString Notification::autostartPath()
{
    QFileInfo fileInfo(QCoreApplication::applicationFilePath());

#if defined(Q_OS_WIN)
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + QDir::separator() + "Startup" + QDir::separator();
    return targetDir + fileInfo.completeBaseName() + ".lnk";
#elif defined(Q_OS_LINUX)
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/autostart/";
    QDir().mkpath(targetDir);
    return targetDir + fileInfo.completeBaseName() + ".desktop";
#elif defined(Q_OS_MACOS)
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "Library/LaunchAgents/";
    return targetDir + fileInfo.completeBaseName() + ".plist";
#else
#error "Unsupported OS";
#endif
}

