#ifndef IELANCEDATA_H
#define IELANCEDATA_H

namespace FreelanceNavigator
{
class IElanceData
{
public:
    IElanceData();
    virtual ~IElanceData() = 0;

    inline bool isValid() const { return m_isValid; }

    inline void setIsValid(bool isValid) { m_isValid = isValid; }

private:
    bool m_isValid;
};
}

#endif // IELANCEDATA_H
