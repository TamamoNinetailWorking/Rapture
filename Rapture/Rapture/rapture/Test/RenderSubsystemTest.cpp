#include "Test.h"
#include <Bifrost/Model/Component/PmdModelComponent.h>
#include <Bifrost/Model/Component/PmdModelComponentInitializer.h>

#include <rapture/Environment/PsoNameDefine.h>

USING_BIFROST;

namespace Test
{

	CPmdModelComponent* m_PmdModelComponent = nullptr;

	void RenderSubsystemTest()
	{
		m_PmdModelComponent = new CPmdModelComponent();
		CHECK(m_PmdModelComponent);

		const char* fileName = "resource/mmd/UserFile/Model/‰‰¹ƒ~ƒNmetal.pmd";

		FPmdModelComponentInitializer initializer = {};
		initializer.FileName = CHash160(fileName);
		initializer.ToonMapDirectory = CHash160("resource/mmd/Data/");
		initializer.PsoName = PsoName::PmdRenderingPso;

		if (!m_PmdModelComponent->Initialize(&initializer))
		{
			PRINT("PmdModelComponent is not created.\n");
			return;
		}

		m_PmdModelComponent->BeginPlay();
	}

};