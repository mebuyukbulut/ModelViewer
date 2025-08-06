#include "Engine.h"



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