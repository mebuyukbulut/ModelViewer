#define STB_IMAGE_IMPLEMENTATION
#include "Engine.h"
#include <windows.h>
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
	Engine engine;
	engine.run();
}
// ╔══════════════════════════════════════════════════════════════════════════╗
// ║	NAMING CONVENTION				                                      ║
// ╚══════════════════════════════════════════════════════════════════════════╝    
// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Element			┆ Style				┆ Example						  │
// │┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄│
// │ Class				┆ PascalCase		┆ ModelViewer, ShaderProgram	  │
// │ Function			┆ camelCase()		┆ drawModel(), loadShader()		  │
// │ Member Variable	┆ _camelCase		┆ _camera, _pbrShader			  │
// │ Parameter			┆ camelCase			┆ setCamera(const Camera& camera) │
// │ Local Variable		┆ camelCase			┆ modelMatrix, shaderProgram	  │
// │ Constants			┆ UPPER_CASE		┆ PI, 							  │
// │ Enums				┆ PascalCase		┆ RenderMode::Wireframe			  │
// └──────────────────────────────────────────────────────────────────────────┘
// 
// 