# 0004 - Migration to CMake & Linux Port

## Status
Accepted

## Context
Bilgisayarın çökmesiyle gelişen süreçte artık Windows kullanamayacağım. Projeye devam edebilmek adına Linux'a geçtim. 

Fakat proje Visual Studio'nun build sistemine göre yapılandırılmıştı ve Linux ortamında doğrudan derlenemiyordu. Bu nedenle projeyi Linux üzerinde derlenebilir hale getirecek yeni bir build yaklaşımı seçmem gerekiyordu.


## Decision
Projenin build sistemi CMake'e taşındı ve geliştirme Linux ortamında  devam edecek şekilde yeniden yapılandırıldı.


## Alternatives considered
- Windows'ta kalmak: Daha düşük bir donanımla windowsta proje geliştirmem projenin gelişim hızını ciddi anlamda düşürebilirdi. Çünkü projeyle ilgilenmek yerine teknik sıkıntılarla ilgilenmem gerekecekti. 
- Projeyi sonlandırmak: Projeyi tamamiyle sonlandırıp. Sıfırdan temiz bir projeye başlayabilirdim. Fakat hali hazırda belli bir temel ve tecrübe oturduğu için bu temelden devam etmek daha mantıklı geldi.

## Rationale
CMake platformdan bağımsız bir build sistemi sağladığı için hem Linux üzerinde geliştirmeyi mümkün kılıyor hem de gelecekte Windows veya diğer platformlar için tekrar build alınabilmesine imkan tanıyor.


## Consequences
### Positive
- Cross platform geliştirmenin önü açıldı
- Projenin derlenme süresi ciddi anlamda hızlandı 
- Linux ve CMake sistemine aşina oldum. 

### Negative
- Artık eskiden yazdığımız windows'a bağımlı kodları kullanamıyoruz. Refactor etmeliyiz
- Cross platform desteği vermemiz lazım fakat bu da ek iş yükü demektir. 

## Notes

