#ifndef _HELLOWORLD_H
#define _HELLOWORLD_H


#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include "Object.h"
#include "Scene.h"

class HelloWorld : public Urho3D::Object
{
	OBJECT(HelloWorld);

	Rocket::Core::Context* rocketContext;
	
public:
	HelloWorld(Urho3D::Context* context);
	void Start();
	void Stop();
	void CreateObjects();
	void SubscribeToEvents();
	void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	Urho3D::SharedPtr<Urho3D::Scene> helloScene_;
};

#endif