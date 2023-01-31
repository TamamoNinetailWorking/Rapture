#pragma once

#include <vector>

BIFROST_NAMESPACE_BEGIN

class CRenderingComponent;

struct FRenderingQueueInitializer;

class CRenderingQueue
{
public:

	//using RenderData = const CRenderingComponent*;

	bool Initialize(const FRenderingQueueInitializer* _Initializer);
	void Finalize();

	void SetRenderComponent(const CRenderingComponent* _Component);

	void Clear();

	const CRenderingComponent* GetRenderData(uint32 _Index) const;
	uint32 GetQueueNum() const;

	CRenderingQueue() {};
	~CRenderingQueue() {};

private:

	using Queue = std::vector<const CRenderingComponent*>;

	Queue m_Queue = {};

};

BIFROST_NAMESPACE_END