#include "Root.h"
#include "HelloWorld.h"

#include "RenderInterface.h"
#include "SystemInterface.h"
#include "RocketDocument.h"
#include "RocketDocument2D.h"
#include "RocketDocument3D.h"
#include "RocketEventListener.h"

#include <Input/Input.h>
#include <Core/Context.h>
#include <Engine/Engine.h>
#include <Graphics/Graphics.h>
#include <Resource/ResourceCache.h>

Root Root::g_instance;
bool  Root::g_initialized;

 Root::Root()
 {
	 g_initialized = true;
 }

 Root::~Root()
 {
	 g_initialized = false;
 }

 Root* Root::instance()
 {
	 // if Root does not exist, create it
     return (g_initialized) ? &g_instance : new Root();
 }

 void Root::Run()
 {
	// Init
	 _context = new Urho3D::Context();
	 _engine = new Urho3D::Engine(_context);

	Urho3D::VariantMap engineParameters;
	engineParameters["WindowTitle"] = "B1UE";
    engineParameters["LogName"]     = "B1UE.log";
    engineParameters["FullScreen"]  = false;
    engineParameters["Headless"]    = false;
	//engineParameters["MultiSample"] = 16;
	//engineParameters["TextureAnisotropy"] = 16;
	//engineParameters["WindowResizable"] = true;
	//_engine->SetPauseMinimized(false);

	if (!_engine->Initialize(engineParameters))
    {
        return;
    }
	_context->GetSubsystem<Urho3D::Input>()->SetMouseVisible(true);
//	_context->GetSubsystem<Urho3D::ResourceCache>()->AddResourceDir("GameData");

	Urho3D::Rocket::Core::RenderInterface::RegisterObject(_context);
	Urho3D::Rocket::Core::SystemInterface::RegisterObject(_context);
	Urho3D::Rocket::Core::RocketDocument::RegisterObject(_context);
	Urho3D::Rocket::RocketDocument2D::RegisterObject(_context);
	Urho3D::Rocket::RocketDocument3D::RegisterObject(_context);

	// Initialize Rocket
	_context->RegisterSubsystem(new Urho3D::Rocket::Core::SystemInterface(_context));
	Rocket::Core::SetSystemInterface(_context->GetSubsystem<Urho3D::Rocket::Core::SystemInterface>());

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();
	RocketEventListenerInstancer::Register(_context);

	Urho3D::SharedPtr<HelloWorld> helloWorld(new HelloWorld(_context));
	helloWorld->Start();

	// Main thread
	m_bStop = false;
	while (!m_bStop && !_engine->IsExiting())
	{
		_engine->RunFrame();
	}

	helloWorld->Stop();
 }

 int main()
 {
	 Root::instance()->Run();
 }
