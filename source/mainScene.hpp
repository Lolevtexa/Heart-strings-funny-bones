// Сцена главного меню (MainScene): кнопки «Новая игра», «Загрузить», «Настройки», «Выход» и страницы настроек (громкость, режим экрана, язык).
// ------------------------------------------------------------

// Заголовочный файл. pragma once — защита от множественного включения.
#pragma once
#include "widgets/activatable/clickable/button.hpp"
#include "widgets/activatable/clickable/radioButton.hpp"
#include "widgets/activatable/clickable/slider.hpp"
#include "widgets/activatable/horizontalWigets.hpp"
#include "widgets/activatable/number.hpp"
#include "widgets/activatable/text.hpp"
#include "widgets/constantable/image.hpp"

// Класс MainScene — см. описание в заголовке файла.
class MainScene : public sf::Drawable
{
private:
  const float menuButtonRatio = 1.f / 4.f;
  const float settingsButtonRatio = 1.f / 3.f;

  int menuButtonWidth;
  int settingsButtonWidth;
  int buttonHeight = 50;
  int buttonIndent = 10;

  std::vector<Button *> menuButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

  std::vector<AText *> textsByKey;
  std::vector<ANumber *> numbers;
  Slider *musicVolumeSliders;

  // Конструктор класса.
  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();
  Image background;

public:
  template <typename Exit, typename SetFullscreen, typename SetWindowed,
            typename SetTitle>
  // Конструктор класса.
  MainScene(Exit exit, SetFullscreen setFullscreen, SetWindowed setWindowed,
            SetTitle setTitle, sf::Vector2u windowSize)
      // Конструктор класса.
      : menuButtonWidth(windowSize.x * menuButtonRatio),
        // Конструктор класса.
        settingsButtonWidth(windowSize.x * settingsButtonRatio),
        // Конструктор класса.
        background(&Resource::background)
  {
    // Конструктор класса.
    backgroundMusic->setVolume(Resource::userSettings["music volme"]);
    // Конструктор класса.
    backgroundMusic->setLoop(true);
    // Конструктор класса.
    backgroundMusic->play();

    // Конструктор класса.
    background.setBound(0, 0, static_cast<float>(windowSize.x),
                        static_cast<float>(windowSize.y), 0);

    // Конструктор класса.
    addMainButton(std::vector<std::string>({"buttons", "new game"}),
                  // Конструктор класса.
                  [this]()
                  { setSettingsPage(1); });
    // Конструктор класса.
    addMainButton(std::vector<std::string>({"buttons", "load game"}),
                  // Конструктор класса.
                  [this]()
                  { setSettingsPage(2); });
    // Конструктор класса.
    addMainButton(std::vector<std::string>({"buttons", "settings"}),
                  // Конструктор класса.
                  [this]()
                  { setSettingsPage(3); });
    // Конструктор класса.
    addMainButton(std::vector<std::string>({"buttons", "exit"}), exit);
    // Конструктор класса.
    updateMainButtonBounds();

    // Конструктор класса.
    addSettingsTextButton(1,
                          std::vector<std::string>({"buttons", "start game"}),
                          []()
                          { std::cout << "Start game" << std::endl; });
    // Конструктор класса.
    addSettingsTextButton(1, std::vector<std::string>({"buttons", "back"}),
                          // Конструктор класса.
                          [this]()
                          { setSettingsPage(0); });

    // Конструктор класса.
    addSettingsTextButton(2, std::vector<std::string>({"buttons", "back"}),
                          // Конструктор класса.
                          [this]()
                          { setSettingsPage(0); });

    // Конструктор класса.
    addSettingsSliderButton(
        3, std::vector<std::string>({"buttons", "music volume"}),
        musicVolumeSliders,
        [this]()
        {
          Resource::userSettings["music volme"] =
              // Конструктор класса.
              musicVolumeSliders->getValue();
        },
        Resource::userSettings["music volme"]);
    // Конструктор класса.
    addSettingsRadioButton(
        3, std::vector<std::string>({"buttons", "screen mode"}),
        std::vector<std::vector<std::string>>(
            {{"buttons", "windowed"}, {"buttons", "fullscreen"}}),
        {setWindowed, setFullscreen});
    // Конструктор класса.
    addSettingsRadioButton(3, std::vector<std::string>({"buttons", "language"}),
                           // Конструктор класса.
                           getLocalizationsNames(),
                           // Конструктор класса.
                           getLocalizationsFunc([this, setTitle]()
                                                {
// Конструктор класса.
                             updateLocalization();
// Конструктор класса.
                             setTitle(); }));
    // Конструктор класса.
    addSettingsTextButton(3, std::vector<std::string>({"buttons", "back"}),
                          // Конструктор класса.
                          [this]()
                          { setSettingsPage(0); });
  }

