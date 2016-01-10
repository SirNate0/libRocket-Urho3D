#include "SystemInterface.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Timer.h>

namespace Urho3D
{
	namespace Rocket
	{
		namespace Core
		{
			// Get the number of seconds elapsed since the start of the application
			float SystemInterface::GetElapsedTime()
			{
				return context_->GetSubsystem<Time>()->GetElapsedTime();
			}

			void SystemInterface::RegisterObject(Urho3D::Context* context)
			{
				context->RegisterFactory<SystemInterface>();
			}

			void SystemInterface::AddReference()
			{
				AddRef();
			}

			void SystemInterface::RemoveReference()
			{
				ReleaseRef();
			}
		}
	}
}
