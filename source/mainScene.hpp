/**
 * @file mainScene.hpp
 * @brief Сцена главного меню: главные кнопки и страницы настроек (громкость,
 * режим экрана, язык).
 *
 * Жизненный цикл: eventProcessing() → update() → draw().
 */
#pragma once
#include "widgets/activatable/clickable/button.hpp"
#include "widgets/activatable/clickable/radioButton.hpp"
#include "widgets/activatable/clickable/slider.hpp"
#include "widgets/activatable/horizontalWigets.hpp"
#include "widgets/activatable/number.hpp"
#include "widgets/activatable/text.hpp"
#include "widgets/constantable/image.hpp"

/**
 * @brief Главная сцена с меню и вкладками настроек.
 */
class MainScene : public sf::Drawable {
private:
  /// Доля ширины окна, занимаемая колонкой меню слева.
  const float menuButtonRatio = 1.f / 4.f;
  /// Доля ширины окна, занимаемая колонкой настроек справа.
  const float settingsButtonRatio = 1.f / 3.f;

  int menuButtonWidth;
  int settingsButtonWidth;
  int buttonHeight = 50;
  int buttonIndent = 10;

  std::vector<Button *> menuButtons;

  int settingsPage =
      0; ///< 0 — главная; 1 — Новая игра; 2 — Загрузка; 3 — Настройки.
  std::vector<std::vector<Button *>> settingsButtons{4};

  // Для обновления надписей/значений при смене локализации/громкости.
  std::vector<AText *> textsByKey;
  std::vector<ANumber *> numbers;
  Slider *musicVolumeSliders;

  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();
  Image background;

public:
  /**
   * @brief Конструктор.
   * @tparam Exit          колбэк «Выход» (закрыть окно)
   * @tparam SetFullscreen колбэк «В полный экран»
   * @tparam SetWindowed   колбэк «В окно»
   * @tparam SetTitle      колбэк «Обновить заголовок окна»
   * @param windowSize     исходный размер окна (для первичной компоновки)
   *
   * @note Здесь используется ключ `"music volme"` из userSettings (возможная
   * опечатка в ключе — оставлена как есть).
   */
  template <typename Exit, typename SetFullscreen, typename SetWindowed,
            typename SetTitle>
  MainScene(Exit exit, SetFullscreen setFullscreen, SetWindowed setWindowed,
            SetTitle setTitle, sf::Vector2u windowSize)
      : menuButtonWidth(windowSize.x * menuButtonRatio),
        settingsButtonWidth(windowSize.x * settingsButtonRatio),
        background(&Resource::background) {
    backgroundMusic->setVolume(Resource::userSettings["music volme"]);
    backgroundMusic->setLooping(true);
    backgroundMusic->play();

    // Растягиваем фон на весь экран
    background.setBound(0, 0, static_cast<float>(windowSize.x),
                        static_cast<float>(windowSize.y), 0);

    // Главные кнопки
    addMainButton(std::vector<std::string>({"buttons", "new game"}),
                  [this]() { setSettingsPage(1); });
    addMainButton(std::vector<std::string>({"buttons", "load game"}),
                  [this]() { setSettingsPage(2); });
    addMainButton(std::vector<std::string>({"buttons", "settings"}),
                  [this]() { setSettingsPage(3); });
    addMainButton(std::vector<std::string>({"buttons", "exit"}), exit);
    updateMainButtonBounds();

    // Страница 1 — Новая игра
    addSettingsTextButton(1,
                          std::vector<std::string>({"buttons", "start game"}),
                          []() { std::cout << "Start game" << std::endl; });
    addSettingsTextButton(1, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });

