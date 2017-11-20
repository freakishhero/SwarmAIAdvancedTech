#include "Application.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<Application> application;

	//Create a pointer to the application
	application = std::make_unique<Application>();
	if (!system)
	{
		return 0;
	}

	//Initialize the application
	if (application->Initialize())
	{
		//Run the application if its initialised
		application->Run();
	}
	else 
	{
		//Throw an error if the window fails
		MessageBox(0, L"Window Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	//Shutdown the application and release the pointer.
	application->Shutdown();
	application = nullptr;
	return 0;
}

