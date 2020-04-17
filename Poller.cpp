#include "Poller.h"
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

Poller::Poller(QObject *parent) : QObject(parent)
{
    m_netAccess = new QNetworkAccessManager(this);

    m_pollTimer = new QTimer(this);
    m_pollTimer->setInterval(60 * 1000); // every minute
    connect(m_pollTimer, &QTimer::timeout, this, &Poller::doPoll);

    m_pollTimer->start();

    QSettings settings;
    if (!settings.contains("token")) {
        settings.setValue("token", "setme");
    } else {
        qDebug() << "Have token" << settings.value("token").toString();
    }

//    doPoll();
}

void Poller::doPoll()
{
    if (m_currentRequest) {
        qWarning() << "Already a request, stopping";
        disconnect(m_currentRequest, nullptr, this, nullptr);
        m_currentRequest->abort();
        m_currentRequest->deleteLater();
        m_currentRequest = nullptr;
    }

    QSettings settings;
    const QString token = settings.value("token").toString();
    if (token.isEmpty()) {
        qWarning() << "No token";
        return;
    }

    QUrlQuery query;
    query.addQueryItem("statuses[]", "triggered");
    query.addQueryItem("limit", "5");

    QUrl url("https://api.pagerduty.com/incidents");
    url.setQuery(query);
    qDebug() << "Requesting" << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", "Token token=" + token.toLatin1());

    request.setRawHeader("Accept", "application/vnd.pagerduty+json;version=2");

    m_currentRequest = m_netAccess->get(request);
    connect(m_currentRequest, &QNetworkReply::finished, this, &Poller::onPollReply);
}

void Poller::onPollReply()
{
    if (!m_currentRequest) {
        qWarning() << "No current request?";
        return;
    }

    const QJsonObject rootObject = QJsonDocument::fromJson(m_currentRequest->readAll()).object();
    qDebug() << "Got reply" << rootObject;
    if (!rootObject.contains("incidents")) {
        qWarning() << "No incident array in object!";
        return;
    }

    const QJsonArray incidencts = rootObject["incidents"].toArray();
    if (incidencts.isEmpty()) {
        qDebug() << "No incidents";
        return;
    }

    const QJsonObject object = incidencts.first().toObject();

    const QString id = object["incident_key"].toString();
    const QString title = object["title"].toString();
    const QString url = object["service"].toObject()["html_url"].toString();
    QString body = object["description"].toString();

    if (id.isEmpty() || title.isEmpty() || body.isEmpty() || url.isEmpty()) {
        qWarning() << "Missing in object" << id << title << body;
    }
    body += "\n<br/\n><a href='" + url.toHtmlEscaped() +"'>Link</a>";

    emit newEvent(title, body, id);
}
