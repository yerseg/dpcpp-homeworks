#include <functional>
#include <iostream>

class Button {
public:
    virtual void render() const noexcept = 0;

    virtual void onClick(std::function<void()>) const noexcept = 0;
};

class WinButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "Windows Button";
    }

    void onClick(std::function<void()>) const noexcept override {
    }
};

class MacOSButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "MacOS Button";
    }

    void onClick(std::function<void()>) const noexcept override {
    }
};

class ModalDialog {
public:
    void createWindow() const noexcept {
        std::shared_ptr<Button> button = createButton();
        button->render();
        button->onClick([]() {});
    }

    virtual std::shared_ptr<Button> createButton() const noexcept = 0;
};

class WindowsModalDialog : public ModalDialog {
public:
    std::shared_ptr<Button> createButton() const noexcept override {
        return std::make_shared<WinButton>();
    }
};

class MacOSModalDialog : public ModalDialog {
public:
    std::shared_ptr<Button> createButton() const noexcept override {
        return std::make_shared<MacOSButton>();
    }
};

int fabric_method_main() {
    std::string platform;
    std::cout << "Enter target platform:" << std::endl;
    std::cin >> platform;
    if (platform == "win") {
        WindowsModalDialog().createWindow();
    } else if (platform == "macos") {
        MacOSModalDialog().createWindow();
    } else {
        std::cout << "Unknown platform" << std::endl;
        return 1;
    }
    return 0;
}
