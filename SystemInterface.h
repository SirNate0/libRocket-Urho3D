#ifndef _ROCKET_SYSTEMINTERFACE_H
#define _ROCKET_SYSTEMINTERFACE_H

#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Log.h>
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

				/// Log the specified message.
				/// @param[in] type Type of log message, ERROR, WARNING, etc.
				/// @param[in] message Message to log.
				/// @return True to continue execution, false to break into the debugger.
				virtual bool LogMessage(::Rocket::Core::Log::Type type, const String& message);


				void AddReference();
				void RemoveReference();
			};
		}
	}
}

#endif
