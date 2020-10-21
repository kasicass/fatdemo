#include "FatFramework/Driver/Server/Interface/IServerFactory.h"
#include "FatFramework/Driver/Client/FactorySelector.h"

namespace Fat {

void IServerFactory::RegisterMe(EGraphicAPI::EValue api)
{
	theFactorySelector->RegisterFactory(api, this);
}

}
