#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <locale>
#include <codecvt>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 60;
const float GRAVITY = 800.0f;
const float BOUNCE_DAMPING = 0.7f;

struct FireworkParticle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    sf::Color color;
    float size;
};

struct CoupletCharacter {
    sf::Text text;
    sf::Vector2f targetPosition;
    sf::Vector2f currentPosition;
    sf::Vector2f velocity;
    bool atTarget;
    float bounceFactor;
    int delayFrames;
    int currentDelay;
    float glowPhase;
    float scale;
    float targetScale;
    bool isGlowing;

    // Hiệu ứng rung khi chạm
    float shakeTimer;
    float shakeIntensity;

    // Hiệu ứng xoay
    float rotation;
    float targetRotation;

    CoupletCharacter() :
        atTarget(false),
        bounceFactor(BOUNCE_DAMPING),
        delayFrames(0),
        currentDelay(0),
        glowPhase(0.0f),
        scale(0.1f),
        targetScale(1.0f),
        isGlowing(false),
        shakeTimer(0.0f),
        shakeIntensity(0.0f),
        rotation(0.0f),
        targetRotation(0.0f) {}
};

class VietnameseNewYearFireworks {
private:
    sf::RenderWindow window;
    sf::Font font;

    // Background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture redPaperTexture;
    sf::Sprite leftPaperSprite;
    sf::Sprite rightPaperSprite;

    // Hiệu ứng
    std::vector<FireworkParticle> particles;
    std::vector<CoupletCharacter> leftCouplet;
    std::vector<CoupletCharacter> rightCouplet;

    // Biến thời gian
    sf::Clock clock;
    float totalTime;
    float coupletDropTime;

    // Màu sắc cho hiệu ứng
    std::vector<sf::Color> festiveColors;

    // Hiệu ứng pháo hoa nền
    std::vector<FireworkParticle> backgroundFireworks;

    // Hiệu ứng hoa mai
    std::vector<sf::Sprite> cherryBlossoms;
    sf::Texture cherryBlossomTexture;

    // Cờ đã load font tiếng Việt
    bool fontLoaded;

public:
    VietnameseNewYearFireworks() :
        window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pháo Hoa Tết - Câu Đối SFML", sf::Style::Close),
        totalTime(0.0f),
        coupletDropTime(0.0f),
        fontLoaded(false) {

        srand(static_cast<unsigned>(time(NULL)));
        window.setFramerateLimit(60);
    }

