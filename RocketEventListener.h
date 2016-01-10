/*
 * RocketEventListener.h
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#pragma once

#include <Core/Object.h>
#include <Rocket/Core/EventListenerInstancer.h>
#include <Rocket/Core/EventListener.h>

using namespace Rocket::Core;

class RocketEventListenerInstancer: public Urho3D::Object, public ::Rocket::Core::EventListenerInstancer//, EventInstancer
{
public:
	OBJECT(RocketEventListenerInstancer, Urho3D::Object);
	virtual ~RocketEventListenerInstancer();
	static void Register(Urho3D::Context* context);

	/// Instance an event listener object.
	/// @param value Value of the event.
	/// @param element Element that triggers the events.
	virtual EventListener* InstanceEventListener(const String& value, Element* element);

	/// Releases this event listener instancer.
	virtual void Release();

	void Execute(EventListener* l, Event& e);

protected:
	RocketEventListenerInstancer(Urho3D::Context* context);
	bool executeAngelScript_;
	bool executeLuaScript_;
	bool sendUrhoEvents_;
};

class RocketEventListener: public ::Rocket::Core::EventListener
{
public:
	OBJECT(RocketEventListener, Urho3D::Object);
	RocketEventListener(const String& context, RocketEventListenerInstancer* parent);
	virtual ~RocketEventListener();
//	static void Register(Urho3D::Context* context);

	/// Process the incoming Event
	virtual void ProcessEvent(Event& event);

	/// Called when the listener has been attached to a new Element
	virtual void OnAttach(Element* element);

	/// Called when the listener has been detached from a Element
	virtual void OnDetach(Element* element);


protected:
	Urho3D::String eventCall_;
	RocketEventListenerInstancer* parent_;
};

