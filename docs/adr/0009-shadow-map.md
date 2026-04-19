# 0009 Shadow Maps

# Durum
Nesnelerimiz sahneye çizebiliyoruz. Fakat hangi nesnenin nerede olduğunu tam olarak ayırt edemiyoruz. Çünkü ışık olsa da gölge yok ve gölge olmadığı için nesneler arasındaki mekansal ilişkileri anlamakta zorluk çekiyoruz. 

# Karar
Kalitesizde olsa gölgelendirme yapmamız gerekli. Şuanlık üç temel ışığımız var.
- Directional light
- Point light
- Spot light

# Çıktılar
## Pozitif
- Artık ekranda daha güzel ve anlamlı bir görüntümüz var. 
- Grafik alanında yeni yöntemler öğreniyorum.

## Negatif
- Mecburen daha fazla işlem gücü gerektiriyor.
- DepthMaps için bir yönetim sistemi gerekiyor. N tane ışıkta ne nasıl çalışacak bunlar hesaplanmalı. Textureları bind edilmeli
- Basit olarak görünse de çok fazla detaya sahip olduğu için development hızımıza ket vuruyor .

## Şuanki Durum
Directional light'ı orgin etrafındaki belli bir alan için implemente ettik. Fakat bu geçici bir çözüm. İlerde dinamik olarak kameranın gördüğü alana göre şekillenen bir directional map yapabilirim. Ayrıca Cascade Shadow Map gibi daha advanced teknikleri yazabilirim. 

Directional light için PCF(?) tekniğiyle kenar yumuşatma yaptım. Ana noktanın etrafındaki 4 tane noktayı alıp ortalamasını alıyorum. Çok iyi mi? Hayır. Fakat böyle kalmayacak. 

Directional light için bunları yapmadan önce Spot light ve Point light için shadow desteği getirmeyi düşünüyorum. Bazı sıkıntılarımız var

- Sahnedeki kaç tane ışığı destekleyecek. Yüz tane ışık varsa hepsi için shadow oluşturmalı mıyız? Bence hayır.  
- Desteklenen ışıkların seçimi nasıl yapılacak? Sahnede 100 ışık varsa ve biz sadece 10 ışık shadow map oluşturacaksak bu 10 tane ışığı nasıl seçmemiz gerekli? 
- Point light için CubeMap oluşturmamız lazım. Şuanlık cubemap için bir RenderTarget'ımız yok. Bunun yönetimi nasıl olacak
- Spot light nispeten kolay fakat birden fazla spot light için texture'ların yönetimi nasıl yapılacak. 

- İyi bir performansla kabul edilebilir bir kaliteye getirmek için advanced teknikleri uygulamamız gerekecek fakat bu da apayrı bir dünya. 

