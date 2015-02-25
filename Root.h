#ifndef _ROOT_H
#define _ROOT_H

#include <string>
#include <memory>

class DebugConsole;
namespace Urho3D
{
	class Engine;
	class Context;
}

class Root
 {
   public:
     static Root* instance();

	 Urho3D::Engine* GetEngine() { return _engine; }
	 Urho3D::Context* GetContext() { return _context; }

	 void Run();

   private:
     Root();
     ~Root();

	 Urho3D::Context* _context;
	 Urho3D::Engine* _engine;
	 bool m_bStop;

	 static bool g_initialized;
     static Root g_instance;    
};

#endif