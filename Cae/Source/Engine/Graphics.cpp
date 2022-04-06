#include "Graphics.h"
#include "Log.h"
#include "Util.h"

#include <fstream> // Remove when done testing for files
#include <comdef.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

namespace Cae {
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };

    struct ConstantBuffer {
        DirectX::XMMATRIX mWorld;
        DirectX::XMMATRIX mView;
        DirectX::XMMATRIX mProjection;
    };

    VERTEX Vertices[] =
    {
        {   -1.0f, 1.0f, -1.0f,     1.0f, 0.0f, 0.0f, 1.0f},
        {   1.0f, 1.0, -1.0f,      0.0f, 1.0f, 0.0f, 1.0f},
        {   1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f},
        {   -1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},

        {   -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f},
        {   1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f},
        {   1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f},
        {   -1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f}
    };


	Graphics::Graphics(HWND hWnd, int width, int height) : width(width), height(height) {
        ScopedTimer("DirectX initialization");
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
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        UINT swapCreateFlags = 0u;

#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        /////////////////////////////////////////////////
        // ////////////////////////////////////////////
        // Create Device + Swap-chain ///////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP, // For testing and "limit" the performance in order to optimize the engine better
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

        tempFloat0 = 0.0f;

        SetupPipeline();
	}

    Graphics::~Graphics() {
        g_SwapChain->SetFullscreenState(FALSE, nullptr);
    }

    void Graphics::RenderFrame() {
        ScopedTimer("Render");
        const FLOAT color[4] = { 0.172f, 0.184f, 0.2f, 1.0f };
        g_Context->ClearRenderTargetView(g_BackBuffer.Get(), color);

        // Render
        tempFloat0 += (float)DirectX::XM_PI * 0.0025f;

        DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(tempFloat0);
        DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(tempFloat0);
        g_World = rotY * rotX;

        ConstantBuffer cb;
        cb.mWorld = DirectX::XMMatrixTranspose(g_World);
        cb.mView = DirectX::XMMatrixTranspose(g_View);
        cb.mProjection = DirectX::XMMatrixTranspose(g_Projection);

        g_Context->UpdateSubresource(g_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        g_Context->VSSetConstantBuffers(0, 1, g_ConstantBuffer.GetAddressOf());

        g_Context->DrawIndexed(36, 0, 0);

        // Swap buffer
        HRESULT hr;
        if (FAILED(hr = g_SwapChain->Present(0, 0))) {
            C_ENGINE_ERROR(hr);
        }
    }

    void Graphics::SetupPipeline() {
        ScopedTimer("Shader initialization");
        HRESULT hr;
        Shader* shader = new Shader(this, "ShaderBins\\VertexShader.cso", "ShaderBins\\PixelShader.cso");

        // Load Vertex Shader
        g_Context->VSSetShader(shader->s_VertexShader.Get(), nullptr, 0u);

        // Load Pixel Shader
        g_Context->PSSetShader(shader->s_PixelShader.Get(), nullptr, 0u);

        // Create vertex buffer
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VERTEX) * 8;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;


        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = Vertices;

        hr = g_Device->CreateBuffer(&bd, &sd, &g_VertexBuffer);
        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), "Buffer error", MB_OK | MB_ICONERROR);
        }

        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION",    0,  DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        hr = g_Device->CreateInputLayout(
            ied,
            2,
            shader->s_VSBlob->GetBufferPointer(),
            shader->s_VSBlob->GetBufferSize(),
            &g_Layout
        );
        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), "Input layout error", MB_OK | MB_ICONERROR);
        }

        g_Context->IASetInputLayout(g_Layout.Get());

        // Set vertex buffer
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        g_Context->IASetVertexBuffers(0, 1, g_VertexBuffer.GetAddressOf(), &stride, &offset);

        // Create index buffer
        WORD Indices[] = {
            3,1,0,
            2,1,3,

            0,5,4,
            1,5,0,

            3,4,7,
            0,4,3,

            1,6,5,
            2,6,1,

            2,7,6,
            3,7,2,

            6,4,5,
            7,4,6,
        };

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WORD) * 36;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        sd.pSysMem = Indices;

        hr = g_Device->CreateBuffer(&bd, &sd, &g_IndexBuffer);
        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), "Buffer error", MB_OK | MB_ICONERROR);
        }

        g_Context->IASetIndexBuffer(g_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Create the constant buffer
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBuffer);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        g_Device->CreateBuffer(&bd, nullptr, &g_ConstantBuffer);
        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), "Buffer error", MB_OK | MB_ICONERROR);
        }

        // Initialize the world matrix
        g_World = DirectX::XMMatrixIdentity();

        // Initialize the view matrix
        DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
        DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        g_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);

        // Initialize the projection matrix
        g_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

    }

    Shader::Shader(Graphics* gfx, const char* vertexShaderDir, const char* pixelShaderDir) {
        HRESULT hr;
        /////////////////////////////////////////////////////////////////////////////////////////////
        // Create Vertex Shader /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
        hr = D3DReadFileToBlob(ToWide(vertexShaderDir).c_str(), &s_VSBlob);

        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), vertexShaderDir, MB_OK | MB_ICONERROR);
        }

        gfx->GetDevice()->CreateVertexShader(
            s_VSBlob->GetBufferPointer(),
            s_VSBlob->GetBufferSize(),
            nullptr,
            &s_VertexShader
        );

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Create Pixel Shader //////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
        hr = D3DReadFileToBlob(ToWide(pixelShaderDir).c_str(), &s_PSBlob);

        if (FAILED(hr)) {
            _com_error err(hr);
            MessageBox(nullptr, err.ErrorMessage(), pixelShaderDir, MB_OK | MB_ICONERROR);
        }

        gfx->GetDevice()->CreatePixelShader(
            s_PSBlob->GetBufferPointer(),
            s_PSBlob->GetBufferSize(),
            nullptr,
            &s_PixelShader
        );

    }
}