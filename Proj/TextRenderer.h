#pragma once

struct IFW1Factory;
struct IFW1FontWrapper;

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	bool init();

private:
	IFW1Factory* m_FW1_factory;
	IFW1FontWrapper* m_font_wrapper;
};

