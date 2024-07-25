#include <iostream>
#include <string>

struct ITextRenderer {
    virtual ~ITextRenderer() = default;

    virtual std::string render(const std::string& data) const = 0;
};

struct TextRenderer : ITextRenderer {
    std::string render(const std::string& data) const override {
        return data;
    }
};

class TextRendererDecorator : public ITextRenderer {
public:
    explicit TextRendererDecorator(std::shared_ptr<ITextRenderer> renderer)
        : renderer_(std::move(renderer)) {
    }

public:
    std::string render(const std::string& data) const override {
        return renderer_->render(data);
    }

private:
    std::shared_ptr<ITextRenderer> renderer_;
};

class ItalicText : public TextRendererDecorator {
public:
    explicit ItalicText(std::shared_ptr<ITextRenderer> text)
        : TextRendererDecorator(std::move(text)) {
    }

    std::string render(const std::string& data) const override {
        std::string result;
        result += "<i>" + TextRendererDecorator::render(data) + "</i>";
        return result;
    }
};

class BoldText : public TextRendererDecorator {
public:
    explicit BoldText(std::shared_ptr<ITextRenderer> text)
        : TextRendererDecorator(std::move(text)) {
    }

    std::string render(const std::string& data) const {
        std::string result;
        result += "<b>" + TextRendererDecorator::render(data) + "</b>";
        return result;
    }

private:
    std::shared_ptr<ITextRenderer> text_;
};

int main() {
    auto not_decorated_renderer = std::make_shared<TextRenderer>();
    auto italic_renderer = std::make_shared<ItalicText>(not_decorated_renderer);
    auto bold_italic_renderer = std::make_shared<BoldText>(italic_renderer);

    std::cout << not_decorated_renderer->render("Hello, World!") << std::endl;
    std::cout << italic_renderer->render("Hello, World!") << std::endl;
    std::cout << bold_italic_renderer->render("Hello, World!") << std::endl;

    return 0;
}
