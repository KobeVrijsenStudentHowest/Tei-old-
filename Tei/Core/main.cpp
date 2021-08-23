
#include "../Application/Application.h"
#include "Core.h"

#include "../Libs/Sdl.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <vld.h>
#endif

int main(int argc, char** argv)
{
	try
	{
		puts("Tei engine - starting");

		METRICS_INITLOG("metrics.json");
		METRICS_TIMEBLOCK;
	
		srand(unsigned(time(nullptr)));
		tei::internal::application::StartApplication(argc, argv);
		tei::internal::core::Core::Run();
		tei::internal::application::StopApplication();

		puts("Tei engine - quit successfully");
		return 0;
	}
	catch (const std::exception& e)
	{
		puts("[Error] Uncaught exception!");
		puts(e.what());
		puts("[Error] Quitting...");
	}
	catch (...)
	{
		puts("[Error] Uncaught exception!");
		puts("[Error] exception Unknown");
		puts("[Error] Quitting...");
	}
	return -1;
}