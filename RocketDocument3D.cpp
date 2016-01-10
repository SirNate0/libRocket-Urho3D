#include "RocketDocument3D.h"

#include <Rocket/Core.h>
#include <Core/Context.h>
#include <Core/CoreEvents.h>
#include <Graphics/Geometry.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsEvents.h>
#include <Graphics/Material.h>
//#include <Graphics/MaterialCache2D>
#include <Scene/Node.h>
#include <Resource/ResourceCache.h>
#include <Scene/Scene.h>
#include <Graphics/Technique.h>
#include <Resource/XMLFile.h>


namespace Urho3D
{
	namespace Rocket
	{
		

		RocketDocument3D::RocketDocument3D(Urho3D::Context* context) :
			Drawable(context, Urho3D::DRAWABLE_GEOMETRY)
		{
			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();

			zorder = 0.f;

			noTextureVS = graphics->GetShader(Urho3D::VS, "Basic", "VERTEXCOLOR");
			noTexturePS = graphics->GetShader(Urho3D::PS, "Basic", "VERTEXCOLOR");

			diffTextureVS = graphics->GetShader(Urho3D::VS, "Basic", "DIFFMAP VERTEXCOLOR");
			diffTexturePS = graphics->GetShader(Urho3D::PS, "Basic", "DIFFMAP VERTEXCOLOR");

			//Urho3D::Material* mat = new Urho3D::Material(context);
			

			_rocketContext = std::shared_ptr<::Rocket::Core::Context>(
				::Rocket::Core::CreateContext("main2",
				::Rocket::Core::Vector2i(graphics->GetWidth(), graphics->GetHeight()),
				this));
			input = new RocketInput(Urho3D::Object::GetContext(), _rocketContext.get());

			::Rocket::Core::FontDatabase::LoadFontFace("Data/Fonts/Delicious-Bold.otf");
			::Rocket::Core::FontDatabase::LoadFontFace("Data/Fonts/Delicious-BoldItalic.otf");
			::Rocket::Core::FontDatabase::LoadFontFace("Data/Fonts/Delicious-Italic.otf");
			::Rocket::Core::FontDatabase::LoadFontFace("Data/Fonts/Delicious-Roman.otf");

			::Rocket::Core::ElementDocument *Document = _rocketContext->LoadDocument("Data/UI/rocketDemo.html");
//			::Rocket::Core::ElementDocument *Document = _rocketContext->LoadDocument("Data/UI/demo.rml");

			if (Document)
			{
				Document->Show();
			};

			SubscribeToEvent(Urho3D::E_UPDATE, HANDLER(RocketDocument3D, HandleUpdate));
			SubscribeToEvent(Urho3D::E_ENDRENDERING, HANDLER(RocketDocument3D, HandlePostRender));
		}

		RocketDocument3D::~RocketDocument3D()
		{
//			_rocketContext->RemoveReference();
//			_rocketContext.reset();
		}

		void RocketDocument3D::RegisterObject(Urho3D::Context* context)
		{
			context->RegisterFactory<RocketDocument3D>();
		}

		void RocketDocument3D::AddReference()
		{
			AddRef();
		}

		void RocketDocument3D::RemoveReference()
		{
			ReleaseRef();
		}

		// Called by Rocket when it wants to render geometry that it does not wish to optimise.
		void RocketDocument3D::RenderGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const ::Rocket::Core::TextureHandle texture, const ::Rocket::Core::Vector2f& translation)
		{
			MarkForUpdate();
		}

		// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
		::Rocket::Core::CompiledGeometryHandle RocketDocument3D::CompileGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const ::Rocket::Core::TextureHandle texture)
		{
			RocketBatchInfo* batchInfo = new RocketBatchInfo();
			batchInfo->position = Urho3D::Vector3::ZERO;
			batchInfo->batch = 0;

			// Populate texture
			batchInfo->texture = 0;
			if (texture)
			{
				batchInfo->texture = textures.at(texture - 1);
			}

			// Populate vertices
			for (int i = 0; i < num_vertices; ++i) {
				RocketVertexInfo info;
				info.position = Urho3D::Vector3(vertices[i].position.x, vertices[i].position.y, 0.f);
				info.color = Color(vertices[i].colour.red / 255.f, vertices[i].colour.green / 255.f, vertices[i].colour.blue / 255.f, vertices[i].colour.alpha / 255.f);

				if (texture) {
					info.textureCoordinates = Urho3D::Vector2(vertices[i].tex_coord.x, vertices[i].tex_coord.y);
				}

				batchInfo->vertices.Push(info);
			}

			// Populate indices
			for (int i = 0; i < num_indices; i+=3)
			{
				Urho3D::Vector3 info(indices[i], indices[i+1], indices[i+2]);
				batchInfo->indicies.Push(info);
			}	

			RecalculateBatch(*batchInfo);

			return (::Rocket::Core::CompiledGeometryHandle) batchInfo;
		}

		// Called by Rocket when it wants to render application-compiled geometry.		
		void RocketDocument3D::RenderCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry, const ::Rocket::Core::Vector2f& translation)
		{
			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();

			RocketBatchInfo* batchInfo = (RocketBatchInfo*)geometry;

			if (batchInfo->position.x_ != translation.x || batchInfo->position.y_ != translation.y)
			{
				batchInfo->position = Urho3D::Vector3(translation.x, translation.y, zorder);

				RecalculateBatch(*batchInfo);
				MarkForUpdate();

			}
			
			batches_.Push(*batchInfo->batch);
		}

		// Called by Rocket when it wants to release application-compiled geometry.		
		void RocketDocument3D::ReleaseCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry)
		{
			RocketBatchInfo* batch = (RocketBatchInfo*) geometry;
			batch->indicies.Clear();
			batch->texture.Reset();
			batch->vertices.Clear();
		}

		// Called by Rocket when it wants to enable or disable scissoring to clip content.		
		void RocketDocument3D::EnableScissorRegion(bool enable)
		{
			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();

			if (enable)
				graphics->SetScissorTest(true, scissors);
			else
				graphics->SetScissorTest(false);
		}

		// Called by Rocket when it wants to change the scissor region.		
		void RocketDocument3D::SetScissorRegion(int x, int y, int width, int height)
		{
			scissors.left_ = x;
			scissors.top_ = y;
			scissors.bottom_ = height;
			scissors.right_ = width;
		}

		// Called by Rocket when a texture is required by the library.		
		bool RocketDocument3D::LoadTexture(::Rocket::Core::TextureHandle& texture_handle, ::Rocket::Core::Vector2i& texture_dimensions, const ::Rocket::Core::String& source)
		{
			Texture2D* tex = context_->GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Texture2D>(source.CString());
			if (tex) {
				texture_dimensions.x = tex->GetWidth();
				texture_dimensions.y = tex->GetHeight();
			}

			textures.push_back(tex);
			texture_handle = (::Rocket::Core::TextureHandle)textures.size();   // indexing from 1, 0 means no texture

			return true;
		}

		// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
		bool RocketDocument3D::GenerateTexture(::Rocket::Core::TextureHandle& texture_handle, const ::Rocket::Core::byte* source, const ::Rocket::Core::Vector2i& source_dimensions)
		{
			SharedPtr<Image> img = SharedPtr<Image>(new Image(context_));
			img->SetSize(source_dimensions.x, source_dimensions.y, 4);
			img->SetData(source);
			Texture2D* tex = new Texture2D(context_);
			tex->SetData(img);

			textures.push_back(tex);
			texture_handle = (::Rocket::Core::TextureHandle)textures.size();		// indexing from 1, 0 means no texture

			return true;
		}

		// Called by Rocket when a loaded texture is no longer required.		
		void RocketDocument3D::ReleaseTexture(::Rocket::Core::TextureHandle texture_handle)
		{
			Texture2D *tex = textures.at(texture_handle - 1);
			delete tex;
			tex = NULL;
		}

		float RocketDocument3D::GetHorizontalTexelOffset()
		{
#ifdef USE_OPENGL
			return 0.f;
#else
			return 0.5f;
#endif
		}

		float RocketDocument3D::GetVerticalTexelOffset()
		{
#ifdef USE_OPENGL
			return 0.f;
#else
			return 0.5f;
#endif
		}

		void RocketDocument3D::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
		{
			zorder = 0.f;
			batches_.Clear();
			_rocketContext->Update();
			_rocketContext->Render();


			/// Sends a mouse-wheel movement event into this context.
			/// @param[in] wheel_delta The mouse-wheel movement this frame. Rocket treats a negative delta as up movement (away from the user), positive as down.
			/// @param[in] key_modifier_state The state of key modifiers (shift, control, caps-lock, etc) keys; this should be generated by ORing together members of the Input::KeyModifier enumeration.
			/// @return True if the event was not consumed (ie, was prevented from propagating by an element), false if it was.
			bool ProcessMouseWheel(int wheel_delta, int key_modifier_state);
		}

		void RocketDocument3D::HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
		{
			
		}

		void RocketDocument3D::OnWorldBoundingBoxUpdate()
		{
			worldBoundingBox_ = boundingBox_.Transformed(node_->GetWorldTransform());
		}

		void RocketDocument3D::RecalculateBatch(RocketBatchInfo& batchInfo)
		{
			Urho3D::SharedPtr<VertexBuffer> vBuff(new VertexBuffer(context_));
			if (batchInfo.texture)
			{
				vBuff->SetSize(batchInfo.vertices.Size(), MASK_POSITION | MASK_COLOR | MASK_TEXCOORD1, true);
			}
			else
			{
				vBuff->SetSize(batchInfo.vertices.Size(), MASK_POSITION | MASK_COLOR, true);
			}

			Vector2 invScreenSize(1.0f / _rocketContext->GetDimensions().x, 1.0f / _rocketContext->GetDimensions().y);
			float* dest = (float*)vBuff->Lock(0, batchInfo.vertices.Size(), true);
			for (Urho3D::Vector<RocketVertexInfo>::Iterator itr = batchInfo.vertices.Begin(); itr != batchInfo.vertices.End(); ++itr)
			{
				*dest++ = ((*itr).position.x_ + batchInfo.position.x_ - (_rocketContext->GetDimensions().x / 2.f)) / 2.f;
				*dest++ = ((*itr).position.y_ + batchInfo.position.y_ - (_rocketContext->GetDimensions().y / 2.f)) / -2.f;
				*dest++ = batchInfo.position.z_;

				*((unsigned*)dest) = (*itr).color.ToUInt(); dest++;

				if (batchInfo.texture) {
					*dest++ = (*itr).textureCoordinates.x_;
					*dest++ = (*itr).textureCoordinates.y_;
				}
			}
			vBuff->Unlock();

			Urho3D::SharedPtr<IndexBuffer> iBuff(new IndexBuffer(context_));
			iBuff->SetSize(batchInfo.indicies.Size() * 3, true);

			// indices
			unsigned* indices_stream = (unsigned*)iBuff->Lock(0, batchInfo.indicies.Size() * 3, true);
			for (Urho3D::Vector<Urho3D::Vector3>::Iterator itr = batchInfo.indicies.Begin(); itr != batchInfo.indicies.End(); ++itr)
			{
				*indices_stream++ = (unsigned int)(*itr).x_;
				*indices_stream++ = (unsigned int)(*itr).y_;
				*indices_stream++ = (unsigned int)(*itr).z_;
			}

			iBuff->Unlock();

			if (!batchInfo.batch)
			{
				batchInfo.batch = new Urho3D::SourceBatch();
				batchInfo.batch->geometry_ = new Geometry(context_);
				batchInfo.batch->geometryType_ = GEOM_STATIC;
			}

			if (batchInfo.texture)
			{
				Urho3D::XMLFile* file = GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::XMLFile>("Materials/RocketDocument.xml");
				batchInfo.batch->material_ = new Urho3D::Material(context_);
				batchInfo.batch->material_->Load(file->GetRoot());
				
				batchInfo.batch->material_->SetTexture(TU_DIFFUSE, batchInfo.texture);
				
			}
			else
			{
				batchInfo.batch->material_ = GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Material>("Materials/RocketDocumentNoTexture.xml");
				
			}

			batchInfo.batch->material_->SetCullMode(CULL_NONE);

			batchInfo.batch->geometry_->SetVertexBuffer(0, vBuff);
			batchInfo.batch->geometry_->SetIndexBuffer(iBuff);

			//Draw indexed geometry
			batchInfo.batch->geometry_->SetDrawRange(TRIANGLE_LIST, 0, iBuff->GetIndexCount(), 0, vBuff->GetVertexCount());
		}
	}
}
