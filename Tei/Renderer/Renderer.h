#pragma once

#include "../Resources/Resources.h"
#include "../Utility/Utility.h"
#include "../Units/Units.h"

namespace tei::internal::application
{
	struct Window;
}

namespace tei::internal::renderer
{

	class RendererClass
	{
	public:

		RendererClass(application::Window const&);
		~RendererClass();

		void Clear();
		void Update();
		void Present();

		auto const& RenderTraget()
		{ return m_RenderTarget; }

		void DrawTexture(resources::Texture const& texture, units::Transform const& transform, std::optional<units::Rectangle> source = std::nullopt) const;
		void DrawSprite(resources::Sprite const& texture, units::Transform const& transform) const;

	private:

		float m_Time{};

		struct RenderTarget
		{
			void* pData;
			int w, h;
		}
		m_RenderTarget{};

		units::Vec2 m_TargetSize{};
		units::Vec2 m_TargetCenter{};
		units::Vec2 m_TargetRatio{};
		units::Vec1 m_TargetScale{};

	public:

		using Service = utility::Service<RendererClass const>;

	};

	extern RendererClass::Service Renderer;

}

namespace tei::external
{

	constexpr static auto const& Renderer{ tei::internal::renderer::Renderer };

}