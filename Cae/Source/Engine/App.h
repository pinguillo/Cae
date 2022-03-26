#pragma once
#include "Engine.h"
#include "Cae.h"

namespace Cae {
	class App {
	public:
		App();
		virtual ~App();

		void Run();
	private:
		Engine* engine;
	};
}