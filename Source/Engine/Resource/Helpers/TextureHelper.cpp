#include "TextureHelper.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <wincodec.h>
#endif
namespace MAGE
{

#if defined(DELUSION_WINDOWS)
	TextureProps ReadTextureProperties(const string& absolutePath)
	{
		IWICImagingFactory* pFactory = nullptr;
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pFrame = nullptr;

		HRESULT hr = CoInitialize(nullptr);
		hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
		if (FAILED(hr)) throw std::runtime_error("Failed to initialize WIC Imaging Factory.");

		hr = pFactory->CreateDecoderFromFilename(std::wstring(absolutePath.begin(), absolutePath.end()).c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (FAILED(hr)) throw std::runtime_error("Failed to create decoder for the image file.");

		hr = pDecoder->GetFrame(0, &pFrame);
		if (FAILED(hr)) throw std::runtime_error("Failed to retrieve the image frame.");

		UINT width, height;
		hr = pFrame->GetSize(&width, &height);
		if (FAILED(hr)) throw std::runtime_error("Failed to get image dimensions.");

		WICPixelFormatGUID pixelFormat;
		hr = pFrame->GetPixelFormat(&pixelFormat);
		if (FAILED(hr)) throw std::runtime_error("Failed to get pixel format.");

		TextureProps props;
		props.texSize = { static_cast<u32>(width), static_cast<u32>(height), 1 };

		IWICPixelFormatInfo* pPixelFormatInfo = nullptr;
		hr = pFactory->CreateComponentInfo(pixelFormat, (IWICComponentInfo**)&pPixelFormatInfo);
		if (SUCCEEDED(hr) && pPixelFormatInfo)
		{
			UINT bitsPerPixel;
			pPixelFormatInfo->GetBitsPerPixel(&bitsPerPixel);
			props.depthBit = bitsPerPixel / props.channelCount;

			if (pixelFormat == GUID_WICPixelFormat8bppGray)
			{
				props.channelCount = 1;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat16bppGray)
			{
				props.channelCount = 1;
				props.depthBit = 16;
			}
			else if (pixelFormat == GUID_WICPixelFormat24bppBGR)
			{
				props.channelCount = 3;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat24bppRGB)
			{
				props.channelCount = 3;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat32bppBGRA)
			{
				props.channelCount = 4;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat32bppRGBA)
			{
				props.channelCount = 4;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat32bppBGR)
			{
				props.channelCount = 3;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat32bppPBGRA)
			{
				props.channelCount = 4;
				props.depthBit = 8;
			}
			else if (pixelFormat == GUID_WICPixelFormat48bppRGB)
			{
				props.channelCount = 3;
				props.depthBit = 16;
			}
			else if (pixelFormat == GUID_WICPixelFormat64bppRGBA)
			{
				props.channelCount = 4;
				props.depthBit = 16;
			}
			else if (pixelFormat == GUID_WICPixelFormat64bppPRGBA)
			{
				props.channelCount = 4;
				props.depthBit = 16;
			}
			else if (pixelFormat == GUID_WICPixelFormat96bppRGBFloat)
			{
				props.channelCount = 3;
				props.depthBit = 32;
			}
			else if (pixelFormat == GUID_WICPixelFormat128bppRGBAFloat)
			{
				props.channelCount = 4;
				props.depthBit = 32;
			}
			pPixelFormatInfo->Release();
		}
		else
			throw std::runtime_error("Unsupported pixel format.");

		GUID containerFormat;
		hr = pDecoder->GetContainerFormat(&containerFormat);
		/*if (SUCCEEDED(hr))
			if (containerFormat == GUID_ContainerFormatPng || containerFormat == GUID_ContainerFormatJpeg || containerFormat == GUID_ContainerFormatTiff)
				return props;*/

		if (pFrame) pFrame->Release();
		if (pDecoder) pDecoder->Release();
		if (pFactory) pFactory->Release();
		CoUninitialize();

		return props;
	}
#endif

	Texture* Helper::CreateTexture(const ResourceProps& resDesc, const TextureProps& texDesc)
	{
		return nullptr;
	}
}
