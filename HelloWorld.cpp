#include "HelloWorld.h"
#include "RocketDocument2D.h"
#include "RocketDocument3D.h"
#include "SystemInterface.h"

#include <Graphics/Camera.h>
#include <Core/Context.h>
#include <Core/CoreEvents.h>
#include <Engine/Engine.h>
#include <UI/Font.h>
#include <Input/Input.h>
#include <Graphics/Light.h>
#include <Graphics/Material.h>
#include <Graphics/Model.h>
#include <Graphics/Octree.h>
#include <Core/ProcessUtils.h>
#include <Graphics/Renderer.h>
#include <Resource/ResourceCache.h>
#include <Scene/Scene.h>
#include <Graphics/StaticModel.h>
#include <UI/Text.h>
#include <UI/UI.h>
#include <Core/Main.h>
#include <DebugNew.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsEvents.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>
#include <Graphics/Viewport.h>
#include <Graphics/RenderPath.h>
HelloWorld::HelloWorld(Urho3D::Context* context) :
Object(context),
rocketContext()
{
}

void HelloWorld::Start()
{
	Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();

	helloScene_ = new Urho3D::Scene(context_);
	helloScene_->CreateComponent<Urho3D::Octree>();
	
	auto objectNode = helloScene_->CreateChild();
	Urho3D::Node* lightNode = helloScene_->CreateChild();
	Urho3D::Node* cameraNode = helloScene_->CreateChild();
	//Urho3D::StaticModel* object = objectNode->CreateComponent<Urho3D::StaticModel>();
	//object->SetModel(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Model>("Models/Mushroom.mdl"));
	//object->SetMaterial(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Material>("Materials/Mushroom.xml"));
	Urho3D::Light* light = lightNode->CreateComponent<Urho3D::Light>();
	light->SetLightType(Urho3D::LIGHT_DIRECTIONAL);
	lightNode->SetDirection(Urho3D::Vector3(-1.0f, -1.0f, -1.0f));
	Urho3D::Camera* camera = cameraNode->CreateComponent<Urho3D::Camera>();
	camera->SetFarClip(10000.f);
	cameraNode->SetPosition(Urho3D::Vector3(0.0f, 0.f, 0.0f));
	cameraNode->SetRotation(Urho3D::Quaternion(5.f, Urho3D::Vector3(0.f, 1.f, 0.f)));
	Urho3D::Viewport* vp = new Urho3D::Viewport(context_, helloScene_, camera);
	GetSubsystem<Urho3D::Renderer>()->SetViewport(0, vp);
	//CreateText();
	SubscribeToEvents();

	//Urho3D::SharedPtr<Urho3D::RenderPath> effectRenderPath = vp->GetRenderPath()->Clone();
	//effectRenderPath->Append(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::XMLFile>("PostProcess/ColorCorrection.xml"));
	//effectRenderPath->Append(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::XMLFile>("PostProcess/FXAA3.xml"));
	//// Make the bloom mixing parameter more pronounced
	//effectRenderPath->SetShaderParameter("BloomMix", Urho3D::Vector2(0.9f, 0.6f));
	//effectRenderPath->SetEnabled("Bloom", false);
	//effectRenderPath->SetEnabled("FXAA3", false);
	//vp->SetRenderPath(effectRenderPath);

	Urho3D::Node* node = helloScene_->CreateChild();
	//node->SetPosition(Urho3D::Vector3(0.0f, 0.0f, 0.0f));
	//node->SetRotation(Urho3D::Quaternion(0.0f, 0.f, 0.0f));
	//node->CreateComponent<Urho3D::Rocket::RocketDocument2D>();

	// Left
	
	node = helloScene_->CreateChild();
	node->SetPosition(Urho3D::Vector3(0.0f, 0.0f, 250.0f));
	// This will orient the node to "billboard" the document
	node->SetWorldRotation(camera->GetFaceCameraRotation(node->GetPosition(), node->GetRotation(), Urho3D::FC_ROTATE_XYZ));
	//node->SetRotation(Urho3D::Quaternion(30.0f, -60.f, 0.0f));
	node->CreateComponent<Urho3D::Rocket::RocketDocument2D>();
	//node->LookAt(cameraNode->GetPosition());
	//Right
	//node = helloScene_->CreateChild();
	//node->SetPosition(Urho3D::Vector3(100.0f, 0.0f, 0.0f));
	//node->SetRotation(Urho3D::Quaternion(0.0f, 45.f, 0.0f));
	//node->CreateComponent<Urho3D::Rocket::RocketDocument3D>();

	//graphics->SetDefaultTextureFilterMode(Urho3D::FILTER_ANISOTROPIC);
}

void HelloWorld::Stop()
{
	helloScene_->Clear(true,true);
	Rocket::Core::Shutdown();
}

void HelloWorld::CreateObjects()
{
	
}

void HelloWorld::SubscribeToEvents()
{
	SubscribeToEvent(Urho3D::E_UPDATE, HANDLER(HelloWorld, HandleUpdate));
	SubscribeToEvent(Urho3D::E_ENDRENDERING, HANDLER(HelloWorld, HandlePostRender));
}

void HelloWorld::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
	//float timeStep = eventData[Update::P_TIMESTEP].GetFloat();


}

void HelloWorld::HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{

}
