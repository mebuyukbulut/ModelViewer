# 0011 - Builtin

# Durum
Hard coded stringlerimiz var. Tek bir noktada olsa sıkıntı olmayacak fakat birden fazla noktadalar. Bu da bazı sorunları bereberinde getiriyor.

- Bir yerdeki string'i değiştirince aynı string'in kullanıldığı diğer yerleri de güncellemek lazım. 
- Kullanmak istedimde hatırlamakta güçlük çekiyorum. 


# Karar
Namespace içinde tutacağız. İç içe namespace'ler içinde string şeklinde. İhtiyacımız olduğunda Builtin::Material::DefaultMaterial vb şekillerde kullanacağız.

Neden Builtin ismini kullandık? Bu stringler engine içerisinde gömülü olan şekil, material, shader gibi built-in şeyleri isimlendirmek için kullanılmaya başlandı. 

# Alternatifler
- Namespace yerine bir sınıf oluşturup sınıftan bunları çekebilirdik. Muhtemelen builder pattern'ı kullanırdık. Bunu kullanmanın kesin bir avantajı veya dezavantajı var mı bilmiyorum. 

- Neden namespace kullandım? Çünkü bu sistem bana daha esnek ve kullanışlı geldi. Kişisel tercihim bu yönde oldu. 


# Çıktılar
## Pozitif 
- Artık string hardcoded değişkenlerimiz tek bir noktaya toplandı. Yönetimi kolaylaştı. Hatalar azaldı. Development hızlandı. 

## Negatif
- Bazı yerlerdeki enumları kaldırdım. Belki de artık kodumuz daha az "typesafe" 


## Şuan ki durum 


    C++
    namespace Builtin
    {
    namespace Material{
        inline constexpr const char* DefaultMaterial = "builtin::materials::defaultMaterial";
    }
    namespace Model
    {
        inline constexpr const char* Cube        = "builtin::models::cube";
        inline constexpr const char* Cone        = "builtin::models::cone";
        inline constexpr const char* Cylinder    = "builtin::models::cylinder";
        inline constexpr const char* Sphere      = "builtin::models::sphere";
        inline constexpr const char* Plane       = "builtin::models::plane";
        inline constexpr const char* Torus       = "builtin::models::torus";

        inline constexpr const char* BgPlane     = "builtin::models::bgPlane";
        inline constexpr const char* GridPlane   = "builtin::models::gridPlane";

        inline constexpr const char* LightArrow  = "builtin::models::lightArrow";
        inline constexpr const char* LightCone   = "builtin::models::lightCone";
        inline constexpr const char* LightSphere = "builtin::models::lightSphere";

        inline constexpr const char* All[] = {
            Cube,       
            Cone,       
            Cylinder,   
            Sphere,
            Plane,      
            Torus,      
            BgPlane,    
            GridPlane,  
            LightArrow, 
            LightCone,  
            LightSphere,
        };
    }
    // ...
    }
