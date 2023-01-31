#include <eden/include/Object/Object.h>

EDENS_NAMESPACE_USING;

bool CObject::IsPendingKill() const
{
    return m_IsPendingKill;
}

void CObject::SetPendingKill(bool _IsKill)
{
    m_IsPendingKill = _IsKill;
}
