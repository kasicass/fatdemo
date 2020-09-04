#include "FatFramework.h"

namespace Fat {

IServerFactory::IServerFactory(EGraphicAPI::EValue api)
{
	theFactorySelector->RegisterFactory(api, this);
}

IServerFactory::~IServerFactory()
{
}

}
