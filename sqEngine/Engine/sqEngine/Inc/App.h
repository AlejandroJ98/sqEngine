#pragma once
namespace sqEngine
{
	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"sqEngine" ;
		uint32_t winWidth = 1280;
		uint32_t winHeight = 720;
		uint32_t debugDrawLimit = 100000;
		std::filesystem::path textureRoot = "../../Assets/Images";
	};

	class App final
	{
	public:
		template <class StateType>
		void AddState(std::string stateName)
		{
			static_assert(std::is_base_of_v<AppState, StateType>, "App -- 'StateType' must be derived from 'AppState'");
			
			auto [iter, success] = mAppStates.try_emplace(std::move(stateName), nullptr);
			if (success)
			{
				std::unique_ptr<AppState>& ptr = iter->second;
				ptr = std::make_unique<StateType>();
				if (mCurrentState == nullptr)
				{
					LOG("App -- Current state: %s", iter->first.c_str());
					mCurrentState = ptr.get();
				}
			}
		}
		void ChangeState(const std::string& stateName);
		
		void Run(AppConfig appConfig);
		void Quit();

	private:
		using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;

		AppStateMap mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
		bool mRunning = false;
	};
}