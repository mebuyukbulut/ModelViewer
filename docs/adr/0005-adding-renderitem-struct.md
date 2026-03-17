# 0005 - Adding RenderItem Struct

## Status
Accepted 

## Context
SceneManage sınıfı ile Renderer sınıfı arasındaki iletişimi Entity objeleriyle sağlıyorduk. Yani gerekli olan entity'ler SceneManager'dan -> Renderer sınıfına atılıyordu. Fakat bu iki sınıf arasındaki bağlantıyı güçlendiriyordu. Renderer gerekli olmadığı halde Entity sınıfı hakkında bilgi sahibiydi. 

Buradaki bu bağlantıyı koparıp sınıfları daha iyi tanımlı hale getirebilmek ve iletişimin sağlanması için bir ara obje olaraka RenderItem struct'ını yaptık. 

SceneManager -- RenderItem's --> Renderer

RenderItem struct'ı şuanlık şunlardan oluşmakla beraber ilerde tanımı ve görevi genişleyebilir:

struct RenderItem
{
	Model* model{};
	glm::mat4 transform{ 1.0f };
	uint32_t entityIndex{}; // for selection pass
};


## Decision
SceneManager ve Renderer sınıfları arasındaki iletişimin sağlanabilmesi için RenderItem isimli struct oluşturuldu. 

## Alternatives considered
- Aynı şekilde bırakabilirdik. Kısa vadeli bir sorun çıkarmasa da ilerleyen dönemde özellikle render bölümüne yeni featureların gelmesiyle kuvvetle muhtemel sıkıntı çıkaracaktı. 
- model ve transformu parametre olarak alacak bir Renderer fonksiyonu yazabilirdik fakat bu sefer de scenemanager ile renderer arasındaki bağlantı daha güçlü olacak. Kodumuzun karmaşıklığı artacaktı. 

## Rationale
-

## Consequences
### Positive
- İki sınıf artık birbirinden daha bağımsız
- SceneManager::draw() fonksiyonu daha sade ve anlaşılabilir bir hal aldı

### Negative
- Biraz vaktimizi aldı. 
- Ara katman beraberinde ek bir işlem yükünü de getiriyor. Optimizasyon yaparken bu durumu göz önünde bulundurmamız gerekecek. 

## Notes
-
