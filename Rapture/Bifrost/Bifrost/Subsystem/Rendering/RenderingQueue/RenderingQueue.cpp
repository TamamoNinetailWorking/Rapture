#include "RenderingQueue.h"
#include "RenderingQueueInitializer.h"

#include <Bifrost/Component/RenderingComponent.h>

USING_BIFROST;

bool CRenderingQueue::Initialize(const FRenderingQueueInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		m_Queue.reserve(_Initializer->m_QueueReserveNum);

		return true;

	} while (0);

	Finalize();
	return false;
}

void CRenderingQueue::Finalize()
{
	m_Queue.clear();
}

void CRenderingQueue::SetRenderComponent(const CRenderingComponent* _Component)
{
	m_Queue.push_back(_Component);
}

void CRenderingQueue::Clear()
{
	m_Queue.resize(0);
}

const CRenderingComponent* CRenderingQueue::GetRenderData(uint32 _Index) const
{
	return m_Queue.at(_Index);
}

uint32 CRenderingQueue::GetQueueNum() const
{
	return SCast<uint32>(m_Queue.size());
}
