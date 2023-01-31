#pragma once

EDENS_NAMESPACE_BEGIN

class CObject
{
public:

	bool IsPendingKill() const;
	void SetPendingKill(bool _IsKill);

private:

	bool m_IsPendingKill = false;

};

EDENS_NAMESPACE_END