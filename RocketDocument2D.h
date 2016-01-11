#ifndef _ROCKET_ROCKETDOCUMENT2D_H
#define _ROCKET_ROCKETDOCUMENT2D_H

#pragma once

#include <memory>
#include <Rocket/Core/Context.h>
#include "RenderInterface.h"
#include "SystemInterface.h"

#include <Scene/Component.h>

#include "RocketInput.h"

namespace Urho3D
{
	namespace Rocket
	{
		class RocketDocument2D : public Urho3D::Component, public ::Rocket::Core::RenderInterface
		{
			OBJECT(RocketDocument2D, Urho3D::Component);
		public:
			RocketDocument2D(Urho3D::Context* context);
			~RocketDocument2D();

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

		protected:

			void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
			void HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

		private:
			std::vector<SharedPtr<Texture2D>> textures;
//			std::vector<Texture2D*> textures;
//			Vector<Texture2D*> textures;
			IntRect scissors;
			bool scissor;

			SharedPtr<ShaderVariation> noTextureVS;
			SharedPtr<ShaderVariation> noTexturePS;
			SharedPtr<ShaderVariation> diffTextureVS;
			SharedPtr<ShaderVariation> diffTexturePS;

			std::shared_ptr<::Rocket::Core::Context> _rocketContext;
			SharedPtr<RocketInput> input;
		};
	}
}

#endif
