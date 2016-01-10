#include "RocketDocument.h"

#include <Core/Context.h>

namespace Urho3D
{
	namespace Rocket
	{
		namespace Core
		{
			RocketDocument::RocketDocument(Urho3D::Context* context) :
				Object(context),
				::Rocket::Core::ElementDocument("")
			{
			}

			RocketDocument::~RocketDocument()
			{

			}

			void RocketDocument::RegisterObject(Urho3D::Context* context)
			{
				context->RegisterFactory<RocketDocument>();
			}

			void RocketDocument::AddReference()
			{
				AddRef();
			}

			void RocketDocument::RemoveReference()
			{
				ReleaseRef();
			}
		}
	}
}
