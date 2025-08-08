// Точка входа приложения. Инициализирует окно SFML, сцену MainScene и цикл обработки событий/отрисовки. Также управляет полноэкранным/оконным режимом.
// ------------------------------------------------------------

#include "mainScene.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
// #include <windows.h>
// Вообще нужный комит

// Конструктор класса.
void setTitle(sf::RenderWindow &window)
{
  // Конструктор класса.
  window.setTitle(utf8_to_wstring(Resource::localization["window name"]));
}

// Конструктор класса.
void setFullscreen(sf::RenderWindow &window, bool &isFullscreen)
{
  // Конструктор класса.
  if (!isFullscreen)
  {
    // Конструктор класса.
    window.create(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen);
    // Конструктор класса.
    setTitle(window);
    // Конструктор класса.
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "fullscreen";
    isFullscreen = true;
  }
}

// Конструктор класса.
void setWindowed(sf::RenderWindow &window, bool &isFullscreen)
{
  // Конструктор класса.
  if (isFullscreen)
  {
    // Конструктор класса.
    window.create(sf::VideoMode(800, 600),
                  // Конструктор класса.
                  utf8_to_wstring(Resource::localization["window name"]),
                  sf::Style::Default);
    // Конструктор класса.
    setTitle(window);
    // Конструктор класса.
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "windowed";
    isFullscreen = false;
  }
}

// Конструктор класса.
int main()
{
  // ShowWindow(GetConsoleWindow(), SW_HIDE);

  bool isFullscreen = Resource::userSettings["screen mode"] != "fullscreen";
  sf::RenderWindow window;
  // Конструктор класса.
  if (!isFullscreen)
  {
    // Конструктор класса.
    setFullscreen(window, isFullscreen);
  }
  else
  {
    // Конструктор класса.
    setWindowed(window, isFullscreen);
  }

  // Конструктор класса.
  MainScene mainScene(
      // Конструктор класса.
      [&window]()
      { window.close(); },
      // Конструктор класса.
      [&window, &isFullscreen]()
      { setFullscreen(window, isFullscreen); },
      // Конструктор класса.
      [&window, &isFullscreen]()
      { setWindowed(window, isFullscreen); },
      // Конструктор класса.
      [&window]()
      { setTitle(window); }, window.getSize());

  // Конструктор класса.
  while (window.isOpen())
  {
    sf::Event event;

    // Конструктор класса.
    while (window.pollEvent(event))
    {
      // Конструктор класса.
      if (event.type == sf::Event::Closed)
        // Конструктор класса.
        window.close();

      // Конструктор класса.
      if (event.type == sf::Event::Resized)
      {
        // Конструктор класса.
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        // Конструктор класса.
        window.setView(sf::View(visibleArea));
      }

      // Конструктор класса.
      mainScene.eventProcessing(event);
      // Конструктор класса.
      Resource::writeJson("assets/settings.json", Resource::userSettings);
    }
    // Конструктор класса.
    mainScene.update();

    // Конструктор класса.
    window.clear(sf::Color::White);
    // Draw your game objects here
    // Конструктор класса.
    window.draw(mainScene);
    // Конструктор класса.
    window.display();
  }

  return 0;
}