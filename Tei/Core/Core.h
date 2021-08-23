#pragma once

namespace tei::internal::core
{

	class Core
	{

	public:

		static void Run();
		static void Stop();

	private:

		static void GameLoop();
		static void FrameUpdate();

		static bool m_IsRunning;

	};

}