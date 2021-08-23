
#include "Renderer.h"
#include "../Application/Application.h"
#include "../Time/Time.h"
#include "../Libs/Sdl.h"

namespace tei::internal::renderer
{

	RendererClass::Service Renderer {};

}

using namespace tei::internal::renderer;
using namespace tei::internal::resources;
using namespace tei::internal::utility;
using namespace tei::internal;

#define m_SDLRenderer RefAs<SDL_Renderer>(m_RenderTarget.pData)

RendererClass::RendererClass(application::Window const& window)
{
	METRICS_TIMEBLOCK;

	m_SDLRenderer = SDL_CreateRenderer(
		static_cast<SDL_Window*>(window.pData),
		-1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC
	);
	if (m_SDLRenderer == nullptr)
		throw utility::TeiRuntimeError{ "Could not load SDL Renerer" };

	SDL_GetRendererOutputSize(m_SDLRenderer, &m_RenderTarget.w, &m_RenderTarget.h);
}

RendererClass::~RendererClass()
{
	METRICS_TIMEBLOCK;

	if (m_SDLRenderer != nullptr) SDL_DestroyRenderer(m_SDLRenderer);
}

void RendererClass::Clear()
{
	METRICS_TIMEBLOCK;
	SDL_RenderClear(m_SDLRenderer);
}

void RendererClass::Update()
{
	METRICS_TIMEBLOCK;

	SDL_GetRendererOutputSize(m_SDLRenderer, &m_RenderTarget.w, &m_RenderTarget.h);
	m_TargetSize.x = float(m_RenderTarget.w);
	m_TargetSize.y = float(m_RenderTarget.h);
	m_TargetCenter = m_TargetSize / 2.f;
	m_TargetScale.x = std::min(m_TargetSize.x, m_TargetSize.y);
	m_TargetRatio = m_TargetSize / m_TargetScale;

	m_Time = time::Time->frame.now.time_since_epoch().count();
}

void RendererClass::Present()
{
	METRICS_TIMEBLOCK;
	SDL_RenderPresent(m_SDLRenderer);
}

void RendererClass::DrawTexture(resources::Texture const& texture, units::Transform const& transform, std::optional<units::Rectangle> source) const
{
	METRICS_TIMEBLOCK;

	auto const sourceRatio{ source ? source.value()[1] : units::Scale{ 1, 1 } };
	auto const texRatio{ sourceRatio * units::Vec2{ texture.w, texture.h } / float(std::min(texture.w, texture.h)) };
	auto const center = m_TargetCenter + transform.position / 2.f * m_TargetScale;
	auto const scale = transform.scale * m_TargetScale * texRatio;
	auto const position = center - scale / 2.f;
	SDL_Rect const dest{
		.x = int(position.x),
		.y = int(position.y),
		.w = int(scale.x),
		.h = int(scale.y),
	};
	auto src = [&] { 
		return SDL_Rect{
				.x = int(source.value()[0].x * texture.w),
				.y = int(source.value()[0].y * texture.h),
				.w = int(source.value()[1].x * texture.w),
				.h = int(source.value()[1].y * texture.h),
		}; 
	};
	SDL_RenderCopyEx(
		m_SDLRenderer,
		static_cast<SDL_Texture*>(texture.pData),
		source ? TmpData(src()): nullptr,
		&dest,
		glm::degrees(transform.rotation.x),
		TmpData(SDL_Point{
			.x = int(scale.x / 2),
			.y = int(scale.y / 2),
		}),
		SDL_FLIP_NONE
	);
}

void RendererClass::DrawSprite(resources::Sprite const& sprite, units::Transform const& transform) const
{
	auto frame = unsigned((m_Time - sprite.origin) / sprite.step);

	if (sprite.loop == false)
		frame = std::min<unsigned>(frame, sprite.rows * sprite.cols - 1);

	units::Vec2 const div{
		1.f / sprite.cols,
		1.f / sprite.rows
	};
	units::Vec2 const pos{
		frame % sprite.cols,
		frame / sprite.cols % sprite.rows
	};

	DrawTexture(sprite.texture, transform, units::Rectangle{ div * pos, div });
}
