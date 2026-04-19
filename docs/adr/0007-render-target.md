# 0007 Render Target

# Durum
Çeşitli ihtiyaçlar için (shadow, 3d viewport) frame buffer object management'ı renderer içinde yapıyoruz. Fakat sistem büyüdükçe yeni ihtiyaçlar hasıl oldukça bu yapı yetersiz kalmaktadır. 

# Karar
Framebuffer management işlemini bir sınıf içinde kapsüllemek. Böylece karmaşıklığı azaltmak.


# Çıktılar
## Pozitif 
- Artık eskisine göre framebuffer yönetimi aşırı derecede kolaylaştı

## Negatif
- Sistemin gelecekteki ihtiyaçlarını öngöremediğimiz için yaptığımız abstraction muhtemelen ilerde modifiye edilmeye ihtiyaç duyacaktır. 

## Şuanki Durum
Şuanlık color ve depth için iki tane sınıfımız var. İlerde ihtiyaca göre bunları daha farklı forma 
    class ColorRenderTarget {
        GLuint fbo;
        GLuint colorTex;
        GLuint depthRbo;
        int width, height;

    public:
        void create(int width, int height);
        void destroy();
        bool resize(int width, int height);
        void bind();
        void unbind();

        // Getters
        GLuint framebuffer() const;
        GLuint colorTexture() const;
        GLuint depthBuffer() const;
    };

    class ShadowMapTarget {
        GLuint fbo;
        GLuint depthMap;
        int width, height;

    public:
        void create(int width, int height);
        void destroy();
        bool resize(int width, int height);
        void bind();
        void unbind();

        // Getters
        GLuint framebuffer() const;
        GLuint depthBuffer() const;
    };