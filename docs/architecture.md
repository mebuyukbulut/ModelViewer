# Architecture

## Core modules

### Engine
Owns top-level application lifecycle and main loop.

### Renderer
Responsible for rendering passes, shaders, materials, and frame output.

### Scene
Owns entities, transforms, and scene-level state.

### Editor
Owns viewport UI, hierarchy UI, inspector UI, and selection tools.

### Assets
Owns loading, caching, and GPU upload lifecycle.

### Simulation
Reserved for play mode, scripted motion, and mechanical constraints.

## Current architectural concerns
- SceneManager owns too many responsibilities
- Global access patterns should be reduced
- Render pass structure is still immature
