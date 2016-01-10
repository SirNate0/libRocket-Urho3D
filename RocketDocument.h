#ifndef _ROCKET_ROCKETDOCUMENT_H
#define _ROCKET_ROCKETDOCUMENT_H

#pragma once

#include <Rocket/Core/ElementDocument.h>
#include <Scene/Component.h>

namespace Urho3D
{
	namespace Rocket
	{
		namespace Core
		{
			class RocketDocument : public Urho3D::Object, public ::Rocket::Core::ElementDocument
			{
				OBJECT(RocketDocument,Urho3D::Object);
			public:
				RocketDocument(Urho3D::Context* context);
				~RocketDocument();

				static void RegisterObject(Urho3D::Context* context);

				void AddReference();
				void RemoveReference();

			protected:
			private:
			};
		}
	}
}

#endif
