#include "Driver/Server/Interface/IServerFactory.h"
#include "Driver/Client/FactorySelector.h"

namespace Fat {

void IServerFactory::RegisterMe(EGraphicAPI::EValue api)
{
	theFactorySelector->RegisterFactory(api, this);
}

}
