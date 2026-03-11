# 0002 - Start graphics work with shadow mapping

## Status
Accepted

## Context
Projede renderer mevcut ama render pass yapısı henüz net değil.
İlk büyük rendering feature seçilmeli.
Amaç hem render pipeline bilgisini artırmak hem de portföy değerini yükseltmek.

## Decision
İlk büyük rendering feature olarak shadow mapping ile başlanacak.

## Alternatives considered
- HDR + tonemapping ile başlamak
- SSAO ile başlamak
- Önce tam refactor yapmak

## Rationale
Shadow mapping depth pass, FBO, light-space matrix, bias ve debug ihtiyaçlarını aynı anda zorlar.
Bu yüzden renderer mimarisindeki eksikleri erken görünür kılar.

## Consequences
### Positive
- İlk gerçek render pass eklenecek
- Portföy değeri artacak
- Sonraki feature’lar için temel oluşacak

### Negative
- Mevcut renderer yapısındaki dağınıklık daha görünür olacak
- Küçük bir hazırlık refactor’u gerekecek

## Notes
Bu karar 4 hafta sonra yeniden gözden geçirilebilir.
