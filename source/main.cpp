/**
 * @file main.cpp
 * @brief Точка входа приложения: окно SFML, MainScene, обработка событий, смена
 * режимов экрана.
 */

#include "mainScene.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>

/**
 * @brief Установить локализованный заголовок окна.
 */
void setTitle(sf::RenderWindow &window) {
  window.setTitle(utf8_to_wstring(Resource::localization["window name"]));
}

/**
 * @brief Переключить окно в полноэкранный режим.
 * @param isFullscreen Флаг-состояние (обновляется по месту).
 *
 * Побочный эффект: пишет "fullscreen" в Resource::userSettings["screen mode"].
 */
void setFullscreen(sf::RenderWindow &window, bool &isFullscreen) {
  if (!isFullscreen) {
    window.create(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "fullscreen";
    isFullscreen = true;
  }
}

/**
 * @brief Переключить окно в оконный режим (800x600).
 * @param isFullscreen Флаг-состояние (обновляется по месту).
 *
 * Побочный эффект: пишет "windowed" в Resource::userSettings["screen mode"].
 */
void setWindowed(sf::RenderWindow &window, bool &isFullscreen) {
  if (isFullscreen) {
    window.create(sf::VideoMode(800, 600),
                  utf8_to_wstring(Resource::localization["window name"]),
                  sf::Style::Default);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "windowed";
    isFullscreen = false;
  }
}

int main() {
  // true, если сейчас режим НЕ fullscreen -> надо создать окно windowed
  bool isFullscreen = Resource::userSettings["screen mode"] != "fullscreen";
  sf::RenderWindow window;
  if (!isFullscreen) {
    setFullscreen(window, isFullscreen);
  } else {
    setWindowed(window, isFullscreen);
  }

  // Пробрасываем действия в сцену (замыкания на окно/флаг).
  MainScene mainScene(
      [&window]() { window.close(); },
      [&window, &isFullscreen]() { setFullscreen(window, isFullscreen); },
      [&window, &isFullscreen]() { setWindowed(window, isFullscreen); },
      [&window]() { setTitle(window); }, window.getSize());

  // Главный цикл
  while (window.isOpen()) {
    sf::Event event;

    // Очередь событий
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      // Обновление вида под новое окно
      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
      }

      mainScene.eventProcessing(event);
    }

    // Сохраняем настройки пользователя (например, громкость/язык)
    Resource::updateJson();

    // Логика интерфейса
    mainScene.update();

    // Кадр
    window.clear(sf::Color::White);
    window.draw(mainScene);
    window.display();
  }

  return 0;
}
