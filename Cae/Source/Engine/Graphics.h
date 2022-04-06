#pragma once
#include <wrl.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "Types.h"

namespace Cae {
	class Graphics {
	public:
		Graphics(HWND hWnd, int width, int height);
		~Graphics();

		void RenderFrame();

		ID3D11Device* GetDevice() { return g_Device.Get(); }
	private:
		void SetupPipeline();
	private:
		UINT width;
		UINT height;
		D3D_FEATURE_LEVEL g_FeatureLevel;

		float tempFloat0;

		Microsoft::WRL::ComPtr<ID3D11Device>			g_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			g_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		g_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	g_BackBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	g_DepthStencilView;

		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_ConstantBuffer;

		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_IndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>		g_Layout;

	private:
		DirectX::XMMATRIX g_World;
		DirectX::XMMATRIX g_View;
		DirectX::XMMATRIX g_Projection;
	};

	class Shader {
	public:
		Shader(Graphics* gfx, const char* vertexShaderDir, const char* pixelShaderDir);
		~Shader() {}

		Microsoft::WRL::ComPtr<ID3D11VertexShader> s_VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> s_VSBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> s_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> s_PSBlob;
	};
}