  // Конструктор класса.
  ~MainScene()
  {
    // Конструктор класса.
    while (menuButtons.size())
    {
      // Конструктор класса.
      delete *(menuButtons.rbegin());
      // Конструктор класса.
      menuButtons.pop_back();
    }

    // Конструктор класса.
    for (auto &settingsButton : settingsButtons)
    {
      // Конструктор класса.
      while (settingsButton.size())
      {
        // Конструктор класса.
        delete *(settingsButton.rbegin());
        // Конструктор класса.
        settingsButton.pop_back();
      }
    }

    // Конструктор класса.
    backgroundMusic->stop();
  }

  // Обработка ввода/событий SFML (мышь/клавиатура/окно).
  void eventProcessing(sf::Event event)
  {
    // Конструктор класса.
    for (auto &button : menuButtons)
    {
      // Конструктор класса.
      button->eventProcessing(event);
    }

    // Конструктор класса.
    for (auto &button : settingsButtons[settingsPage])
    {
      // Конструктор класса.
      button->eventProcessing(event);
    }

    // Конструктор класса.
    if (event.type == sf::Event::Resized)
    {
      // Конструктор класса.
      background.setBound(0, 0, static_cast<float>(event.size.width),
                          static_cast<float>(event.size.height), 0);
      menuButtonWidth = event.size.width / 4;
      settingsButtonWidth = event.size.width / 2;
    }
  }

  // Обновление состояния/логики перед отрисовкой.
  void update()
  {
    // Конструктор класса.
    for (auto &button : menuButtons)
    {
      // Конструктор класса.
      button->update();
    }

    // Конструктор класса.
    for (auto &button : settingsButtons[settingsPage])
    {
      // Конструктор класса.
      button->update();
    }

    // Конструктор класса.
    updateMainButtonBounds();
    // Конструктор класса.
    updateSettingsButtonsBound();
    // Конструктор класса.
    backgroundMusic->setVolume(musicVolumeSliders->getValue());
  }

  // Отрисовка объекта на целевой поверхности.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    // Конструктор класса.
    target.draw(background, states);

    // Конструктор класса.
    for (auto &button : menuButtons)
    {
      // Конструктор класса.
      target.draw(*button, states);
    }

    // Конструктор класса.
    for (auto &button : settingsButtons[settingsPage])
    {
      // Конструктор класса.
      target.draw(*button, states);
    }
  }

