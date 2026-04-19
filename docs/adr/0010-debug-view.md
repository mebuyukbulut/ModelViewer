# 0010 - Debug View

# Durum 
Directional light'ı implemente ederken. ShadowMap'in durumunu gösteren bir penceremiz yoktu. Bu da işleri zorlaştırıyordu. 

# Karar 
ShadowMap'i ayrı bir imgui penceresinde bastıracaktık. Böylece anlık olarak neyin nasıl işlendiğini görebilecektik. 

Ayrıca ilerde gerektiği durumlarda shadowmap dışında farklı çıktıların gösterilmesi için de kullanılabilir. 


# Çıktılar
## Pozitif
- Artık directional light için ürettiğimiz shadow map i görebiliyoruz. 
- İlerde farklı kullanım alanlarında işimize yarayacak kullanışlı bir araç olacak gibi duruyor. 

## Negatif 
- Şuanlık büyük bir negatif etkisi yok. Sadece biraz daha kod ve bir tık işlem gücü. 
- ShadowMap şuanlık tek kanal olarak kırmızı gösteriyor. ilerde belki bunun siyah-beyaz hale getirilmesi için ek işlemler yapılabilir. 

