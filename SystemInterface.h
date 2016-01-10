#ifndef _ROCKET_SYSTEMINTERFACE_H
#define _ROCKET_SYSTEMINTERFACE_H

#include <Rocket/Core/SystemInterface.h>
#include <Core/Context.h>

/**
	A custom system interface for Rocket. This provides timing information.
 */

namespace Urho3D
{
	namespace Rocket
	{
		namespace Core
		{
			class SystemInterface : public Urho3D::Object, public ::Rocket::Core::SystemInterface
			{
				OBJECT(SystemInterface, Urho3D::Object);

			public:
				SystemInterface(Urho3D::Context* context) :
					Object(context) {};

				static void RegisterObject(Urho3D::Context* context);

				/// Get the number of seconds elapsed since the start of the application
				/// @returns Seconds elapsed
				virtual float GetElapsedTime();

				void AddReference();
				void RemoveReference();
			};
		}
	}
}

#endif
