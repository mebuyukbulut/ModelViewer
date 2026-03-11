# Shadow Mapping

## Goal
Add the first real multi-pass rendering feature to the renderer.

## Why
Shadow mapping introduces:
- depth-only rendering
- framebuffer usage
- light-space transforms
- bias management
- debug visualization

## Scope
- Directional light shadows only
- One shadow map
- Debug preview in editor

## Done criteria
- Shadow map is generated
- Shadows appear in scene
- Bias is adjustable
- Debug preview exists

## Open questions
- Where should shadow pass live?
- How should render passes be organized?
- What data should be shared between passes?
