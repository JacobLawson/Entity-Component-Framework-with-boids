#include <framework.h>
#include <learnopengl/include/shader.h>
#include <learnopengl/include/model.h>

//Project Includes
#include <Scene.h>
#include <MainScene.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <ModelComponent.h>

int main(int argc, char* argv[])
{
	Framework* fw = new Framework();
	if (fw != nullptr)
	{
		if (fw->Initialise("Hello library", 480, 320))
		{
			MainScene* mainScene = new MainScene("Main Scene", fw);
			mainScene->Initialise();

			//Main Gameplay Loop
			do
			{
				fw->Update();

				mainScene->Update();
				fw->ClearWindow();
				mainScene->Draw();

				fw->SwapBuffers();

			} while (fw->MainWindowActive());

			fw->Destroy();
		}
		delete fw;
		fw = nullptr;
	}
	return EXIT_SUCCESS;
}