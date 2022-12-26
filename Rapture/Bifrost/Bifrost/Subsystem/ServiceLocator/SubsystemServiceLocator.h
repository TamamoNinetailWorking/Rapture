#pragma once


//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class CSubsystemDominator;
class IResourceSubsystem;

class CSubsystemServiceLocator
{
public:

	static void SetSubsystemDominator(CSubsystemDominator* _Dominator);

	static CSubsystemDominator* GetSubsystemDominator();

	static IResourceSubsystem* GetResourceSubsystem();

private:

	static ObjectPtr(CSubsystemDominator) s_Dominator;

};


BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END