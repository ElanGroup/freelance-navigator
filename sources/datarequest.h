#ifndef DATAREQUEST_H
#define DATAREQUEST_H

#include "elanceapirequest.h"

namespace FreelanceNavigator
{
class DataRequest : public ElanceApiRequest
{
    Q_OBJECT
public:
    explicit DataRequest(const QString & accessToken,
                         QNetworkAccessManager * networkManager,
                         QObject * parent);
    virtual ~DataRequest();

    void setAccessToken(const QString & accessToken);

protected:
    virtual void readData(const QByteArray & data) = 0;
    QString accessToken() const;

protected slots:
    void processReply();

private:
    Q_DISABLE_COPY(DataRequest)

    QString m_accessToken;
};
}

#endif // DATAREQUEST_H
