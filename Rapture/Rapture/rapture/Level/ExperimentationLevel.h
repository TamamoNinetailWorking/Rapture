#pragma once

#include <Bifrost/Level/LevelBase.h>

class CExperimentationLevel : public BIFROST_NAMESPACE::CLevelBase
{
private:

	using Super = CLevelBase;

public:

	bool Initialize() override;
	void Finalize() override;

	void LevelUpdate(float _DeltaTime) override;

};