# 0006 Shadermanager Is Redundant

## Status
Accepted 

## Context

AssetManager farklı assetlerin kaynak yönetimini işletiyor. AssetManager sistemini kurduktan sonra ek olarak bir ShaderManager class'ına ihtiyacımız kalmadı. Çünkü ShaderManager da şuanlık sadece shader load/unload etmekten başka bir işe yaramıyordu. 

Uzun lafın kısası Shader sınıfını bir Asset olarak tanımlayıp AssetManager altında yönetebilirdik ki biz de bunu yaptık. 

## Decision
- Shader sınıfının Asset'ten kalıtım almasına 
- ShaderManager sınıfının silinip onun yerine AssetManager sınıfının kullanılmasına 
karar verilmiştir. 

## Alternatives considered
- ShaderManager sınıfını silmek çok hayati bir öncelik değildi. Fakat sistemi olabildiğince temiz tutmaya çalıştığım için bu refactoru yaptım. 

İleride projemize advance featurelar eklemek gerektiğinde belki de yine bir ShadarManager'a ihtiyaç duyabiliriz. Bunu bilemiyorum fakat bunu düşünmek için şimdilik çok erken. 

## Rationale
-

## Consequences
### Positive
- 

### Negative
- 

## Notes
-
