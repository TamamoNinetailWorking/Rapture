#include "Test.h"

#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>

USING_BIFROST;

namespace Test
{

	void TestMain()
	{
		auto* mainDevice = CSubsystemServiceLocator::GetRenderingSubsystemEdit()->GetProcessorEdit()->GetDeviceEdit();

		Test::MatrixVectorTestMain();

		Test::FileLoaderTest(mainDevice);
		Test::GraphicsPipelineTest(mainDevice);
		Test::BindFunctionTest();

		//Test::RenderSubsystemTest();
	}

};