    bool init() {
        // Thử load các font hỗ trợ tiếng Việt
        std::vector<std::string> fontPaths = {
            // Font Windows
            "C:\\Windows\\Fonts\\arial.ttf",
            "C:\\Windows\\Fonts\\times.ttf",
            "C:\\Windows\\Fonts\\verdana.ttf",
            "C:\\Windows\\Fonts\\tahoma.ttf",
            "C:\\Windows\\Fonts\\segoeui.ttf",

            // Font Linux
            "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
            "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",

            // Font macOS
            "/Library/Fonts/Arial.ttf",
            "/System/Library/Fonts/Supplemental/Arial.ttf",

            // Font trong thư mục hiện tại
            "arial.ttf",
            "times.ttf",
            "verdana.ttf",
            "UVNVan.ttf",  // Font tiếng Việt phổ biến
            "vuArial.ttf", // Font VNI
            ".ttf"         // Thử bất kỳ font nào
        };

        for (const auto& path : fontPaths) {
            if (font.loadFromFile(path)) {
                std::cout << "Da tai font thanh cong: " << path << std::endl;
                fontLoaded = true;

                // Kiểm tra font có hỗ trợ tiếng Việt không
                testVietnameseFont();
                break;
            }
        }

        if (!fontLoaded) {
            std::cerr << "Khong the tai font nao! Tao font mac dinh...\n";
            // Vẫn tiếp tục chương trình với font hệ thống mặc định
        }

        // Load background
        if (backgroundTexture.loadFromFile("phongnentet.jpg")) {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setScale(
                static_cast<float>(SCREEN_WIDTH) / backgroundTexture.getSize().x,
                static_cast<float>(SCREEN_HEIGHT) / backgroundTexture.getSize().y
            );
        } else {
            std::cout << "Khong tim thay phongnentet.jpg, tao background gradient...\n";
            createGradientBackground();
        }

        // Load texture giấy đỏ cho câu đối
        if (redPaperTexture.loadFromFile("red_paper.png")) {
            // Tạo giấy đỏ bên trái
            leftPaperSprite.setTexture(redPaperTexture);
            leftPaperSprite.setScale(0.8f, 0.8f);
            leftPaperSprite.setPosition(SCREEN_WIDTH/4 - 150, 100);
            leftPaperSprite.setColor(sf::Color(255, 200, 200, 200));

            // Tạo giấy đỏ bên phải
            rightPaperSprite.setTexture(redPaperTexture);
            rightPaperSprite.setScale(0.8f, 0.8f);
            rightPaperSprite.setPosition(3*SCREEN_WIDTH/4 - 150, 100);
            rightPaperSprite.setColor(sf::Color(255, 200, 200, 200));
        } else {
            std::cout << "Khong tim thay red_paper.png, bo qua hieu ung giay do...\n";
        }

        // Load hoa mai
        if (cherryBlossomTexture.loadFromFile("cherry_blossom.png")) {
            createCherryBlossoms();
        } else {
            std::cout << "Khong tim thay cherry_blossom.png, bo qua hieu ung hoa mai...\n";
        }

        // Khởi tạo màu sắc lễ hội
        festiveColors = {
            sf::Color(255, 215, 0),    // Vàng
            sf::Color(255, 50, 50),    // Đỏ đậm
            sf::Color(255, 140, 0),    // Cam
            sf::Color(255, 100, 100),  // Đỏ nhạt
            sf::Color(255, 200, 0),    // Vàng cam
            sf::Color(255, 255, 200)   // Vàng nhạt
        };

        // Khởi tạo câu đối với tiếng Việt
        initCouplets();

        return true;
    }

    void testVietnameseFont() {
        // Test với một số ký tự tiếng Việt
        sf::Text testText;
        testText.setFont(font);
        testText.setString(L"ÁÂĂĐÊÔƠƯ");
        testText.setCharacterSize(20);

        // Kiểm tra bounding box
        sf::FloatRect bounds = testText.getLocalBounds();
        if (bounds.width > 0) {
            std::cout << "Font co ho tro tieng Viet!\n";
        } else {
            std::cout << "Font co the khong ho tro day du tieng Viet\n";
        }
    }

    std::wstring utf8_to_wstring(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.from_bytes(str);
    }

    void createGradientBackground() {
        // Tạo background gradient đẹp
        sf::Image gradientImage;
        gradientImage.create(SCREEN_WIDTH, SCREEN_HEIGHT, sf::Color(10, 10, 40));

        // Tạo gradient từ đen đến xanh đậm
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            float ratio = static_cast<float>(y) / SCREEN_HEIGHT;
            sf::Color color(
                10 + static_cast<int>(50 * ratio),
                10 + static_cast<int>(30 * ratio),
                40 + static_cast<int>(20 * ratio)
            );

            for (int x = 0; x < SCREEN_WIDTH; x++) {
                // Thêm một ít nhiễu cho background sống động
                if (rand() % 100 < 30) {
                    int noise = rand() % 10 - 5;
                    gradientImage.setPixel(x, y, sf::Color(
                        std::min(255, std::max(0, color.r + noise)),
                        std::min(255, std::max(0, color.g + noise)),
                        std::min(255, std::max(0, color.b + noise))
                    ));
                } else {
                    gradientImage.setPixel(x, y, color);
                }
            }
        }

