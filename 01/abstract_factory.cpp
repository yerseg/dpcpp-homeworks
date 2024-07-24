#include <functional>
#include <iostream>
#include <memory>

class Button {
public:
    virtual void render() const noexcept = 0;

    virtual void onClick(std::function<void()>) const noexcept = 0;
};

class WinButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "Windows Button" << std::endl;
    }

    void onClick(std::function<void()>) const noexcept override {
    }
};

class MacOSButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "MacOS Button" << std::endl;
    }

    void onClick(std::function<void()>) const noexcept override {
    }
};

class TextBox {
public:
    virtual void render() const noexcept = 0;

    void setText(const std::string& text) noexcept {
        this->text = text;
    };

    std::string text;
};

class WinText : public TextBox {
public:
    void render() const noexcept override {
        std::cout << "This is windows style text: " << text << std::endl;
    }
};

class MacOSText : public TextBox {
public:
    void render() const noexcept override {
        std::cout << "This is MacOS style text: " << text << std::endl;
    }
};

class AbstractModalDialogFactory {
public:
    virtual std::shared_ptr<Button> createButton() const noexcept = 0;

    virtual std::shared_ptr<TextBox> createTextBox() const noexcept = 0;
};

class WindowsModalDialogFactory : public AbstractModalDialogFactory {
public:
    std::shared_ptr<Button> createButton() const noexcept override {
        return std::make_shared<WinButton>();
    }

    std::shared_ptr<TextBox> createTextBox() const noexcept override {
        return std::make_shared<WinText>();
    }
};

class MacOSModalDialogFactory : public AbstractModalDialogFactory {
public:
    std::shared_ptr<Button> createButton() const noexcept override {
        return std::make_shared<MacOSButton>();
    }

    std::shared_ptr<TextBox> createTextBox() const noexcept override {
        return std::make_shared<MacOSText>();
    }
};

class Application {
public:
    explicit Application(std::shared_ptr<AbstractModalDialogFactory> factory)
        : factory_(std::move(factory)) {
        render();
    }
    void render() const noexcept {
        std::shared_ptr<Button> button = factory_->createButton();
        std::shared_ptr<TextBox> textBox = factory_->createTextBox();
        textBox->setText("Hello world");
        button->render();
        textBox->render();
    }

private:
    std::shared_ptr<AbstractModalDialogFactory> factory_;
};

int abstract_factory_main() {
    std::string platform;
    std::cout << "Enter target platform:" << std::endl;
    std::cin >> platform;
    std::shared_ptr<AbstractModalDialogFactory> factory;
    if (platform == "win") {
        factory = std::make_shared<WindowsModalDialogFactory>();
    } else if (platform == "macos") {
        factory = std::make_shared<MacOSModalDialogFactory>();
    } else {
        std::cout << "Unknown platform" << std::endl;
        return 1;
    }
    auto app = Application(factory);
    return 0;
}
