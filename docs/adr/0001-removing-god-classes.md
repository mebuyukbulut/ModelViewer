# 0001 - Removing God Classes

## Status
Accepted

## Context
SceneManager currently owns multiple unrelated responsibilities, which makes adding new rendering and editor features harder.

## Decision
We will incrementally reduce SceneManager responsibilities instead of doing a large up-front redesign.

## Alternatives considered
- Non refactoring 

## Rationale
The target architecture is not clear enough yet. A gradual decomposition is safer than forcing an artificial structure too early.

## Consequences
### Positive
- Making future changes has become easier.

### Negative
- We will have to wait a while to see a tangible result.

## Notes
-