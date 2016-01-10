#ifndef _ROCKET_ROCKETDOCUMENT3D_H
#define _ROCKET_ROCKETDOCUMENT3D_H

#pragma once

#include <memory>
#include <Core/Context.h>
#include "RenderInterface.h"
#include "SystemInterface.h"

#include <Graphics/Drawable.h>
#include "RocketInput.h"

namespace Urho3D
{
	namespace Rocket
	{

		struct RocketVertexInfo
		{
			Urho3D::Color color;
			Urho3D::Vector2 textureCoordinates;
			Urho3D::Vector3 position;
		};

		struct RocketBatchInfo
		{
			RocketBatchInfo()
			{
				batch = 0;
			}

			Urho3D::Vector3 position;
			Urho3D::SourceBatch* batch;
			Urho3D::SharedPtr<Urho3D::Texture2D> texture;
			Urho3D::Vector<RocketVertexInfo> vertices;
			Urho3D::Vector<Urho3D::Vector3> indicies;
		};

		class RocketDocument3D : public Urho3D::Drawable, public ::Rocket::Core::RenderInterface
		{
			OBJECT(RocketDocument3D, Urho3D::Drawable);
		public:
			RocketDocument3D(Urho3D::Context* context);
			~RocketDocument3D();

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
			/// Recalculate the world-space bounding box.
			virtual void OnWorldBoundingBoxUpdate();

			void RecalculateBatch(RocketBatchInfo& batchInfo);

			void HandleInput(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
			void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
			void HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

		private:
			float zorder;
			std::vector<Texture2D*> textures;
			IntRect scissors;

			SharedPtr<ShaderVariation> noTextureVS;
			SharedPtr<ShaderVariation> noTexturePS;
			SharedPtr<ShaderVariation> diffTextureVS;
			SharedPtr<ShaderVariation> diffTexturePS;

			std::shared_ptr<::Rocket::Core::Context> _rocketContext;
			SharedPtr<RocketInput> input;
//			::Rocket::Core::Context* _rocketContext;
		};
	}
}

#endif