    // Страница 2 — Загрузка (пока только «Назад»)
    addSettingsTextButton(2, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });

    // Страница 3 — Настройки
    addSettingsSliderButton(
        3, std::vector<std::string>({"buttons", "music volume"}),
        musicVolumeSliders,
        [this]() {
          Resource::userSettings["music volme"] =
              musicVolumeSliders->getValue();
        },
        Resource::userSettings["music volme"]);

    addSettingsRadioButton(
        3, std::vector<std::string>({"buttons", "screen mode"}),
        std::vector<std::vector<std::string>>(
            {{"buttons", "windowed"}, {"buttons", "fullscreen"}}),
        {setWindowed, setFullscreen}, getCurrentScreenMode());

    addSettingsRadioButton(3, std::vector<std::string>({"buttons", "language"}),
                           getLocalizationsNames(),
                           getLocalizationsFunc([this, setTitle]() {
                             updateLocalization();
                             setTitle();
                           }),
                           getCurrentLocalizationIndex());

    addSettingsTextButton(3, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });
  }

  /**
   * @brief Освобождает владение сырыми указателями на кнопки и останавливает
   * музыку.
   */
  ~MainScene() {
    while (menuButtons.size()) {
      delete *(menuButtons.rbegin());
      menuButtons.pop_back();
    }

    for (auto &settingsButton : settingsButtons) {
      while (settingsButton.size()) {
        delete *(settingsButton.rbegin());
        settingsButton.pop_back();
      }
    }

    backgroundMusic->stop();
  }

  /**
   * @brief Обработка событий SFML (мышь/клавиатура/окно).
   *
   * @details Прокидывает события в кнопки активной страницы и в главное меню.
   * На ресайз — обновляет фон и перерасчёт ширины колонок.
   */
  void eventProcessing(sf::Event event) {
    for (auto &button : menuButtons) {
      button->eventProcessing(event);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->eventProcessing(event);
    }

    if (auto* r = event.getIf<sf::Event::Resized>()) {
      background.setBound(0, 0, static_cast<float>(r->size.x),
                          static_cast<float>(r->size.y), 0);
      menuButtonWidth = r->size.x / 4;
      settingsButtonWidth = r->size.x / 2;
    }
  }

  /**
   * @brief Логическое обновление перед отрисовкой.
   *
   * @details Обновляет все кнопки, пересчитывает компоновку, подтягивает
   * громкость из слайдера.
   */
  void update() {
    for (auto &button : menuButtons) {
      button->update();
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->update();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
    backgroundMusic->setVolume(musicVolumeSliders->getValue());
  }

  /**
   * @brief Отрисовка сцены.
   */
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(background, states);

    for (auto &button : menuButtons) {
      target.draw(*button, states);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      target.draw(*button, states);
    }
  }