        backgroundTexture.loadFromImage(gradientImage);
        backgroundSprite.setTexture(backgroundTexture);
    }

    void createCherryBlossoms() {
        // Tạo nhiều hoa mai rơi
        for (int i = 0; i < 50; i++) {
            sf::Sprite blossom;
            blossom.setTexture(cherryBlossomTexture);

            // Random size
            float scale = 0.05f + static_cast<float>(rand() % 15) / 100.0f;
            blossom.setScale(scale, scale);

            // Random position
            blossom.setPosition(
                static_cast<float>(rand() % SCREEN_WIDTH),
                static_cast<float>(-rand() % 200) // Bắt đầu từ trên màn hình
            );

            // Random màu sắc (hồng nhạt)
            int colorVar = rand() % 50;
            blossom.setColor(sf::Color(255, 200 + colorVar, 220 + colorVar, 180 + rand() % 75));

            cherryBlossoms.push_back(blossom);
        }
    }

    void initCouplets() {
        // Câu đối Tết truyền thống - Sử dụng ký tự tiếng Việt chuẩn
        std::vector<std::wstring> leftTexts = {L"NGÀY", L"XUÂN", L"VINH", L"HOA"};
        std::vector<std::wstring> rightTexts = {L"PHÚ", L"QUÝ", L"VỀ"};

        float startX = 1.2 * SCREEN_WIDTH / 4;
        float startY = -100;

        // Khởi tạo câu đối bên trái
        for (size_t i = 0; i < leftTexts.size(); i++) {
            CoupletCharacter ch;

            if (fontLoaded) {
                ch.text.setFont(font);
            }

            // Sử dụng wstring cho tiếng Việt
            ch.text.setString(leftTexts[i]);
            ch.text.setCharacterSize(FONT_SIZE);
            ch.text.setFillColor(festiveColors[0]);
            ch.text.setOutlineColor(sf::Color::Black);
            ch.text.setOutlineThickness(2.0f);
            ch.text.setStyle(sf::Text::Bold);

            // Căn giữa chữ
            sf::FloatRect bounds = ch.text.getLocalBounds();
            ch.text.setOrigin(bounds.width / 2, bounds.height / 2);

            // Vị trí đích
            ch.targetPosition = sf::Vector2f(
                startX,
               150.0f + i * (FONT_SIZE + 30)
            );

            // Vị trí bắt đầu (phía trên màn hình)
            ch.currentPosition = sf::Vector2f(
                startX,
                startY - i * 100
            );

            ch.text.setPosition(ch.currentPosition);

            // Độ trễ cho hiệu ứng rơi lần lượt
            ch.delayFrames = static_cast<int>(i) * 10;
            ch.currentDelay = 0;

            // Thêm hiệu ứng xoay ngẫu nhiên ban đầu
            ch.rotation = static_cast<float>(rand() % 360);
            ch.targetRotation = 0.0f;

            leftCouplet.push_back(ch);
        }

        // Khởi tạo câu đối bên phải
        startX = 2.7 * SCREEN_WIDTH / 4;

        for (size_t i = 0; i < rightTexts.size(); i++) {
            CoupletCharacter ch;

            if (fontLoaded) {
                ch.text.setFont(font);
            }

            ch.text.setString(rightTexts[i]);
            ch.text.setCharacterSize(FONT_SIZE);
            ch.text.setFillColor(festiveColors[1]);
            ch.text.setOutlineColor(sf::Color::Black);
            ch.text.setOutlineThickness(2.0f);
            ch.text.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = ch.text.getLocalBounds();
            ch.text.setOrigin(bounds.width / 2, bounds.height / 2);

            ch.targetPosition = sf::Vector2f(
                startX,
                150.0f + i * (FONT_SIZE + 30)
            );

            ch.currentPosition = sf::Vector2f(
                startX,
                startY - i * 100
            );

            ch.text.setPosition(ch.currentPosition);
            ch.delayFrames = static_cast<int>(i) * 10 + 5; // Lệch pha với bên trái
            ch.currentDelay = 0;

            ch.rotation = static_cast<float>(rand() % 360);
            ch.targetRotation = 0.0f;

            rightCouplet.push_back(ch);
        }
    }

    void createFirework(const sf::Vector2f& position, int particleCount = 100) {
        for (int i = 0; i < particleCount; i++) {
            FireworkParticle p;
            p.position = position;

            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
            float speed = 100.0f + static_cast<float>(rand() % 200);

            p.velocity = sf::Vector2f(
                cos(angle) * speed,
                sin(angle) * speed
            );

            p.lifetime = 1.0f + static_cast<float>(rand() % 100) / 100.0f * 2.0f;
            p.maxLifetime = p.lifetime;

            // Màu ngẫu nhiên từ bảng màu lễ hội
            p.color = festiveColors[rand() % festiveColors.size()];
            p.size = 2.0f + static_cast<float>(rand() % 10) / 10.0f * 3.0f;

            particles.push_back(p);
        }
    }

    void createMiniSparkles(const sf::Vector2f& position, int count = 20) {
        for (int i = 0; i < count; i++) {
            FireworkParticle p;
            p.position = position;

            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
            float speed = 50.0f + static_cast<float>(rand() % 100);

            p.velocity = sf::Vector2f(
                cos(angle) * speed,
                sin(angle) * speed
            );

            p.lifetime = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
            p.maxLifetime = p.lifetime;
            p.color = sf::Color(255, 255, 200, 255); // Màu vàng sáng
            p.size = 1.0f + static_cast<float>(rand() % 5) / 10.0f;

            particles.push_back(p);
        }
    }

    void createBackgroundFirework() {
        if (rand() % 100 < 2) { // 2% chance mỗi frame
            FireworkParticle fw;
            fw.position = sf::Vector2f(
                static_cast<float>(rand() % SCREEN_WIDTH),
                static_cast<float>(SCREEN_HEIGHT + 50)
            );

            fw.velocity = sf::Vector2f(0, -300 - rand() % 200);
            fw.lifetime = 3.0f;
            fw.maxLifetime = 3.0f;
            fw.color = festiveColors[rand() % festiveColors.size()];
            fw.size = 3.0f;

            backgroundFireworks.push_back(fw);
        }
    }

    void updateCherryBlossoms(float deltaTime) {
        for (auto& blossom : cherryBlossoms) {
            // Di chuyển xuống với tốc độ ngẫu nhiên
            float speed = 20.0f + static_cast<float>(rand() % 30);
            blossom.move(0, speed * deltaTime);

            // Xoay nhẹ
            blossom.rotate(10.0f * deltaTime);

            // Di chuyển ngang nhẹ (hiệu ứng bay)
            float swing = sin(totalTime * 2.0f + blossom.getPosition().x * 0.01f) * 0.5f;
            blossom.move(swing, 0);

            // Nếu ra khỏi màn hình, reset lên trên
            if (blossom.getPosition().y > SCREEN_HEIGHT + 50) {
                blossom.setPosition(
                    static_cast<float>(rand() % SCREEN_WIDTH),
                    static_cast<float>(-50 - rand() % 100)
                );

                // Random lại màu sắc
                int colorVar = rand() % 50;
                blossom.setColor(sf::Color(255, 200 + colorVar, 220 + colorVar, 180 + rand() % 75));
            }
        }
    }

    void update(float deltaTime) {
        totalTime += deltaTime;
        coupletDropTime += deltaTime;

        // Cập nhật câu đối
        updateCouplet(leftCouplet, deltaTime);
        updateCouplet(rightCouplet, deltaTime);

        // Cập nhật particles
        updateParticles(deltaTime);

        // Cập nhật pháo hoa nền
        updateBackgroundFireworks(deltaTime);

        // Cập nhật hoa mai
        if (!cherryBlossoms.empty()) {
            updateCherryBlossoms(deltaTime);
        }

        // Tạo pháo hoa nền ngẫu nhiên
        createBackgroundFirework();

        // Cập nhật hiệu ứng giấy đỏ
        updatePaperEffect();
    }

    void updateCouplet(std::vector<CoupletCharacter>& couplet, float deltaTime) {
        for (auto& ch : couplet) {
            ch.currentDelay++;

            if (ch.currentDelay < ch.delayFrames) {
                continue;
            }

            if (!ch.atTarget) {
                // Hiệu ứng rơi với trọng lực
                ch.velocity.y += GRAVITY * deltaTime;
                ch.currentPosition.y += ch.velocity.y * deltaTime;

                // Hiệu ứng xoay dần về 0
                ch.rotation += (ch.targetRotation - ch.rotation) * 5.0f * deltaTime;

                // Phóng to dần
                ch.scale += (ch.targetScale - ch.scale) * 4.0f * deltaTime;
                ch.text.setScale(ch.scale, ch.scale);

                // Kiểm tra chạm đích
                if (ch.currentPosition.y >= ch.targetPosition.y) {
                    ch.currentPosition.y = ch.targetPosition.y;

                    if (ch.velocity.y > 50.0f) {
                        // Hiệu ứng nảy
                        ch.velocity.y = -ch.velocity.y * ch.bounceFactor;
                        ch.bounceFactor *= 0.6f;

                        // Hiệu ứng rung
                        ch.shakeTimer = 0.3f;
                        ch.shakeIntensity = ch.velocity.y * 0.01f;

                        // Tạo tia lửa khi chạm
                        createMiniSparkles(ch.currentPosition);
                    } else {
                        ch.atTarget = true;
                        ch.velocity = sf::Vector2f(0, 0);
                        ch.isGlowing = true;

                        // Pháo hoa lớn khi đến đích
                        createFirework(ch.currentPosition, 200);
                    }
                }

                ch.text.setPosition(ch.currentPosition);
            } else {
                // Hiệu ứng sau khi đến đích

                // Nhấp nháy màu sắc
                ch.glowPhase += deltaTime * 3.0f;
                float glowValue = (sin(ch.glowPhase) + 1.0f) / 2.0f;

                // Chuyển đổi màu sắc giữa vàng và đỏ
                sf::Color baseColor = ch.text.getFillColor();
                sf::Color targetColor = (baseColor == festiveColors[0]) ? festiveColors[1] : festiveColors[0];

                sf::Color newColor(
                    static_cast<sf::Uint8>(baseColor.r + (targetColor.r - baseColor.r) * glowValue),
                    static_cast<sf::Uint8>(baseColor.g + (targetColor.g - baseColor.g) * glowValue),
                    static_cast<sf::Uint8>(baseColor.b + (targetColor.b - baseColor.b) * glowValue)
                );

                ch.text.setFillColor(newColor);

                // Hiệu ứng rung nhẹ
                if (ch.shakeTimer > 0) {
                    ch.shakeTimer -= deltaTime;
                    float shakeX = (rand() % 100 - 50) * ch.shakeIntensity;
                    float shakeY = (rand() % 100 - 50) * ch.shakeIntensity;
                    ch.text.setPosition(ch.currentPosition + sf::Vector2f(shakeX, shakeY));
                } else {
                    ch.text.setPosition(ch.currentPosition);
                }

                // Hiệu ứng phóng to thu nhỏ nhẹ
                float pulse = sin(totalTime * 2.0f) * 0.05f;
                ch.text.setScale(ch.scale + pulse, ch.scale + pulse);
            }

            // Áp dụng xoay
            ch.text.setRotation(ch.rotation);
        }
    }

    void updateParticles(float deltaTime) {
        for (auto& p : particles) {
            // Trọng lực
            p.velocity.y += GRAVITY * 0.5f * deltaTime;
            p.position += p.velocity * deltaTime;
            p.lifetime -= deltaTime;

            // Lực cản không khí
            p.velocity *= 0.98f;
        }

        // Xóa particles đã hết lifetime
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const FireworkParticle& p) { return p.lifetime <= 0.0f; }), particles.end());
    }

    void updateBackgroundFireworks(float deltaTime) {
        for (auto& fw : backgroundFireworks) {
            fw.position += fw.velocity * deltaTime;
            fw.lifetime -= deltaTime;

            // Nổ khi hết thời gian bay lên
            if (fw.velocity.y >= 0 && fw.lifetime < fw.maxLifetime * 0.7f) {
                createFirework(fw.position, 50);
                fw.lifetime = 0; // Xóa firework chính
            }
        }

        backgroundFireworks.erase(std::remove_if(backgroundFireworks.begin(), backgroundFireworks.end(),
            [](const FireworkParticle& fw) { return fw.lifetime <= 0.0f; }), backgroundFireworks.end());
    }

    void updatePaperEffect() {
        // Hiệu ứng rung nhẹ cho giấy đỏ
        if (leftPaperSprite.getTexture() && rightPaperSprite.getTexture()) {
            float paperShake = sin(totalTime * 1.5f) * 2.0f;

            leftPaperSprite.setPosition(
                SCREEN_WIDTH/4 - 150 + paperShake,
                100 + paperShake * 0.5f
            );

            // Hiệu ứng trong suốt theo nhịp
            float alpha = 150 + sin(totalTime * 2.0f) * 50;
            leftPaperSprite.setColor(sf::Color(255, 200, 200, static_cast<sf::Uint8>(alpha)));

            rightPaperSprite.setPosition(
                3*SCREEN_WIDTH/4 - 150 - paperShake,
                100 - paperShake * 0.5f
            );

            alpha = 150 + cos(totalTime * 2.0f) * 50;
            rightPaperSprite.setColor(sf::Color(255, 200, 200, static_cast<sf::Uint8>(alpha)));
        }
    }

    void render() {
        window.clear(sf::Color(10, 10, 30)); // Màu nền đêm

        // Vẽ background
        if (backgroundSprite.getTexture()) {
            window.draw(backgroundSprite);
        }

        // Vẽ hoa mai (nếu có)
        for (const auto& blossom : cherryBlossoms) {
            window.draw(blossom);
        }

        // Vẽ pháo hoa nền (vẽ trước để ở dưới)
        for (const auto& fw : backgroundFireworks) {
            if (fw.lifetime > 0) {
                float lifeRatio = fw.lifetime / fw.maxLifetime;
                sf::CircleShape particle(fw.size * lifeRatio);
                particle.setPosition(fw.position);
                particle.setFillColor(sf::Color(
                    fw.color.r,
                    fw.color.g,
                    fw.color.b,
                    static_cast<sf::Uint8>(255 * lifeRatio)
                ));
                window.draw(particle);
            }
        }

        // Vẽ giấy đỏ nền cho câu đối
        if (leftPaperSprite.getTexture()) {
            window.draw(leftPaperSprite);
        }
        if (rightPaperSprite.getTexture()) {
            window.draw(rightPaperSprite);
        }

        // Vẽ particles (tia lửa)
        for (const auto& p : particles) {
            if (p.lifetime > 0) {
                float lifeRatio = p.lifetime / p.maxLifetime;
                float size = p.size * lifeRatio;

                // Tạo hiệu ứng lấp lánh với hình dạng khác nhau
                if (rand() % 100 < 30) { // 30% là hình sao
                    // Vẽ hình sao đơn giản
                    sf::CircleShape star(size, 5);
                    star.setPosition(p.position);
                    star.setFillColor(sf::Color(
                        p.color.r,
                        p.color.g,
                        p.color.b,
                        static_cast<sf::Uint8>(255 * lifeRatio)
                    ));
                    star.setRotation(totalTime * 100);
                    window.draw(star);
                } else {
                    // Hình tròn thông thường
                    sf::CircleShape particle(size);
                    particle.setPosition(p.position);
                    particle.setFillColor(sf::Color(
                        p.color.r,
                        p.color.g,
                        p.color.b,
                        static_cast<sf::Uint8>(255 * lifeRatio)
                    ));
                    window.draw(particle);
                }
            }
        }

        // Vẽ hiệu ứng glow cho chữ (bằng cách vẽ nhiều lớp)
        for (const auto& ch : leftCouplet) {
            if (ch.isGlowing) {
                drawTextWithGlow(ch);
            }
        }
        for (const auto& ch : rightCouplet) {
            if (ch.isGlowing) {
                drawTextWithGlow(ch);
            }
        }

        // Vẽ chữ chính
        for (const auto& ch : leftCouplet) {
            window.draw(ch.text);
        }
        for (const auto& ch : rightCouplet) {
            window.draw(ch.text);
        }

        // Vẽ hiệu ứng chúc mừng năm mới
        if (coupletDropTime > 3.0f) {
            drawNewYearGreeting();
        }

        // Vẽ hướng dẫn
        drawInstructions();

        window.display();
    }

    void drawTextWithGlow(const CoupletCharacter& ch) {
        // Tạo hiệu ứng glow bằng cách vẽ nhiều lớp với độ trong suốt khác nhau
        sf::Text glowText = ch.text;

        // Lớp 1: Glow lớn nhất, mờ nhất
        for (int i = 5; i >= 1; i--) {
            glowText.setScale(ch.text.getScale().x + i * 0.03f, ch.text.getScale().y + i * 0.03f);

            sf::Color glowColor = ch.text.getFillColor();
            glowColor.a = static_cast<sf::Uint8>(50 / i);

            glowText.setFillColor(glowColor);
            glowText.setOutlineColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(30 / i)));

            // Dịch chuyển nhẹ để tạo hiệu ứng ánh sáng
            float offset = i * 0.5f;
            glowText.setPosition(ch.text.getPosition() + sf::Vector2f(offset, offset));

            window.draw(glowText);
        }
    }

    void drawNewYearGreeting() {
        static sf::Text greeting;
        static bool initialized = false;

        if (!initialized && fontLoaded) {
            greeting.setFont(font);
            greeting.setString(L"CHÚC MỪNG \n\n    NĂM MỚI");
            greeting.setCharacterSize(46);
            greeting.setFillColor(sf::Color(255, 215, 0));
            greeting.setOutlineColor(sf::Color::Red);
            greeting.setOutlineThickness(3.0f);
            greeting.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = greeting.getLocalBounds();
            greeting.setOrigin(bounds.width / 2, bounds.height / 2 + 70);
            greeting.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT - 130);

            initialized = true;
        }

        if (initialized) {
            // Hiệu ứng nhấp nháy
            float pulse = sin(totalTime * 3.0f) * 0.1f + 1.0f;
            greeting.setScale(pulse, pulse);

            // Đổi màu
            float colorPulse = (sin(totalTime * 2.0f) + 1.0f) / 2.0f;
            sf::Color newColor(
                static_cast<sf::Uint8>(255),
                static_cast<sf::Uint8>(215 - 100 * colorPulse),
                static_cast<sf::Uint8>(100 * colorPulse)
            );
            greeting.setFillColor(newColor);

            window.draw(greeting);
        }
    }

    void drawInstructions() {
        static sf::Text instructions;
        static bool initialized = false;

        if (!initialized && fontLoaded) {
            instructions.setFont(font);
            instructions.setString(L"SPACE: Tạo pháo hoa | ESC: Thoát");
            instructions.setCharacterSize(24);
            instructions.setFillColor(sf::Color(200, 200, 200));
            instructions.setPosition(10, 10);

            initialized = true;
        }

        if (initialized) {
            // Làm mờ nhẹ theo thời gian
            float alpha = 150 + sin(totalTime * 2.0f) * 50;
            instructions.setFillColor(sf::Color(200, 200, 200, static_cast<sf::Uint8>(alpha)));

            window.draw(instructions);
        }
    }

    void run() {
        sf::Clock frameClock;

        while (window.isOpen()) {
            sf::Time deltaTime = frameClock.restart();

            // Xử lý sự kiện
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    if (event.key.code == sf::Keyboard::Space) {
                        // Tạo pháo hoa tại vị trí ngẫu nhiên khi nhấn Space
                        createFirework(sf::Vector2f(
                            static_cast<float>(rand() % SCREEN_WIDTH),
                            static_cast<float>(rand() % SCREEN_HEIGHT)
                        ), 150);
                    }
                }
            }

            update(deltaTime.asSeconds());
            render();
        }
    }
};

int main() {
    // Set locale để hỗ trợ Unicode
    std::locale::global(std::locale(""));

    VietnameseNewYearFireworks app;

    if (!app.init()) {
        std::cerr << "Khoi tao that bai!\n";
        return 1;
    }

    std::cout << "Chuong trinh Phao Hoa Tet da san sang!\n";
    std::cout << "Nhan SPACE de tao phao hoa ngau nhien\n";
    std::cout << "Nhan ESC de thoat\n";

    app.run();
    return 0;
}
