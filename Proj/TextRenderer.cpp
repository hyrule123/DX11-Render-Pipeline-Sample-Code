#include "TextRenderer.h"

#include "CDX.h"
#include <FW1FontWrapper/FW1FontWrapper.h>
#include "Manager.h"

TextRenderer::TextRenderer()
	: m_FW1_factory(nullptr)
	, m_font_wrapper(nullptr)
{
}

TextRenderer::~TextRenderer()
{
}

bool TextRenderer::init()
{
	HRESULT result = FW1CreateFactory(FW1_VERSION, &m_FW1_factory);

	//auto device = 

	//result = m_FW1_factory->CreateFontWrapper(device.Get(), L"Arial", &m_font_wrapper);

	return false;
}


