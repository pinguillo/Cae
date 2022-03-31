#include "Graphics.h"
#include "Log.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

namespace Cae {
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };


	Graphics::Graphics(HWND hWnd, int width, int height) : width(width), height(height) {
        HRESULT hr;
        /////////////////////////////////////////////////////////////////////////////////////////////
        // Create Swap-chain descriptor /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Test what perfoms better later
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 0;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 1;
        scd.OutputWindow = hWnd;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        UINT swapCreateFlags = 0u;

#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Create Device + Swap-chain ///////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP, // For testing
            nullptr,
            swapCreateFlags,
            levels,
            ARRAYSIZE(levels),
            D3D11_SDK_VERSION,
            &scd,
            &g_SwapChain,
            &g_Device,
            &g_FeatureLevel,
            &g_Context
        );

        if (FAILED(hr)) {
            C_ENGINE_ERROR("Creation of Devide+Swap-chain failed");
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Viewport / Backbuffer ////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
        
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;

        hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &pBackBuffer);
        if (FAILED(hr)) { C_ENGINE_ERROR("Swap chain GetBuffer error"); }

        hr = g_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, g_BackBuffer.GetAddressOf());
        if (FAILED(hr)) { C_ENGINE_ERROR("Swap chain CreateRenderTargetView error"); }

        g_Context->OMSetRenderTargets(1, g_BackBuffer.GetAddressOf(), NULL);

        D3D11_TEXTURE2D_DESC bbDesc;
        pBackBuffer->GetDesc(&bbDesc);

        CD3D11_TEXTURE2D_DESC depthStencilDesc(
            DXGI_FORMAT_D24_UNORM_S8_UINT,
            static_cast<UINT> (bbDesc.Width),
            static_cast<UINT> (bbDesc.Height),
            1, // This depth stencil view has only one texture.
            1, // Use a single mipmap level.
            D3D11_BIND_DEPTH_STENCIL
        );

        g_Device->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil);

        CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

        g_Device->CreateDepthStencilView(pDepthStencil.Get(), &depthStencilViewDesc, &g_DepthStencilView);

        D3D11_VIEWPORT viewport = {};
        viewport.Height = (float)bbDesc.Height;
        viewport.Width = (float)bbDesc.Width;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;

        g_Context->RSSetViewports(1, &viewport);
	}

    Graphics::~Graphics() {

    }

    void Graphics::RenderFrame() {
        const FLOAT color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
        g_Context->ClearRenderTargetView(g_BackBuffer.Get(), color);

        // Render

        // Swap buffer
        HRESULT hr;
        if (FAILED(hr = g_SwapChain->Present(0, 0))) {
            C_ENGINE_ERROR(hr);
        }
    }
}