private:
  /**
   * @brief Переключить страницу настроек (повторный выбор — вернуться на 0).
   */
  void setSettingsPage(int page) {
    settingsPage = settingsPage == page ? 0 : page;
  }

  /**
   * @brief Добавить кнопку в главное меню (текст по ключам локализации).
   */
  void addMainButton(const std::vector<std::string> &localizationKeys,
                     std::function<void()> func) {
    AText *text = new AText(localizationKeys);
    textsByKey.emplace_back(text);
    menuButtons.emplace_back(new Button(func, {text}));
  }

  /**
   * @brief Добавить кнопку в главное меню (готовая строка).
   */
  void addMainButton(const std::wstring &text, std::function<void()> func) {
    AText *textButton = new AText(text);
    menuButtons.emplace_back(new Button(func, {textButton}));
  }

  /**
   * @brief Добавить текстовую кнопку на страницу настроек (по ключам
   * локализации).
   */
  void addSettingsTextButton(int page, const std::vector<std::string> &textKeys,
                             std::function<void()> func) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);
    settingsButtons[page].emplace_back(new Button(func, {text}));
  }

  /**
   * @brief Добавить текстовую кнопку на страницу настроек (по строке).
   */
  void addSettingsTextButton(int page, const std::wstring &text,
                             std::function<void()> func) {
    AText *textButton = new AText(text);
    settingsButtons[page].emplace_back(new Button(func, {textButton}));
  }

  /**
   * @brief Добавить RadioButton с вариантами, заданными ключами локализации.
   * @param page         Номер страницы настроек.
   * @param textKeys     Подпись группы (ключи).
   * @param textsKeys    Подписи вариантов (списки ключей).
   * @param funcs        Действия для каждого варианта.
   * @param buttonNumber Индекс выбранного варианта по умолчанию.
   */
  void
  addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                         const std::vector<std::vector<std::string>> &textsKeys,
                         const std::vector<std::function<void()>> &funcs,
                         int buttonNumber = 0) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    for (int i = 0; i < textsKeys.size(); i++) {
      AText *text = new AText(textsKeys[i]);
      textsByKey.emplace_back(text);
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    settingsButtons[page].emplace_back(
        new RadioButton([]() {}, {text}, buttons, buttonNumber));
  }

  /**
   * @brief Перегрузка: добавить RadioButton с уже развёрнутыми строками
   * вариантов.
   */
  void addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                              const std::vector<std::wstring> &textsKeys,
                              const std::vector<std::function<void()>> &funcs,
                              int buttonNumber = 0) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    for (int i = 0; i < textsKeys.size(); i++) {
      AText *text = new AText(textsKeys[i]);
      textsByKey.emplace_back(text);
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    settingsButtons[page].emplace_back(
        new RadioButton([]() {}, {text}, buttons, buttonNumber));
  }

  /**
   * @brief Добавить слайдер-группу: [Текст | Число] + Slider.
   * @param page         Страница настроек.
   * @param textKeys     Подпись группы (ключи).
   * @param slider       Выходной указатель — сюда сохранится созданный Slider.
   * @param func         Действие при применении значения.
   * @param defaultValue Стартовое значение слайдера.
   */
  void addSettingsSliderButton(int page, std::vector<std::string> textKeys,
                               Slider *&slider, std::function<void()> func,
                               int defaultValue = 50) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    slider = new Slider(defaultValue);

    std::function<int()> getValue = [&slider]() { return slider->getValue(); };
    ANumber *number = new ANumber(getValue);
    numbers.emplace_back(number);

    settingsButtons[page].emplace_back(
        new Button(func, {new AHorizontalWigets({text, number}), slider}));
  }

  /**
   * @brief Перекомпоновка кнопок главного меню (вертикально).
   */
  void updateMainButtonBounds() {
    int deltaY = 0;
    for (auto &button : menuButtons) {
      button->setBound(buttonIndent, buttonIndent + deltaY, menuButtonWidth,
                       buttonHeight, buttonIndent);
      deltaY += button->getBound().size.y + buttonIndent;
    }
  }

  /**
   * @brief Перекомпоновка кнопок на всех страницах настроек (вертикально в
   * правой колонке).
   */
  void updateSettingsButtonsBound() {
    for (auto &buttonsPage : settingsButtons) {
      int deltaY = 0;
      for (auto &button : buttonsPage) {
        button->setBound(buttonIndent + menuButtonWidth + buttonIndent,
                         buttonIndent + deltaY, settingsButtonWidth,
                         buttonHeight, buttonIndent);
        deltaY += button->getBound().size.y + buttonIndent;
      }
    }
  }

  /**
   * @brief Индекс текущего режима экрана: 0 — windowed, 1 — fullscreen, -1 —
   * неизвестно.
   */
  int getCurrentScreenMode() const {
    std::string current = Resource::userSettings["screen mode"];
    if (current == "windowed")
      return 0;
    if (current == "fullscreen")
      return 1;
    return -1;
  }

  /**
   * @brief Построить список действий, устанавливающих соответствующую
   * локализацию.
   * @param updateLocalization Колбэк, вызываемый после установки локализации.
   */
  template <typename Func>
  std::vector<std::function<void()>>
  getLocalizationsFunc(Func updateLocalization) {
    std::vector<std::function<void()>> loadLanguageFuncs;
    for (auto &langugage : Resource::listLocalizations()) {
      loadLanguageFuncs.emplace_back([langugage, updateLocalization]() {
        Resource::localization = Resource::loadJson(langugage);
        Resource::userSettings["localization"] = langugage;
        updateLocalization();
      });
    }

    return loadLanguageFuncs;
  }

  /**
   * @brief Получить отображаемые названия доступных локализаций (из JSON).
   */
  std::vector<std::wstring> getLocalizationsNames() {
    std::vector<std::wstring> localizationsNames;
    std::vector<std::string> listLocalizations = Resource::listLocalizations();

    for (auto &langugage : listLocalizations) {
      nlohmann::json localization = Resource::loadJson(langugage);
      localizationsNames.emplace_back(
          utf8_to_wstring(localization["language"]));
    }

    return localizationsNames;
  }

  /**
   * @brief Найти индекс текущей локализации в списке доступных.
   */
  int getCurrentLocalizationIndex() const {
    std::vector<std::string> list = Resource::listLocalizations();
    std::string current = Resource::userSettings["localization"];
    for (int i = 0; i < static_cast<int>(list.size()); ++i)
      if (list[i] == current)
        return i;
    return 0;
  }

  /**
   * @brief Применить локализацию ко всем текстам и пересчитать компоновку.
   */
  void updateLocalization() {
    for (auto &text : textsByKey) {
      text->resetString();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
  }
};
