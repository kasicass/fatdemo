#include "FatFramework.h"

namespace Fat {

void IServerFactory::RegisterMe(EGraphicAPI::EValue api)
{
	theFactorySelector->RegisterFactory(api, this);
}

}
