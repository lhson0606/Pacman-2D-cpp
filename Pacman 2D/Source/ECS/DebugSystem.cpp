#include "DebugSystem.h"
#include "DebugPathComponent.h"
#include "dy/Log.h"
#include <glm/gtc/matrix_transform.hpp>

void DebugSystem::Init(std::shared_ptr<SharedData> appState)
{
	this->sharedData = appState;

	//AddTestingPath();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//create large enough buffer to hold all the vertices
	std::vector temp(10000, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), temp.data(), GL_STATIC_DRAW);

	//for drawing target tiles
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void DebugSystem::SetUpImgui(GLFWwindow* window)
{
	this->window = window;

	const char* glsl_version = "#version 130";
	glfwSetErrorCallback(glfw_error_callback);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//#todo: report this!? viewport won't work with render docs, so yeah
	//io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);
#define IMGUI_ENABLE_FREETYPE
}

void DebugSystem::AddTestingPath()
{
	for (int i = 0; i < 4; i++)
	{
		Entity pathEntity = coordinator->CreateEntity();

		std::vector<crushedpixel::Vec2> points{
			{ -0.25f + i * 0.25f, -0.5f  },
			{ -0.25f,  0.5f  },
			{  0.25f,  0.25f },
			{  0.0f,   0.0f  },
			{  0.25f, -0.25f + i * 0.25f },
			{ -0.4f - i * 0.25f,  -0.25f }
		};

		coordinator->AddComponent<DebugPathComponent>(pathEntity, { points, (float)i });
	}
}

void DebugSystem::Draw(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, std::shared_ptr<Texture> tex)
{
	if (!sharedData->IsPathDebugEnabled())
	{
		return;
	}

	PrepareTargetTile(targetTileShader);
	targetTileShader->Use();
	tex->Attach(0);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	targetTileShader->Stop();

	Prepare();

	if (!shouldDraw)
		return;

	pathShader->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
	pathShader->Stop();
}

static bool show_demo_window = false;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void DebugSystem::DrawImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//FPS
	{
		//set the window to the top left corner
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(150, 20));
		ImGui::Begin("Window Name", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("%.1f FPS/%.3f ms", io->Framerate, 1000.0f / io->Framerate);
		ImGui::End();
	}	

	//Input
	{
		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::SetNextWindowSize(ImVec2(250, 150));
		ImGuiIO& io = ImGui::GetIO();

		// Display inputs submitted to ImGuiIO
		ImGui::Begin("Inputs & Focus/Inputs");
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse pos: <INVALID>");
		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

		// We iterate both legacy native range and named ImGuiKey ranges. This is a little unusual/odd but this allows
		// displaying the data for old/new backends.
		// User code should never have to go through such hoops!
		// You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
		ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
#else
		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key >= 0 && key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
		ImGuiKey start_key = (ImGuiKey)0;
#endif
		ImGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); }
		ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
		ImGui::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

		ImGui::End();
	}

	//Debug window
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		ImGui::SetNextWindowPos(ImVec2(w-200, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 400));
		ImGui::Begin("Debug");
		ImGui::Checkbox("Toggle debug path", sharedData->GetPathDebugPtr());

		*sharedData->GetBtnChasePtr() = false;
		*sharedData->GetBtnScatterPtr() = false;

		if (ImGui::Button("Trigger chase mode"))
		{
			*sharedData->GetBtnChasePtr() = true;
		}
		else if (ImGui::Button("Trigger scatter mode"))
		{
			*sharedData->GetBtnScatterPtr() = true;
		}
		else if (ImGui::Button("Trigger frightened mode"))
		{
			*sharedData->GetBtnFrightenedPtr() = true;
		}
		else if (ImGui::Button("Trigger dead mode"))
		{
			*sharedData->GetBtnDeadPtr() = true;
		}

		ImGui::End();
	}

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::EndFrame();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void DebugSystem::SetCoordinator(std::shared_ptr<Coordinator> coordinator)
{
	this->coordinator = coordinator;
}

void DebugSystem::Prepare()
{
	std::vector<float> vertices;

	for (Entity e : entities)
	{
		auto& path = coordinator->GetComponent<DebugPathComponent>(e);
		for (auto& point : path.vertices)
		{
			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(path.id);
		}
	}

	if (vertices.size() == 0)
	{
		vertexCount = 0;
		shouldDraw = false;
		return;
	}

	shouldDraw = true;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//load new data into the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

	if (glGetError() != GL_NO_ERROR)
	{
		DyLogger::LogArg(DyLogger::LOG_ERROR, "OpenGL error: %d", glGetError());
		//assert(false && "Error!");
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	vertexCount = vertices.size() / 3;
}

void DebugSystem::PrepareTargetTile(std::shared_ptr<Shader> shader)
{
	shader->Use();
	for (Entity e : entities)
	{
		auto& debugComponent = coordinator->GetComponent<DebugPathComponent>(e);
		glm::vec3 pos = debugComponent.targetPositions;
		int id = (int)debugComponent.id;
		shader->SetMat4("models[" + std::to_string(id) + "]", glm::translate(glm::mat4(1.0f), pos));
		//shader->SetMat4("models[" + std::to_string(count) + "]", glm::mat4(1.0f));
		assert(id < 4 && "Too many target tiles!");
	}
	shader->Stop();
}

void DebugSystem::LoadTexture(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
{
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->Stop();
}

void DebugSystem::LoadProjectMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 proj)
{
	pathShader->Use();
	pathShader->SetMat4("projection", proj);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetMat4("projection", proj);
	targetTileShader->Stop();
}

void DebugSystem::LoadViewMat(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::mat4 view)
{
	pathShader->Use();
	pathShader->SetMat4("view", view);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetMat4("view", view);
	targetTileShader->Stop();
}

void DebugSystem::LoadColors(std::shared_ptr<Shader> pathShader, std::shared_ptr<Shader> targetTileShader, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3)
{
	pathShader->Use();
	pathShader->SetVec3("colors[0]", color0);
	pathShader->SetVec3("colors[1]", color1);
	pathShader->SetVec3("colors[2]", color2);
	pathShader->SetVec3("colors[3]", color3);
	pathShader->Stop();

	targetTileShader->Use();
	targetTileShader->SetVec3("colors[0]", color0);
	targetTileShader->SetVec3("colors[1]", color1);
	targetTileShader->SetVec3("colors[2]", color2);
	targetTileShader->SetVec3("colors[3]", color3);
	targetTileShader->Stop();
}

void DebugSystem::CleanUp()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

DebugSystem::~DebugSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}