private:
  // Конструктор класса.
  void setSettingsPage(int page)
  {
    settingsPage = settingsPage == page ? 0 : page;
  }

  // Конструктор класса.
  void addMainButton(const std::vector<std::string> &localizationKeys,
                     // Конструктор класса.
                     std::function<void()> func)
  {
    // Конструктор класса.
    AText *text = new AText(localizationKeys);
    // Конструктор класса.
    textsByKey.emplace_back(text);
    // Конструктор класса.
    menuButtons.emplace_back(new Button(func, {text}));
  }

  // Конструктор класса.
  void addMainButton(const std::wstring &text, std::function<void()> func)
  {
    // Конструктор класса.
    AText *textButton = new AText(text);
    // Конструктор класса.
    menuButtons.emplace_back(new Button(func, {textButton}));
  }

  // Конструктор класса.
  void addSettingsTextButton(int page, const std::vector<std::string> &textKeys,
                             // Конструктор класса.
                             std::function<void()> func)
  {
    // Конструктор класса.
    AText *text = new AText(textKeys);
    // Конструктор класса.
    textsByKey.emplace_back(text);
    // Конструктор класса.
    settingsButtons[page].emplace_back(new Button(func, {text}));
  }

  // Конструктор класса.
  void addSettingsTextButton(int page, const std::wstring &text,
                             // Конструктор класса.
                             std::function<void()> func)
  {
    // Конструктор класса.
    AText *textButton = new AText(text);
    // Конструктор класса.
    settingsButtons[page].emplace_back(new Button(func, {textButton}));
  }

  void
  // Конструктор класса.
  addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                         const std::vector<std::vector<std::string>> &textsKeys,
                         // Конструктор класса.
                         const std::vector<std::function<void()>> &funcs)
  {
    // Конструктор класса.
    AText *text = new AText(textKeys);
    // Конструктор класса.
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    // Конструктор класса.
    for (int i = 0; i < textsKeys.size(); i++)
    {
      // Конструктор класса.
      AText *text = new AText(textsKeys[i]);
      // Конструктор класса.
      textsByKey.emplace_back(text);
      // Конструктор класса.
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    // Конструктор класса.
    settingsButtons[page].emplace_back(
        // Конструктор класса.
        new RadioButton([]() {}, {text}, buttons));
  }

  // Конструктор класса.
  void addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                              const std::vector<std::wstring> &textsKeys,
                              // Конструктор класса.
                              const std::vector<std::function<void()>> &funcs)
  {
    // Конструктор класса.
    AText *text = new AText(textKeys);
    // Конструктор класса.
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    // Конструктор класса.
    for (int i = 0; i < textsKeys.size(); i++)
    {
      // Конструктор класса.
      AText *text = new AText(textsKeys[i]);
      // Конструктор класса.
      textsByKey.emplace_back(text);
      // Конструктор класса.
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    // Конструктор класса.
    settingsButtons[page].emplace_back(
        // Конструктор класса.
        new RadioButton([]() {}, {text}, buttons));
  }

  // Конструктор класса.
  void addSettingsSliderButton(int page, std::vector<std::string> textKeys,
                               // Конструктор класса.
                               Slider *&slider, std::function<void()> func,
                               int defaultValue = 50)
  {
    // Конструктор класса.
    AText *text = new AText(textKeys);
    // Конструктор класса.
    textsByKey.emplace_back(text);

    // Конструктор класса.
    slider = new Slider(defaultValue);

    // Конструктор класса.
    std::function<int()> getValue = [&slider]()
    { return slider->getValue(); };
    // Конструктор класса.
    ANumber *number = new ANumber(getValue);
    // Конструктор класса.
    numbers.emplace_back(number);

    // Конструктор класса.
    settingsButtons[page].emplace_back(
        // Конструктор класса.
        new Button(func, {new AHorizontalWigets({text, number}), slider}));
  }

  // Конструктор класса.
  void updateMainButtonBounds()
  {
    int deltaY = 0;
    // Конструктор класса.
    for (auto &button : menuButtons)
    {
      // Конструктор класса.
      button->setBound(buttonIndent,
                       buttonIndent + deltaY,
                       menuButtonWidth, buttonHeight, buttonIndent);
      // Конструктор класса.
      deltaY += button->getBound().getSize().y + buttonIndent;
    }
  }

  // Конструктор класса.
  void updateSettingsButtonsBound()
  {
    // Конструктор класса.
    for (auto &buttonsPage : settingsButtons)
    {
      int deltaY = 0;
      // Конструктор класса.
      for (auto &button : buttonsPage)
      {
        // Конструктор класса.
        button->setBound(
            buttonIndent + menuButtonWidth + buttonIndent,
            buttonIndent + deltaY, settingsButtonWidth, buttonHeight,
            buttonIndent);
        // Конструктор класса.
        deltaY += button->getBound().getSize().y + buttonIndent;
      }
    }
  }

  template <typename Func>
  // Конструктор класса.
  std::vector<std::function<void()>>
  // Конструктор класса.
  getLocalizationsFunc(Func updateLocalization)
  {
    // Конструктор класса.
    std::vector<std::function<void()>> loadLanguageFuncs;
    // Конструктор класса.
    for (auto &langugage : Resource::listLocalizations())
    {
      // Конструктор класса.
      loadLanguageFuncs.emplace_back([langugage, updateLocalization]()
                                     {
// Конструктор класса.
        Resource::localization = Resource::loadJson(langugage);
        Resource::userSettings["localization"] = langugage;
// Конструктор класса.
        updateLocalization(); });
    }

    return loadLanguageFuncs;
  }

  // Конструктор класса.
  std::vector<std::wstring> getLocalizationsNames()
  {
    std::vector<std::wstring> localizationsNames;
    // Конструктор класса.
    std::vector<std::string> listLocalizations = Resource::listLocalizations();

    // Конструктор класса.
    for (auto &langugage : listLocalizations)
    {
      // Конструктор класса.
      nlohmann::json localization = Resource::loadJson(langugage);
      // Конструктор класса.
      localizationsNames.emplace_back(
          // Конструктор класса.
          utf8_to_wstring(localization["language"]));
    }

    return localizationsNames;
  }

  // Конструктор класса.
  void updateLocalization()
  {
    // Конструктор класса.
    for (auto &text : textsByKey)
    {
      // Конструктор класса.
      text->resetString();
    }

    // Конструктор класса.
    updateMainButtonBounds();
    // Конструктор класса.
    updateSettingsButtonsBound();
  }
};