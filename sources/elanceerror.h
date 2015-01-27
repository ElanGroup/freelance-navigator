#ifndef ELANCEERROR_H
#define ELANCEERROR_H

#include "ielanceerror.h"

namespace FreelanceNavigator
{
class ElanceError : public IElanceError
{
public:
    ElanceError();
    ~ElanceError();

    virtual bool isValid() const;
    virtual QString type() const;
    void setType(const QString & type);
    virtual QString code() const;
    void setCode(const QString & code);
    virtual QString description() const;
    void setDescription(const QString & description);

private:
    ElanceError(const ElanceError &);
    ElanceError & operator=(const ElanceError &);

    QString m_type;
    QString m_code;
    QString m_description;
};
}

#endif // ELANCEERROR_H
