#ifndef _ROCKET_RENDERINTERFACE_H
#define _ROCKET_RENDERINTERFACE_H

#include <vector>
#include <Rocket/Core/RenderInterface.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Math/Rect.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/ShaderVariation.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/VertexBuffer.h>

namespace Urho3D
{
	namespace Rocket
	{
		namespace Core
		{
			struct Urho3DCompiledGeometry
			{
				WeakPtr<Urho3D::IndexBuffer> iBuff;
				SharedPtr<Urho3D::VertexBuffer> vBuff;
				SharedPtr<Texture2D> texture;
			};

			class RenderInterface : public Urho3D::Object, public ::Rocket::Core::RenderInterface
			{
				OBJECT(RenderInterface, Urho3D::Object);
			public:
				RenderInterface(Urho3D::Context* context);
				~RenderInterface();

				static void RegisterObject(Urho3D::Context* context);

				void AddReference();
				void RemoveReference();

				/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
				virtual void RenderGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, ::Rocket::Core::TextureHandle texture, const ::Rocket::Core::Vector2f& translation);

				/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
				virtual ::Rocket::Core::CompiledGeometryHandle CompileGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, ::Rocket::Core::TextureHandle texture);

				/// Called by Rocket when it wants to render application-compiled geometry.
				virtual void RenderCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry, const ::Rocket::Core::Vector2f& translation);
				/// Called by Rocket when it wants to release application-compiled geometry.
				virtual void ReleaseCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry);

				/// Called by Rocket when it wants to enable or disable scissoring to clip content.
				virtual void EnableScissorRegion(bool enable);
				/// Called by Rocket when it wants to change the scissor region.
				virtual void SetScissorRegion(int x, int y, int width, int height);

				/// Called by Rocket when a texture is required by the library.
				virtual bool LoadTexture(::Rocket::Core::TextureHandle& texture_handle, ::Rocket::Core::Vector2i& texture_dimensions, const ::Rocket::Core::String& source);
				/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
				virtual bool GenerateTexture(::Rocket::Core::TextureHandle& texture_handle, const ::Rocket::Core::byte* source, const ::Rocket::Core::Vector2i& source_dimensions);
				/// Called by Rocket when a loaded texture is no longer required.
				virtual void ReleaseTexture(::Rocket::Core::TextureHandle texture_handle);

				/// Returns the native horizontal texel offset for the renderer.
				/// @return The renderer's horizontal texel offset. The default implementation returns 0.
				virtual float GetHorizontalTexelOffset();
				/// Returns the native vertical texel offset for the renderer.
				/// @return The renderer's vertical texel offset. The default implementation returns 0.
				virtual float GetVerticalTexelOffset();

			private:
				std::vector<Texture2D*> textures;
				IntRect scissors;
				Graphics* graphics;

				SharedPtr<ShaderVariation> noTextureVS;
				SharedPtr<ShaderVariation> noTexturePS;
				SharedPtr<ShaderVariation> diffTextureVS;
				SharedPtr<ShaderVariation> diffTexturePS;
			};
		}
	}
}

#endif
